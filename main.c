#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "Rocket.h"

//GLOBALS=========================================
const int WIDTH = 700;
const int HEIGHT = 700;
const int FPS = 60;
const int POPULATION_SIZE = 25;
const int MATING_POOL_SIZE = 2500;

void Evaluate(Rocket *rockets[], Rocket *mating_pool[], Vector target, int *mating_pool_count);
void Select(Rocket *rockets[], Rocket *mating_pool[], int mating_pool_count);
void ClearMatingPool(Rocket *mating_pool[], int *mating_pool_size);

int main(int argc, char* argv[]) {
	//allegro variables
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;

	srand(time(NULL));

	//primitive variables
	bool redraw = false;
	bool done = false;
	int count = 0;
	int mating_pool_count = 0;
	int rect_x_tl = 150;
	int rect_y_tl = HEIGHT/2;
	int rect_x_br = WIDTH - 150;
	int rect_y_br = HEIGHT/2 + 10;

	//Game Objects
	Rocket *rockets[POPULATION_SIZE];
	Rocket *mating_pool[MATING_POOL_SIZE];

	Vector target = CreateVector(WIDTH/2, 50);
	
	for (int i = 0; i < POPULATION_SIZE; i++) {
		rockets[i] = CreateRocket(CreateVector(WIDTH/2, HEIGHT - 15),
				CreateVector(0, 0), CreateVector(0, 0), NULL);
	}

	for (int i = 0; i < MATING_POOL_SIZE; i++) {
		mating_pool[i] = NULL;
	}
	
	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	if (!al_init_primitives_addon()) {
		fprintf(stderr, "failed to initialize primitives addon!\n");
		return -1;
	}

	if (!al_install_mouse()) {
		fprintf(stderr, "failed to initialize mouse!\n");
		return -1;
	}


	display = al_create_display(WIDTH, HEIGHT);

	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);

	if (!timer) {
		fprintf(stderr, "failed to create timer!\n");
		al_destroy_display(display);
		return -1;
	}

	event_queue = al_create_event_queue();

	if (!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_timer(timer);
		al_destroy_display(display);
		return -1;
	}


	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_start_timer(timer);

	while (!done) {
	
		ALLEGRO_EVENT ev;

		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;
			for (int i = 0; i < POPULATION_SIZE; i++) {
				UpdateRocket(rockets[i], target, count, rect_x_tl, rect_x_br,
						rect_y_tl, rect_y_br, WIDTH, HEIGHT);
			}
			count++;
			if (count >= LIFE_SPAN) {
				Evaluate(rockets, mating_pool, target, &mating_pool_count);
				Select(rockets, mating_pool, mating_pool_count);
				ClearMatingPool(mating_pool, &mating_pool_count);
				count = 0;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			//@DRAW_STUFF

			for (int i = 0; i < POPULATION_SIZE; i++) {
				DrawRocket(rockets[i]);
			}

			al_draw_filled_circle(GetX(target), GetY(target),
					30, al_map_rgb(255, 255, 255));

			al_draw_filled_rectangle(rect_x_tl, rect_y_tl, rect_x_br, rect_y_br,
					al_map_rgb(255, 255, 255));
			
			al_flip_display();
		
		}
	
	}

	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);
	for (int i = 0; i < POPULATION_SIZE; i++) {
		DestroyRocket(rockets[i]);
	}

	return 0;
}

void Evaluate(Rocket *rockets[], Rocket *mating_pool[], Vector target, int *mating_pool_size) {
	double max_fitness = 0.0001;
	for (int i = 0; i < POPULATION_SIZE; i++) {
		CalculateFitness(rockets[i], target);
		if (rockets[i]->fitness > max_fitness) {
			max_fitness = rockets[i]->fitness;
		}
	}

	for (int i = 0; i < POPULATION_SIZE; i++) {
		rockets[i]->fitness /= max_fitness;
	}

	for (int i = 0; i < POPULATION_SIZE; i++) {
		int n = floor(rockets[i]->fitness * 100);
		for (int j = 0; j < n; j++) {
			mating_pool[(*mating_pool_size)] = CopyRocket(rockets[i]);
			(*mating_pool_size)++;
		}
	}
}

void Select(Rocket *rockets[], Rocket *mating_pool[], int mating_pool_size) {
	DNA *parent_a = NULL;
	DNA *parent_b = NULL;
	DNA *child = NULL;
	
	for (int i = 0; i < POPULATION_SIZE; i++) {

		parent_a = mating_pool[rand() % mating_pool_size]->dna;
		parent_b = mating_pool[rand() % mating_pool_size]->dna;
		child = CrossOver(parent_a, parent_b);
		Mutation(child);
		DestroyRocket(rockets[i]);
		rockets[i] = CreateRocket(CreateVector(WIDTH/2, HEIGHT - 15),
				CreateVector(0, 0), CreateVector(0, 0), child);
	}
}

void ClearMatingPool(Rocket *mating_pool[], int *mating_pool_size) {
	for (int i = 0; i < *mating_pool_size; i++) {
		DestroyRocket(mating_pool[i]);
		mating_pool[i] = NULL;
	}
	*mating_pool_size = 0;


}
