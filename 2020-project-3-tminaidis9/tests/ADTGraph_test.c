//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT Graph.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing

#include "ADTGraph.h"
#include "ADTList.h"


// Προς υλοποίηση

int* create_int(int value) {
	int* p = malloc(sizeof(int));
	*p = value;
	return p;
}

int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

void test_create(void){
	
	Graph graph = graph_create(compare_ints,NULL);
	TEST_ASSERT(graph_size(graph) == 0);
	TEST_ASSERT(graph != NULL);
	List vertices = graph_get_vertices(graph);
	TEST_ASSERT(list_size(vertices) == 0);
	list_destroy(vertices);
	graph_destroy(graph);


}

void test_insert(void){
	
	int N = 10;
	Pointer array[N];
	Graph graph = graph_create(compare_ints,NULL);
	graph_set_hash_function(graph,hash_int);
	
	for(int i = 0 ; i < N ; i++)
		array[i] = create_int(i);

	for(int i = 0 ; i < N ; i++)
		graph_insert_vertex(graph,array[i]);
		
	for(uint i = 1 ; i < N ; i++)
		graph_insert_edge(graph,array[i],array[i-1],i);

	graph_insert_edge(graph,array[0],array[9],15);
	graph_insert_edge(graph,array[1],array[3],2);
	graph_insert_edge(graph,array[3],array[5],2);
	graph_insert_edge(graph,array[7],array[9],2);
	graph_insert_edge(graph,array[5],array[7],6);
	List verticies = graph_get_vertices(graph);
	List lists[N];
	
	for(int i = 0 ; i < N ; i++){
		lists[i] = graph_get_adjacent(graph,array[i]);
	}

	for(int i = 0 ; i < N ; i ++){
		if( i % 2 == 0)
			TEST_ASSERT(list_size(lists[i]) == 2);
		else if (i == 9 || i == 1)
			TEST_ASSERT(list_size(lists[i]) == 3);
		else
			TEST_ASSERT(list_size(lists[i]) == 4);

	}
	TEST_ASSERT(list_size(verticies) == N);
	
	for(int i = 0 ; i < N ; i++){
		list_destroy(lists[i]); 
	}
	
	list_destroy(verticies);
	
	for(int i = 0 ; i < N ; i++){
		free(array[i]); 
	}
	graph_destroy(graph);
	
}

void test_remove(void){
	
	int N = 10;
	Pointer array[N];
	Graph graph = graph_create(compare_ints,NULL);
	graph_set_hash_function(graph,hash_int);

	for(int i = 0 ; i < N ; i++)
		array[i] = create_int(i);

	for(int i = 0 ; i < N ; i++)
		graph_insert_vertex(graph,array[i]);
		
	for(uint i = 1 ; i < N ; i++)
		graph_insert_edge(graph,array[i],array[i-1],i);

	graph_remove_vertex(graph,array[0]);
	List adj = graph_get_adjacent(graph,array[1]);
	TEST_ASSERT(list_size(adj) == 1);

	graph_insert_edge(graph,array[1],array[9],8);
	adj = graph_get_adjacent(graph,array[1]);
	TEST_ASSERT(list_size(adj) == 2);

	graph_remove_edge(graph,array[1],array[2]);
	adj = graph_get_adjacent(graph,array[1]);
	TEST_ASSERT(list_size(adj) == 1);

	graph_remove_edge(graph,array[1],array[9]);
	adj = graph_get_adjacent(graph,array[1]);
	TEST_ASSERT(list_size(adj) == 0);

	graph_destroy(graph);


}

void test_shortest_path(void){ // Δημιουργώ ένα graph σαν αυτό του μαθήματος με αρχή το 1 και target το 5 και πρεπει στο τέλος η list
                               // να έχει 6 κόμβους
	int N = 6;
	Pointer array[N];
	Graph graph = graph_create(compare_ints,NULL);
	graph_set_hash_function(graph,hash_int);

	for(int i = 1 ; i <= N ; i++)
		array[i] = create_int(i);

	for(int i = 1 ; i <= N ; i++)
		graph_insert_vertex(graph,array[i]);
		
	for(uint i = 2 ; i <= N ; i++)
		graph_insert_edge(graph,array[i],array[i-1],i);


	graph_insert_edge(graph,array[6],array[1],128);
	graph_insert_edge(graph,array[1],array[5],15);
	graph_insert_edge(graph,array[2],array[5],18);


	List list = graph_shortest_path(graph,array[1],array[6]);
	TEST_ASSERT(list_size(list) == 6);

	// for(ListNode node = list_first(list) ; node != LIST_EOF ; node = list_next(list,node))
	// 	printf("lnv %d \n", *(int*)list_node_value(list,node));

}




// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	//  { "create", test_create },

	{ "graph_create", test_create },
	{ "graph_insert", test_insert },
	{ "graph_remove", test_remove },
	{ "graph_shortest_path", test_shortest_path },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
}; 