///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Priority Queue μέσω σωρού.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "ADTPriorityQueue.h"
#include "ADTVector.h"			// Η υλοποίηση του PriorityQueue χρησιμοποιεί Vector

// Ενα PriorityQueue είναι pointer σε αυτό το struct
struct priority_queue {
	Vector vector;				// Τα δεδομένα, σε Vector ώστε να έχουμε μεταβλητό μέγεθος χωρίς κόπο
	CompareFunc compare;		// Η διάταξη
	DestroyFunc destroy_value;	// Συνάρτηση που καταστρέφει ένα στοιχείο του vector.
      PriorityQueueNode dummy;
	int steps;
};


struct priority_queue_node {         //τα node της pqueue
	Pointer value;
	int pos;
};

// Βοηθητικές συναρτήσεις ////////////////////////////////////////////////////////////////////////////

// Προσοχή: στην αναπαράσταση ενός complete binary tree με πίνακα, είναι βολικό τα ids των κόμβων να
// ξεκινάνε από το 1 (ρίζα), το οποίο απλοποιεί τις φόρμουλες για εύρεση πατέρα/παιδιών. Οι θέσεις
// ενός vector όμως ξεκινάνε από το 0. Θα μπορούσαμε απλά να αφήσουμε μία θέση κενή, αλλά δεν είναι ανάγκη,
// μπορούμε απλά να αφαιρούμε 1 όταν διαβάζουμε/γράφουμε στο vector. Για απλοποίηση του κώδικα, η
// πρόσβαση στα στοιχεία του vector γίνεται από τις παρακάτω 2 βοηθητικές συναρτήσεις.

// Επιστρέφει την τιμή του κόμβου node_id

static Pointer node_value(PriorityQueue pqueue, int node_id) {
	// τα node_ids είναι 1-based, το node_id αποθηκεύεται στη θέση node_id - 1
	return vector_get_at(pqueue->vector, node_id - 1);
}

// Ανταλλάσει τις τιμές των κόμβων node_id1 και node_id2

static void node_swap(PriorityQueue pqueue, int node_id1, int node_id2) {
	// τα node_ids είναι 1-based, το node_id αποθηκεύεται στη θέση node_id - 1
	PriorityQueueNode value1 = node_value(pqueue, node_id1); //πλεον εισαγουμε κομβους κα οχι τιμες
	PriorityQueueNode value2 = node_value(pqueue, node_id2);

	vector_set_at(pqueue->vector, node_id1 - 1, value2);
	vector_set_at(pqueue->vector, node_id2 - 1, value1);
      
	int pos2 = value1->pos;
      value1->pos = value2->pos;
	value2->pos = pos2;

}

// Αποκαθιστά την ιδιότητα του σωρού.
// Πριν: όλοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού, εκτός από
//       τον node_id που μπορεί να είναι _μεγαλύτερος_ από τον πατέρα του.
// Μετά: όλοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού.

static void bubble_up(PriorityQueue pqueue, int node_id) {
	// Αν φτάσαμε στη ρίζα, σταματάμε
      pqueue->steps += 1;
	
	if (node_id == 1)
		return;
	int parent = node_id / 2;		// Ο πατέρας του κόμβου. Τα node_ids είναι 1-based

      PriorityQueueNode node_i = node_value(pqueue, node_id); //πλεον ολα τα στοιχεια ειναι κομβοι και οχι τιμες
	PriorityQueueNode parent_node = node_value(pqueue, parent);

	// Αν ο πατέρας έχει μικρότερη τιμή από τον κόμβο, swap και συνεχίζουμε αναδρομικά προς τα πάνω
	if (pqueue->compare(parent_node->value, node_i->value) < 0) {
		node_swap(pqueue, parent, node_id);

		bubble_up(pqueue, parent);
	}
}

// Αποκαθιστά την ιδιότητα του σωρού.
// Πριν: όλοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού, εκτός από τον
//       node_id που μπορεί να είναι _μικρότερος_ από κάποιο από τα παιδιά του.
// Μετά: όλοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού.

static void bubble_down(PriorityQueue pqueue, int node_id) {
	// βρίσκουμε τα παιδιά του κόμβου (αν δεν υπάρχουν σταματάμε)
	
	pqueue->steps += 1;
	
	int left_child = 2 * node_id;
	int right_child = left_child + 1;


	int size = pqueue_size(pqueue);
	if (left_child > size)      //δεν υπαρχουν παιδια
		return;
      PriorityQueueNode right_node; //ολα τα παιδια ειναι κομβοι
	PriorityQueueNode left_node = node_value(pqueue, left_child);

	if(right_child <= size)
		right_node = node_value(pqueue, right_child);
	
	PriorityQueueNode node_i = node_value(pqueue,node_id); //ο κομβος που ειναι στην θεση node_id

	// βρίσκουμε το μέγιστο από τα 2 παιδιά
	PriorityQueueNode max_child = left_node;
	int max = left_child;
	if (right_child <= size && pqueue->compare(left_node->value, right_node->value) < 0){
			max_child = right_node; //ο κομβος
			max = right_child; //και το int που βρισκεται ο κομβος
	}

	// Αν ο κόμβος είναι μικρότερος από το μέγιστο παιδί, swap και συνεχίζουμε προς τα κάτω
	if (pqueue->compare(node_i->value, max_child->value) < 0) {
		node_swap(pqueue, node_id, max);

		bubble_down(pqueue, max);
	}
}

// Αρχικοποιεί το σωρό από τα στοιχεία του vector values.

static void naive_heapify(PriorityQueue pqueue, Vector values) {
	// Απλά κάνουμε insert τα στοιχεία ένα ένα.
	// TODO: υπάρχει πιο αποδοτικός τρόπος να γίνει αυτό!
	int size = vector_size(values);
	for (int i = 0; i < size; i++){
		pqueue_insert(pqueue, vector_get_at(values, i));
	}
}


