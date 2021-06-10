//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT BList.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTBList.h"

int* create_int(int i){
	int* p = malloc(1);
	*p = i;
	return p;
}

int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}



void test_create(void) {
	BList blist = blist_create(free);
	blist_set_destroy_value(blist,NULL);

	TEST_ASSERT(blist != NULL);
	TEST_ASSERT(blist_size(blist) == 0);

	blist_destroy(blist);
}

void test_insert(void) {
	BList blist = blist_create(free);
	for(int i = 0 ; i < 10 ; i++){
            blist_insert(blist,blist_first(blist),create_int(i));
		TEST_ASSERT(blist_size(blist) == (i + 1));
		TEST_ASSERT(*(int*)blist_node_value(blist, blist_first(blist)) == *(int*)create_int(i));
	}
	int j = 9;
	for(BListNode node = blist_first(blist) ; node != BLIST_EOF; node = blist_next(blist,node)){
		int bnl = *(int*)blist_node_value(blist,node);
		if(bnl != j) TEST_ASSERT(0); 
		if(node != blist_first(blist))
		TEST_ASSERT(*(int*)blist_node_value(blist,blist_previous(blist,node)) == j+1);
		j = j - 1;
	}

	BListNode second_node = blist_next(blist,blist_first(blist));
	blist_insert(blist, second_node, NULL);
	TEST_ASSERT(blist_node_value(blist, blist_previous(blist, second_node)) == NULL);
	TEST_ASSERT(blist_node_value(blist, blist_next(blist, blist_first(blist))) == NULL);

	blist_destroy(blist);
	
}

void test_remove(void) {

	BList blist = blist_create(free);
	for(int i = 0 ; i < 10 ; i++){
            blist_insert(blist,blist_first(blist),create_int(i));
	}

      blist_remove(blist,blist_next(blist,blist_first(blist)));
	
	blist_insert(blist,blist_next(blist,blist_first(blist)),create_int(8));

	for(int i = 0 ; i < 10 ; i++){
            int j = 9-i;
		blist_remove(blist,blist_first(blist));
		if(blist_size(blist) != 0)
                  TEST_ASSERT(*(int*)blist_node_value(blist, blist_first(blist)) == *(int*)create_int(j-1));
		
		TEST_ASSERT(blist_size(blist) == j);
	 }
	blist_destroy(blist);
}

void test_find() {
	BList blist = blist_create(NULL);
	int N = 1000;
	int array[N];

	// Εισάγουμε δοκιμαστικές τιμές στον πίνακα, για να ελέγξουμε την test_find
	for (int i = 0; i < N; i++) {
		array[i] = i;
		blist_insert(blist,blist_first(blist), &array[i]);
	}

	// Εύρεση όλων των στοιχείων
	for (int i = 0; i < N; i++) {
		int* value = blist_find(blist, &i, compare_ints); 
		TEST_ASSERT(value == &array[i]);
	}

	int not_exists = -1;
	TEST_ASSERT(blist_find(blist, &not_exists, compare_ints) == NULL);

	blist_destroy(blist);
	
}

void test_find_node() {
		BList blist = blist_create(NULL);

	// Εισαγωγή τιμών στον πίνακα
	int N = 1000;
	int array[N];

	for (int i = 0; i < N; i++) {
		array[i] = i;
		blist_insert(blist, blist_first(blist), &array[i]);
	}

	BListNode node = blist_first(blist);

	for (int i = N - 1; i >= 0; i--) {
		
		BListNode found_node = blist_find_node(blist, &i, compare_ints); 
		TEST_ASSERT(found_node == node);
		TEST_ASSERT(blist_node_value(blist, found_node) == &array[i]);

		// Προχωράμε στον επόμενο κόμβο για να προσπελάσουμε όλη την λίστα
		node = blist_next(blist, node);
	}

	blist_destroy(blist);
}


// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "list_create", test_create },
	{ "list_insert_next", test_insert },
	{ "list_remove", test_remove },
      { "list_find", test_find },
	{ "list_find_node", test_find_node },
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};