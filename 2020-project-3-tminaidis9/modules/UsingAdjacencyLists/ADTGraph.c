///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Graph μέσω λιστών γειτνίασης.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <ADTMap.h>
#include <ADTBList.h>
#include <ADTPriorityQueue.h>
#include <ADTList.h>
#include <limits.h>


int* create_ints(int value) {
	int* p = malloc(sizeof(int));
	*p = value;
	return p;
}

struct graph{
      int size;
      Map vertex;
      DestroyFunc destroy_vertex;
      CompareFunc compare;
      HashFunc hash_f;
};

struct adj_and_weight {
      Pointer adj;
      uint weight;
};

typedef struct graph* Graph;

typedef struct adj_and_weight* AdjWeight;

// Δημιουργεί και επιστρέφει ένα γράφο, στον οποίο τα στοιχεία (οι κορυφές)
// συγκρίνονται με βάση τη συνάρτηση compare. Αν destroy_vertex != NULL, τότε
// καλείται destroy_vertex(vertex) κάθε φορά που αφαιρείται μια κορυφή.

Graph graph_create(CompareFunc compare, DestroyFunc destroy_vertex){
      
      Graph graph = malloc(sizeof(*graph));
      graph->vertex = map_create(compare,NULL,NULL);
      graph->compare = compare;
      graph->destroy_vertex = destroy_vertex;
      graph->size = 0;

      return graph;
}

// Επιστρέφει τον αριθμό στοιχείων (κορυφών) που περιέχει ο γράφος graph.

int graph_size(Graph graph){
      return map_size(graph->vertex);
}

// Προσθέτει μια κορυφή στο γράφο.

void graph_insert_vertex(Graph graph, Pointer vertex){
      BList adj_list = blist_create(NULL);
      map_insert(graph->vertex,vertex,adj_list);

}

// Επιστρέφει λίστα με όλες τις κορυφές του γράφου. Η λίστα δημιουργείται σε κάθε
// κληση και είναι ευθύνη του χρήστη να κάνει list_destroy.

List graph_get_vertices(Graph graph){
      List list = list_create(NULL);
      for(MapNode node = map_first(graph->vertex) ; node != MAP_EOF ; node = map_next(graph->vertex,node)){
            list_insert_next(list,LIST_BOF,map_node_key(graph->vertex,node));
      }

      return list;
}

// Διαγράφει μια κορυφή από τον γράφο (αν υπάρχουν ακμές διαγράφονται επίσης).

void graph_remove_vertex(Graph graph, Pointer vertex){
      map_remove(graph->vertex,vertex);
      BList blist;
      AdjWeight adj;
      for(MapNode node = map_first(graph->vertex) ; node != MAP_EOF ; node = map_next(graph->vertex,node)){
            blist = map_node_value(graph->vertex,node);
            for(BListNode node = blist_first(blist) ; node != BLIST_EOF ; node = blist_next(blist,node)){
                  adj = blist_node_value(blist,node);
                  if(graph->compare(adj->adj,vertex) == 0){
                        blist_remove(blist,node);
                        break;
                  }
            }
      }
}

// Προσθέτει μια ακμή με βάρος weight στο γράφο.

void graph_insert_edge(Graph graph, Pointer vertex1, Pointer vertex2, uint weight){
      
      AdjWeight adj = malloc(sizeof(*adj));
      adj->adj = vertex2;
      adj->weight = weight;
      
      AdjWeight adj2 = malloc(sizeof(*adj2));
      adj2->adj = vertex1;
      adj2->weight = weight;


      MapNode our_vertex = map_find_node(graph->vertex,vertex1);
      MapNode sec_vertex = map_find_node(graph->vertex,vertex2);


      BList adj_list = map_node_value(graph->vertex,our_vertex);
      BList adj_list2 = map_node_value(graph->vertex,sec_vertex);

      blist_insert(adj_list,BLIST_EOF,adj);
      blist_insert(adj_list2,BLIST_EOF,adj2);
}

// Επιστρέφει το βάρος της ακμής ανάμεσα στις δύο κορυφές, ή UINT_MAX αν δεν είναι γειτονικές.

uint graph_get_weight(Graph graph, Pointer vertex1, Pointer vertex2){
      MapNode vert1 = map_find_node(graph->vertex,vertex1);
      BList blist = map_node_value(graph->vertex,vert1);
      AdjWeight adj;
      for(BListNode node = blist_first(blist) ; node != BLIST_EOF ; node = blist_next(blist,node)){
            adj = blist_node_value(blist,node);
            if(graph->compare(adj->adj,vertex2) == 0)
                  return adj->weight;
      }

      return UINT_MAX;
}

// Αφαιρεί μια ακμή από το γράφο.

void graph_remove_edge(Graph graph, Pointer vertex1, Pointer vertex2){
      //mpainw kai stwn dyo tis listes geitniasi kai diagrafw ton vertex1 kai 2 antistoixa
      MapNode first_vertex = map_find_node(graph->vertex,vertex1);
      MapNode second_vertex = map_find_node(graph->vertex,vertex2);
      BList first  = map_node_value(graph->vertex,first_vertex);
      BList second  = map_node_value(graph->vertex,second_vertex);
      uint weight = graph_get_weight(graph,vertex1,vertex2);
      
      if(weight == UINT_MAX)
            return;
      
      for(BListNode node = blist_first(first) ; node != BLIST_EOF ; node = blist_next(first,node)){
            AdjWeight adj = blist_node_value(first,node);
            if (adj->adj == vertex2)
                  blist_remove(first,node);
      }

      for(BListNode node = blist_first(first) ; node != BLIST_EOF ; node = blist_next(first,node)){
            AdjWeight adj2 = blist_node_value(second,node);
            if (adj2->adj == vertex1)
                  blist_remove(second,node);
      }

}


