//////////////////////////////////////////////////////////////////
//
// Test για το pq_sort module
//
//////////////////////////////////////////////////////////////////

#include <limits.h>
#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "pq_sort.h"


int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

int* create_int(int value) {
	int* pointer = malloc(sizeof(int));	// δέσμευση μνήμης
	*pointer = value;					// αντιγραφή του value στον νέο ακέραιο
	return pointer;
}


void test_pq_sort_vector(void) {
	Vector vec = vector_create(0, free);

	// προσθήκη τυχαίων αριθμών
	int N = 10;
	for (int i = 0; i < N; i++)
		vector_insert_last(vec, create_int(rand()));

	// sort
	pq_sort_vector(vec, compare_ints);

	// έλεγχος ότι οι τιμές είναι σε αύξουσα σειρά
	int last = INT_MIN;
	for (int i = 0; i < N; i++) {
		int* current = vector_get_at(vec, i);
		TEST_CHECK(last <= *current);			// Το στοιχείο πρέπει να είναι >= από το προηγούμενο
		last = *current;
	}

	vector_destroy(vec);
}

void test_pq_sort_list(void) {
	List list = list_create(NULL);

	int N = 10;
	for(int i = 0 ; i < N ; i++){
		list_insert_next(list,list_last(list),create_int(rand()));
	}

	pq_sort_list(list,compare_ints);

	int last = INT_MIN;
	for (ListNode node = list_first(list) ; node != LIST_EOF ; node = list_next(list,node)) {
		int* current = list_node_value(list,node);
		TEST_CHECK(last <= *current);			
		last = *current;
	}

	list_destroy(list);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_pq_sort_vector", test_pq_sort_vector },
	{ "test_pq_sort_list", test_pq_sort_list },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};