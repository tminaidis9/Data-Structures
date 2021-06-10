//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT Stack.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTStack.h"


void test_create(void) {
	Stack stack = stack_create(NULL);
	stack_set_destroy_value(stack, NULL);

	TEST_ASSERT(stack != NULL);
	TEST_ASSERT(stack_size(stack) == 0);

	stack_destroy(stack);
}

void test_insert(void) {
	Stack stack = stack_create(NULL);
	int N = 1000;
	int* array = malloc(N * sizeof(*array));					// Στο stack θα προσθέσουμε pointers προς τα στοιχεία αυτού του πίνακα

	// insert 1000 στοιχεία
	for (int i = 0; i < 1000; i++) {
		stack_insert_top(stack, &array[i]);
		TEST_ASSERT(stack_size(stack) == i+1);		// Το μέγεθος πρέπει να μεγαλώσει
		TEST_ASSERT(stack_top(stack) == &array[i]);	// Στην κορυφή είναι πάντα το στοιχείο που μόλις βάλαμε!
	}

	stack_destroy(stack);
	free(array);
}

void test_remove(void) {
	Stack stack = stack_create(NULL);
	int N = 1000;
	int* array = malloc(N * sizeof(*array));

	// insert για προσθήκη δεδομένων, χωρίς ελέγχους (έχουμε ξεχωριστό test για το insert)
	for (int i = 0; i < 1000; i++)
		stack_insert_top(stack, &array[i]);

	// Διαδοχικά remove
	for (int i = 999; i >= 0; i--) {
		TEST_ASSERT(stack_top(stack) == &array[i]);
		stack_remove_top(stack);
		TEST_ASSERT(stack_size(stack) == i);
	}

	stack_destroy(stack);
	free(array);
}

// destroy
//
// Η destroy καλείται σε όλα τα tests, για να βρούμε αν δουλεύει σωστά τρέχουμε
//   make valgrind


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "stack_create", test_create },
	{ "stack_insert_top", test_insert },
	{ "stack_remove_top", test_remove },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};