// Επιστρέφει λίστα με τους γείτονες μιας κορυφής. Η λίστα δημιουργείται σε κάθε
// κληση και είναι ευθύνη του χρήστη να κάνει list_destroy.

List graph_get_adjacent(Graph graph, Pointer vertex){
      List list = list_create(NULL);
      AdjWeight adj;
      MapNode vert1 = map_find_node(graph->vertex,vertex);
      BList blist = map_node_value(graph->vertex,vert1);
      for(BListNode node = blist_first(blist) ; node != BLIST_EOF ; node = blist_next(blist,node)){
            adj =  blist_node_value(blist,node);
            list_insert_next(list,LIST_EOF,adj->adj);
      }

      return list;
}

// Επιστρέφει (σε λίστα) το συντομότερο μονοπάτι ανάμεσα στις κορυφές source και
// target, ή κενή λίστα αν δεν υπάρχει κανένα μονοπάτι. Η λίστα δημιουργείται σε
// κάθε κληση και είναι ευθύνη του χρήστη να κάνει list_destroy.
struct pq_inserts{
      Pointer adj;
      Pointer counter;
};

typedef struct pq_inserts* PQElement;

int* compare_pq(PQElement u,PQElement v){
     return *(int*)v->counter - *(int*)u->counter;
}


List graph_shortest_path(Graph graph, Pointer source, Pointer target){
      
      List shortest_path = list_create(NULL); // W 
      
      List list = graph_get_vertices(graph);
      int bigger = INT_MIN;
      for(ListNode node = list_first(list) ; node != LIST_EOF ; node = list_next(list,node)){
            if(*(int*)list_node_value(list,node) > bigger)
                  bigger = *(int*)list_node_value(list,node);
      }
      
      List adjacent;
      PriorityQueue pq = pqueue_create(compare_pq,NULL,NULL);
      
      int dist[bigger],prev[bigger];
      for(int i = 1; i <= bigger ; i++){
            dist[i] = -1;
            prev[i] = INT_MIN;
      }
      
      dist[*(int*)source] = 0;
      prev[*(int*)source] = INT_MAX;
      PQElement pqsource = malloc(sizeof(*pqsource));
      pqsource->adj = source;
      pqsource->counter = create_ints(0); 
      pqueue_insert(pq,pqsource);
      Pointer u;

      
      
      while(pqueue_size(pq) > 0){
            
            PQElement pqel = pqueue_max(pq);
            u = pqel->adj;
            pqueue_remove_max(pq);
            if(list_find_node(shortest_path,u,graph->compare) == NULL)
                  list_insert_next(shortest_path,LIST_EOF,u);
            
            if(u == target)
                  break;
            
            adjacent = graph_get_adjacent(graph,u);
            for(ListNode node = list_first(adjacent) ; node != LIST_EOF ; node = list_next(adjacent,node)){
                  Pointer v = list_node_value(adjacent,node);
                  if(list_find_node(shortest_path,v,graph->compare) != NULL)
                        continue;
                  
                  int alt = dist[*(int*)u] + graph_get_weight(graph,u,v);
                  if( dist[*(int*)v] == -1 || alt < dist[*(int*)v] ){
                        dist[*(int*)v] = alt;
                        prev[*(int*)v] = *(int*)u;

                        PQElement vi = malloc(sizeof(*vi));
                        vi->adj = v;
                        vi->counter = create_ints(alt);
                        pqueue_insert(pq,vi);
                        PQElement new = pqueue_max(pq);
                  }
                  
            }
      }
      int a = *(int*)u;
            list_destroy(shortest_path);
            shortest_path = list_create(NULL);
            list_insert_next(shortest_path,LIST_EOF,u);
            while(prev[a] != INT_MAX ){
                  list_insert_next(shortest_path,LIST_EOF,create_ints(prev[a]));
                  a = prev[a];
            }
                  
      return shortest_path;

}

// Ελευθερώνει όλη τη μνήμη που δεσμεύει ο γράφος.
// Οποιαδήποτε λειτουργία πάνω στο γράφο μετά το destroy είναι μη ορισμένη.

void graph_destroy(Graph graph){
      map_destroy(graph->vertex);
      free(graph);
      // thetw thn destroy_func me free sto map kai diagrafw t panta
}



//// Για την περίπτωση που ο γράφος χρησιμοποιεί πίνακα κατακερματισμού

#include "ADTMap.h"	// for HashFunc type

// Ορίζει τη συνάρτηση κατακερματισμού hash_func για το συγκεκριμένο γράφο.
// Πρέπει να κληθεί μετά την graph_create και πριν από οποιαδήποτε άλλη συνάρτηση.

void graph_set_hash_function(Graph graph, HashFunc hash_func){
      graph->hash_f = hash_func;
	map_set_hash_function(graph->vertex, hash_func);

}
