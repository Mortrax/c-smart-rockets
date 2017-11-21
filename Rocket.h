#ifndef ROCKET_H_
#define ROCKET_H_
#include "../Vector/vector.h"
#include "DNA.h"
#include <math.h>
typedef struct Rocket {
	Vector position;
	Vector velocity;
	Vector acceleration;
	DNA *dna;
	double fitness;
	bool completed;
	bool crashed;

} Rocket;

Rocket *CreateRocket(Vector p, Vector v, Vector a, DNA *dna) {
	Rocket *result = malloc(sizeof(Rocket));
	result->position = p;
	result->velocity = v;
	result->acceleration = a;
	if (dna == NULL) {
		result->dna = CreateDNA();
	}
	else { 
		result->dna = dna;
	}
	result->fitness = 0;
	result->completed = false;
	result->crashed = false;
	return result;
}

Rocket *CopyRocket(const Rocket *rocket) {
	Rocket *new_rocket = malloc(sizeof(Rocket));
	new_rocket->position = CopyVector(rocket->position);
	new_rocket->velocity = CopyVector(rocket->velocity);
	new_rocket->acceleration = CopyVector(rocket->acceleration);
	new_rocket->dna = CopyDNA(rocket->dna);
	new_rocket->fitness = rocket->fitness;
	new_rocket->completed = rocket->completed;
	new_rocket->crashed = rocket->crashed;
	return new_rocket;

}

void ApplyForce(Rocket *rocket, Vector force) {
	AddVector(rocket->acceleration, force);
}

void CalculateFitness(Rocket *rocket, Vector target) {
	double distance = Distance(rocket->position, target);
	rocket->fitness = 1 / distance;
	if (rocket->completed) {
		rocket->fitness *= 10;
	}
	if (rocket->crashed) {
		rocket->fitness /= 10;
	}
}

void UpdateRocket(Rocket *rocket, Vector target, int count,
	int rect_x_tl, int rect_x_br, int rect_y_tl, int rect_y_br,
	int screen_width, int screen_height) {
	double distance = Distance(rocket->position, target);
	if (distance < 40) {
		rocket->completed = true;
	}

	if (GetX(rocket->position) > rect_x_tl && GetX(rocket->position) < rect_x_br) {
		if (GetY(rocket->position) - 20 < rect_y_tl && GetY(rocket->position) > rect_y_br) {
			rocket->crashed = true;
		}
	}
	
	if (GetX(rocket->position) > screen_width || GetX(rocket->position) < 0) {
		rocket->crashed = true;
	}
	if (GetY(rocket->position) > screen_height || GetY(rocket->position) < 0) {
		rocket->crashed = true;
	}
	ApplyForce(rocket, rocket->dna->genes[count]);
	if (!rocket->completed && !rocket->crashed) {
		AddVector(rocket->velocity, rocket->acceleration);
		AddVector(rocket->position, rocket->velocity);
		MultiplyVector(rocket->acceleration, 0);
	}
}

void DrawRocket(Rocket *rocket) {
	double rocket_end_x = GetX(rocket->position) + 30 * cos(Heading(rocket->velocity));
	double rocket_end_y = GetY(rocket->position) + 30 * sin(Heading(rocket->velocity));
	if (rocket->completed || rocket->crashed) {
		al_draw_line(GetX(rocket->position), GetY(rocket->position), 
				rocket_end_x, rocket_end_y, al_map_rgb(255, 0, 0), 5);	
	}
	else { 
		al_draw_line(GetX(rocket->position), GetY(rocket->position), 
				rocket_end_x, rocket_end_y, al_map_rgb(255, 255, 255), 5);
	}
}

void DestroyRocket(Rocket *rocket) {
	DestroyVector(rocket->position);
	DestroyVector(rocket->velocity);
	DestroyVector(rocket->acceleration);
	DestroyDNA(rocket->dna);
	free(rocket);
}

#endif
