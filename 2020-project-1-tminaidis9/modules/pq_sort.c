////////////////////////////////
//
// Υλοποίηση του pq_sort module
//
////////////////////////////////

#include <stdlib.h>
#include "ADTVector.h"
#include "pq_sort.h"
#include "ADTPriorityQueue.h"
#include <stdio.h>



void pq_sort_vector(Vector vec, CompareFunc compare) {
	// Προσοχή! Μέσα στη συνάρτηση αυτή θα χρειαστεί να αντικαταστήσουμε τα περιεχόμενα του vector. Δε
	// θέλουμε όμως όσο το κάνουμε αυτό το vector να καλέσει τη destroy! Οπότε αλλάζουμε προσωρινά τη
	// συνάρτηση destroy σε NULL (αποθηκεύοντας την παλιά τιμή).
	DestroyFunc old_destroy = vector_set_destroy_value(vec, NULL);
     // int i = -1;
	int j;
	int vsize = vector_size(vec);
            
	for (VectorNode node = vector_first(vec) ; node != VECTOR_EOF ; node = vector_next(vec, node)){
            
		j = vsize-1;
	      for (VectorNode node2 = vector_last(vec) ; node2 != node ; node2 = vector_previous(vec, node2)){
                  
			int* value = vector_node_value(vec,vector_previous(vec,node2));
			int* value2 = vector_node_value(vec,node2);

			if (compare(value , value2) >= 0){
	                  vector_set_at(vec , j-1 , value2);
				vector_set_at(vec , j , value);		      
			} 
			;
			j--;


		}
	}
	// επαναφορά της destroy
	vector_set_destroy_value(vec, old_destroy);
}


void pq_sort_list(List list, CompareFunc compare) {

//Δήλωση μεταβλητών
   ListNode before_l;
	ListNode litt_node;
	Pointer little_value;
	Pointer node2_value;
	Pointer node_value;
	ListNode list_node = list_first(list);
	int l_size = list_size(list);
	int counter2 = 0;

//Εξωτερική επανάληψη η οποία γίνεται ν-1 φορές, όταν ν είναι το μέγεθος της λίστας 
	for(ListNode node = list_first(list); node != list_last(list) ;  node = list_next(list,node)){
		node_value = list_node_value(list, node) ;
		little_value = list_node_value(list, node) ; 
		litt_node = node;

//Εσωτερική επανάληψη, η οποία εντοπίζει τον κόμβο με το μικρότερο value		
		for(ListNode node2 = node ; node2 != LIST_EOF ; node2 = list_next(list,node2)){
			node2_value = list_node_value(list, node2);
			if(compare(little_value,node2_value) >= 0){
				little_value = node2_value ;
				litt_node = node2;
			}
		}

            if( little_value == node_value) continue;
	
//Ψάχνει τον προηγούμενο κόμβο απο τον μικρότερο και αν δεν τον βρει στους υπάρχωντες κόμβους γίνεται LIST_BOF	
            int counter = 0;
		for(ListNode node3 = list_first(list) ; node3 != LIST_EOF ; node3 = list_next(list,node3)){
			if(list_next(list,node3) == litt_node){
			      before_l = node3;
				counter = 1;
				break;}
		}

//Ανταλλαγή value ανάλογα με τον list_node όπου βρισκόμαστε ή LIST_BOF αν θέλουμε να
//ανταλλάξουμε τον πρώτο κόμβο με κάποιον άλλο 		
		if (counter == 0) before_l = LIST_BOF;
            counter2 +=1;
		if(counter2 == 1){
            list_remove_next(list,before_l);
  		list_remove_next(list,LIST_BOF);
  	      list_insert_next(list,LIST_BOF,little_value);
  		list_insert_next(list,before_l,node_value);
		}
		else{
            list_remove_next(list,before_l);
  		list_remove_next(list,list_node);
  	      list_insert_next(list,list_node,little_value);
  		list_insert_next(list,before_l,node_value);
		}
 		 if((counter2 < l_size) && (counter2 > 1))
 		list_node = list_next(list,list_node);
  		

	}
	


}