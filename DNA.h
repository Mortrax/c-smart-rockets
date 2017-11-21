#ifndef DNA_H_
#define DNA_H_

#include "../Vector/vector.h"
#include <math.h>
#define LIFE_SPAN 400

typedef struct DNA {
	Vector genes[LIFE_SPAN];
} DNA;

DNA *CreateDNA() {
	DNA *dna = malloc(sizeof(DNA));
	for (int i = 0; i < LIFE_SPAN; i++) {
		dna->genes[i] = Random();
		SetMagnitude(dna->genes[i], 0.3);
	}
	return dna;
}

DNA *CopyDNA(const DNA *dna) {
	DNA *new_dna = malloc(sizeof(DNA));
	for (int i = 0; i < LIFE_SPAN; i++) {
		new_dna->genes[i] = CopyVector(dna->genes[i]);
	}

	return new_dna;
}

DNA *CrossOver(const DNA *parent_a, const DNA *parent_b) {
	DNA *new_dna = malloc(sizeof(DNA));
	int mid_point = rand() % LIFE_SPAN;
	for (int i = 0; i < LIFE_SPAN; i++) {
		if (i > mid_point) {
			new_dna->genes[i] = CopyVector(parent_a->genes[i]);
		}
		else {
			new_dna->genes[i] = CopyVector(parent_b->genes[i]);
		}
	}
	return new_dna;

}

void Mutation(DNA *dna) {
	for (int i = 0; i < LIFE_SPAN; i++) {
		if (rand() % 200 == 1) {
			DestroyVector(dna->genes[i]);
			dna->genes[i] = Random();
			SetMagnitude(dna->genes[i], 0.3);
		}
	}
}

void DestroyDNA(DNA *dna) {
	for (int i = 0; i < LIFE_SPAN; i++) {
		DestroyVector(dna->genes[i]);
	}
	free(dna);
}


#endif
