/////////////////////////////////////////////////////
//
//Πολυ απλο test για την set_remove_node
//
//
////////////////////////////////////////////////////////





#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTSet.h"

bool set_is_proper(Set set);

// Επιστρέφει έναν ακέραιο σε νέα μνήμη με τιμή value
int* create_int(int value) {
	int* p = malloc(sizeof(int));
	*p = value;
	return p;
}

// Δημιουργούμε μια ειδική compare συνάρτηση
int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

void shuffle(int* array[], int n) {
	for (int i = 0; i < n; i++) {
		int j = i + rand() / (RAND_MAX / (n - i) + 1);
		int* t = array[j];
		array[j] = array[i];
		array[i] = t;
	}
}


void test_remove_node(){
	Set set = set_create(compare_ints, NULL);
	int N = 10;
	int* value_array[N];
	for (int i = 0; i < N; i++)
		value_array[i] = create_int(i);
	// Ανακατεύουμε το value_array ώστε να υπάρχει ομοιόμορφη εισαγωγή τιμών
	// Πχ εάν εισάγουμε δείκτες με αύξουσα σειρά τιμών, τότε εάν το Set υλοποιείται με BST,
	// οι κόμβοι θα προστίθενται μόνο δεξιά της ρίζας, άρα και η set_remove δεν θα δοκιμάζεται πλήρως
	shuffle(value_array, N);
	for (int i = 0; i < N; i++){
		set_insert(set, value_array[i]);
		
	
	}


	// Διαγράφουμε όλους τους κόμβους
	SetNode node;
	for (int i = 0 ; i < N ; i++) {
		node = set_first(set);
		set_remove_node(set,node);
		TEST_ASSERT(set_size(set) == (N - (i+1)));
		TEST_ASSERT(set_is_proper(set));
	}

}

TEST_LIST = {
      {"set_remove_node",test_remove_node},
	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};