///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Priority Queue μέσω ταξινομημένης λίστας.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "ADTPriorityQueue.h"
#include "ADTList.h"			// Η υλοποίηση του PriorityQueue χρησιμοποιεί List


// Ενα PriorityQueue είναι pointer σε αυτό το struct
struct priority_queue {
	List list;		// η λίστα στην οποία αποθηκεύουμε τα στοιχεία
	CompareFunc compare;
	DestroyFunc destroy_value;
};


// Συναρτήσεις του ADTPriorityQueue //////////////////////////////////////////////////

PriorityQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value, Vector values) {
	 assert(compare!=NULL);

	PriorityQueue pqueue = malloc(sizeof(*pqueue));
	pqueue->compare = compare;
	pqueue->destroy_value = destroy_value;
	pqueue->list = list_create(NULL);
	if(values != NULL){
		list_insert_next(pqueue->list,LIST_BOF,vector_get_at(values,0));
		  int size = vector_size(values);
		  for(int i = 1 ; i < size; i++){ //Προσθήκη στοιχείων απο το vector ένα ένα 
			  int done = 0; //Εάν έγινε η εισαγωγή του στοιχείου την λίστα
			  ListNode bef_node = list_first(pqueue->list); //Προηγούμενος κόμβος για να γίνει το list_insert_next
			  for(ListNode node = list_first(pqueue->list) ; node != LIST_EOF ; node = list_next(pqueue->list,node)){  //διατήρηση ταξινόμησης της λίστας
				  Pointer lnv = list_node_value(pqueue->list,node);
				  if(pqueue->compare(vector_get_at(values,i),lnv) > 0){ 
					  done = 1;
					  if(node == list_first(pqueue->list)) { list_insert_next(pqueue->list,LIST_BOF,vector_get_at(values,i)); } //Εάν πρέπει να εισχθεί ως πρώτο στοιχείο της λίστας
					  else { list_insert_next(pqueue->list,bef_node,vector_get_at(values,i)); }
					  break; 
				  }
				  bef_node = node;

				  if(done  == 0) 
			              list_insert_next(pqueue->list,list_last(pqueue->list),vector_get_at(values,i)); //Εάν είναι το τελευταίο στοιχείο που πρέπει να εισαχθεί
					                                                                              //δεν θα μπει στην απο πάνω επανάληψη αλλά θα εισαχθεί απο εδώ
			  }
		  }
	}
	return pqueue;
}

int pqueue_size(PriorityQueue pqueue) {
	return list_size(pqueue->list);
}

Pointer pqueue_max(PriorityQueue pqueue) {
	return list_node_value(pqueue->list,list_first(pqueue->list));
}

 void pqueue_insert(PriorityQueue pqueue, Pointer value) { //Αναζητεί την κατάλληλη θέση για να εισάγει το νέο στοιχείο
	 if(list_size(pqueue->list) == 0){ list_insert_next(pqueue->list,LIST_BOF,value);}
	 else {
		ListNode bef_node = list_first(pqueue->list); //Προηγούμενος κόμβος για να γίνει το list_insert_next
		for(ListNode node = list_first(pqueue->list) ; node != LIST_EOF ; node = list_next(pqueue->list,node)){
			Pointer lnv = list_node_value(pqueue->list,node);
					if(pqueue->compare(value,lnv) > 0){
						if(node == list_first(pqueue->list)) { list_insert_next(pqueue->list,LIST_BOF,value); }
						else { list_insert_next(pqueue->list,bef_node,value); }
						return; 
					}
					bef_node = node;
		}
		list_insert_next(pqueue->list,list_last(pqueue->list),value);
	 }
 }

void pqueue_remove_max(PriorityQueue pqueue) {

	if (pqueue->destroy_value != NULL)
		pqueue->destroy_value(pqueue_max(pqueue));
		
	list_remove_next(pqueue->list,LIST_BOF);
}		

DestroyFunc pqueue_set_destroy_value(PriorityQueue pqueue, DestroyFunc destroy_value) {
	// Απλά καλούμε την αντίστοιχη συνάρτηση της λίστας
	return list_set_destroy_value(pqueue->list, destroy_value);
}

void pqueue_destroy(PriorityQueue pqueue) {
	list_destroy(pqueue->list);
	free(pqueue);

}