// Συναρτήσεις του ADTPriorityQueue //////////////////////////////////////////////////

PriorityQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value, Vector values) {
	assert(compare != NULL);	// LCOV_EXCL_LINE

	PriorityQueue pqueue = malloc(sizeof(*pqueue));
	pqueue->compare = compare;
	pqueue->destroy_value = destroy_value;

	// Δημιουργία του vector που αποθηκεύει τα στοιχεία.
	// ΠΡΟΣΟΧΗ: ΔΕΝ περνάμε την destroy_value στο vector!
	// Αν την περάσουμε θα καλείται όταν κάνουμε swap 2 στοιχεία, το οποίο δεν το επιθυμούμε.
	pqueue->vector = vector_create(0, NULL);
      pqueue->steps = 1;
	// Αν values != NULL, αρχικοποιούμε το σωρό.
	if (values != NULL){
		naive_heapify(pqueue, values);
		pqueue->steps -= 1;    // Λόγω του pqueue->steps = 1 απο πρίν
	}
	return pqueue;
}

int pqueue_size(PriorityQueue pqueue) {
	pqueue->steps = 1;
	return vector_size(pqueue->vector);
}

int pqueue_steps(PriorityQueue pqueue){
	return pqueue->steps;
}

Pointer pqueue_max(PriorityQueue pqueue) {
	PriorityQueueNode node = node_value(pqueue, 1); //επιστρεφει τον πρωτο κομβο
	pqueue->steps = 1;
	return node->value;		// root
}

PriorityQueueNode pqueue_insert(PriorityQueue pqueue, Pointer value) {
	pqueue->steps = 0;
	PriorityQueueNode node = malloc(sizeof(*node));
	node->value = value;
	node->pos = vector_size(pqueue->vector) + 1;
	// Προσθέτουμε την τιμή στο τέλος το σωρού
	vector_insert_last(pqueue->vector, node);

 	// Ολοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού εκτός από τον τελευταίο, που μπορεί να είναι
	// μεγαλύτερος από τον πατέρα του. Αρα μπορούμε να επαναφέρουμε την ιδιότητα του σωρού καλώντας
	// τη bubble_up γα τον τελευταίο κόμβο (του οποίου το 1-based id ισούται με το νέο μέγεθος του σωρού).
	bubble_up(pqueue, pqueue_size(pqueue));

	// TODO: υλοποίηση κόμβων και επιστροφή.

	return node;
}

void pqueue_remove_max(PriorityQueue pqueue) {
	int last_node = pqueue_size(pqueue);
	assert(last_node != 0);		// LCOV_EXCL_LINE

	// Destroy την τιμή που αφαιρείται
	if (pqueue->destroy_value != NULL)
		pqueue->destroy_value(pqueue_max(pqueue));
	// Αντικαθιστούμε τον πρώτο κόμβο με τον τελευταίο και αφαιρούμε τον τελευταίο
	node_swap(pqueue, 1, last_node);
	pqueue->steps = 0;
	vector_remove_last(pqueue->vector);
     

 	// Ολοι οι κόμβοι ικανοποιούν την ιδιότητα του σωρού εκτός από τη νέα ρίζα
 	// που μπορεί να είναι μικρότερη από κάποιο παιδί της. Αρα μπορούμε να
 	// επαναφέρουμε την ιδιότητα του σωρού καλώντας τη bubble_down για τη ρίζα.
	bubble_down(pqueue, 1);
}

DestroyFunc pqueue_set_destroy_value(PriorityQueue pqueue, DestroyFunc destroy_value) {
	DestroyFunc old = pqueue->destroy_value;
	pqueue->destroy_value = destroy_value;
	pqueue->steps = 1;
	return old;
}

void pqueue_destroy(PriorityQueue pqueue) {
	// Αντί να κάνουμε εμείς destroy τα στοιχεία, είναι απλούστερο να
	// προσθέσουμε τη destroy_value στο vector ώστε να κληθεί κατά το vector_destroy.
	pqueue->steps = pqueue_size(pqueue);
	vector_set_destroy_value(pqueue->vector, pqueue->destroy_value);
	vector_destroy(pqueue->vector);


	free(pqueue);
}



//// Νέες συναρτήσεις για την εργασία 2 //////////////////////////////////////////

Pointer pqueue_node_value(PriorityQueue set, PriorityQueueNode node) {
	return node->value;
}

void pqueue_remove_node(PriorityQueue pqueue, PriorityQueueNode node) {
	
	pqueue->steps = 1; //το βαζουμε εδω για να καλυψουμε την περιπτωση που δωθει null για κομβος και στην συνεχεια αφαιρουμε 1
	                   //για να υπολογιστουν τα βηματα μεσω της bubble up και bubbe down 

	assert(node != NULL);
	int last_node = pqueue_size(pqueue);
	assert(last_node != 0);
	int our_node = node->pos;

	// if (pqueue->destroy_value != NULL)
	// 	pqueue->destroy_value(pqueue_max(pqueue));

	node_swap(pqueue, our_node, last_node);

	
	vector_remove_last(pqueue->vector);
     
      pqueue->steps -= 1;      //Επεξήγηση πάνω
	bubble_up(pqueue, our_node); //κανει και bubble up και bubble down για να φτιαχτει σωστα η pqueue
	bubble_down(pqueue,our_node);

}



void pqueue_update_order(PriorityQueue pqueue, PriorityQueueNode node) {
	int pos = node->pos;
	pqueue->steps = 0;
	bubble_up(pqueue,pos);//κανει και bubble up και bubble down για να φτιαχτει σωστα η pqueue
	bubble_down(pqueue,pos);


}
