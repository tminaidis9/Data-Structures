#include <stdlib.h>

#include "ADTIntVector.h"

// Στο module αυτό υλοποιούμε ένα IntVector χρησιμοποιώντας μια **ήδη υπάρχουσα υλοποίηδη του Vector**.
// Ολες οι int_vector_* συναρτήσεις είναι απλές και απλά καλούν τις αντίστοιχες συναρτήσεις vector_*.
//
// Για να προσθέσουμε ένα int value στο vector:
// - δεσμεύουμε μνήμη με malloc
// - αντιγράφουμε εκεί το value
// - προσθέρουμε στο vector τον pointer στην μνήμη που δεσμεύσαμε.
//
// Για ευκολία χρησιμοποιούμε pointer cast, επεξήγηση υπάρχει στο modules/UsingADTList/ADTStack.h που χρησιμοποιεί την ίδια τεχνική.


IntVector int_vector_create(int size) {
	// Περνάμε τη free στην vector_create, ώστε κάθε στοιχείο που αφαιρείται να γίνεται free αυτόματα
	Vector vec = vector_create(size, free);

	// Ο πίνακας περιέχει size μη-αρχικοποιημένους ακεραίους, δεσμεύουμε μνήμη για αυτούς
	for (int i = 0; i < size; i ++)
		vector_set_at(vec, i, malloc(sizeof(int)));

	return (IntVector)vec;
}

int int_vector_size(IntVector vec) {
	return vector_size((Vector)vec);	// trivial
}

void int_vector_set_at(IntVector vec, int pos, int value) {
	// Αλλάζουμε την τιμή ενός υπάρχοντος στοιχείου, έχουμε ήδη μνήμη για αυτό! (από το αντίστοιχο insert)
	int *p = vector_get_at((Vector)vec, pos);		// το vector περιέχει int pointers
	*p = value;										// αλλαγή τιμής
}

int int_vector_get_at(IntVector vec, int pos) {
	return *(int*)vector_get_at((Vector)vec, pos);		// το vector περιέχει int pointers
}

void int_vector_insert_last(IntVector vec, int value) {
	// Νέο στοιχείο, χρειαζόμαστε μνήμη!
	int *p = malloc(sizeof(int));
	*p = value;
	vector_insert_last((Vector)vec, p);		// προσθήκη του int pointer στο vector
}

void int_vector_remove_last(IntVector vec) {
	vector_remove_last((Vector)vec);
}

int compare(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

int int_vector_find(IntVector vec, int value) {
	int* res = vector_find((Vector)vec, &value, compare);
	return res == NULL ? INT_MIN : *res;
}

void int_vector_destroy(IntVector vec) {
	vector_destroy((Vector)vec);
}
