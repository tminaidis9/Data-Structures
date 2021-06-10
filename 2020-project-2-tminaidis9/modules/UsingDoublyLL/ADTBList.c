///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT BList μέσω διπλά συνδεδεμένης λίστας.
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "ADTBList.h"


// Ενα BList είναι pointer σε αυτό το struct
struct blist {
	BListNode dummy;
	BListNode last;
	int size;
	DestroyFunc destroy_value;
};

struct blist_node {
	BListNode previous;
	BListNode next;
	Pointer value; 
};


BList blist_create(DestroyFunc destroy_value) {
	BList blist = malloc(sizeof(*blist));             //Δέσμευση χώρου για το blist και δημιουργία dummy κόμβου
	blist->size = 0;
	blist->destroy_value = destroy_value;
      blist->dummy = malloc(sizeof(*blist->dummy));
	blist->dummy->next = NULL;
	blist->dummy->value = NULL;
	blist->last = blist->dummy;
	return blist;
}

int blist_size(BList blist) {
	return blist->size;
}

void blist_insert(BList blist, BListNode node, Pointer value) {

	BListNode new = malloc(sizeof(*new));
	new->value = value;

	if(blist->size == 0){                 //Εάν δεν έχει στιοχεία μέσα το blist, τοποθετούμε σωστά το πρώτο στοιχείο
		blist->dummy->next = new;
		blist->last = new;
		new->previous = blist->dummy;
		new->next = NULL;

	}
	else if(node == BLIST_EOF || node == BLIST_BOF || node == NULL){        //Εάν μας δίνουν έναν κόμβο null σημαίνει πως εισάγουμε το στοιχείο στο τέλος του blist
		new->previous = blist->last;
		blist->last->next = new;
		blist->last = new;
		new->next = NULL;
	}
	else{                                   //διαφορετικά αν προσθέτουμε στοιχεία ενδιάμεσα στο blist:
		new->next = node;
		new->previous = node->previous;
		BListNode np = node->previous;
		np->next = new;
		node->previous = new;
		if(blist_first(blist) == node) 
		      blist->dummy->next = new;
	}
	blist->size++;	//Αυξάνουμε το size


      
}

void blist_remove(BList blist, BListNode node) {
      if(node == NULL){
		return;
	}
	
      BListNode previous_node = NULL;
      BListNode next_node = NULL;

	if(node != blist_first(blist))         //Εάν ο κόμβος ειναι ο πρώτος δεν έχει προηγούμενο και αν είναι ο τεέυταίος δεν έχει επόμενο
		previous_node = node->previous;
      if(node != blist_last(blist))
		next_node = node->next;


	if(node == blist_first(blist)){        //εάν το στοιχείο πουαφαιρείται είναι το πρώτο
		blist->dummy->next = node->next;

		if(node != blist_last(blist))
		      node->next->previous = blist->dummy;
	}
	else{
	      
		if(node != blist_last(blist)){
			next_node->previous = previous_node;
			previous_node->next = next_node;
		}
		else{
			previous_node->next = NULL;
		}
	}
	
	blist->size--;
      if(blist->size >= 1){
		if(blist->dummy->next == node)      //κρατάμε ενημερωμένο το πρώτο στοιχείο
			blist->dummy->next = next_node;

		if(blist->last == node)        //φροντίζουμε να κρατάμε ενημερωμένο το last
		blist->last = previous_node;
	}

	if(blist->destroy_value != NULL)
	     blist->destroy_value(node->value);

	free(node);          //διαγραφή του στοιχείου

	
}

Pointer blist_find(BList blist, Pointer value, CompareFunc compare) {
	BListNode node = blist_find_node(blist, value, compare);
	return node == NULL ? NULL : node->value;
}

DestroyFunc blist_set_destroy_value(BList blist, DestroyFunc destroy_value) {
	DestroyFunc old = blist->destroy_value;
	blist->destroy_value = destroy_value;
	return old;
}

void blist_destroy(BList blist) {
	
	BListNode node = blist->dummy;
	while(node != NULL){        //διαγράφουμε έναν έναν κάθε κόμβο και στο τέλος κάνουμε free το στρακτ
		BListNode next = node->next;

		if (node != blist->dummy && blist->destroy_value != NULL)
			blist->destroy_value(node->value);

		free(node);
		node = next;
	}

	free(blist);

}


// Διάσχιση της λίστας /////////////////////////////////////////////

BListNode blist_first(BList blist) {
	return blist->dummy->next;
}

BListNode blist_last(BList blist) {
	return blist->last;
}

BListNode blist_next(BList blist, BListNode node) {
	assert(node != NULL);
	
	return node->next;
}

BListNode blist_previous(BList blist, BListNode node) {
	assert(node != NULL);

	return node->previous;
}

Pointer blist_node_value(BList blist, BListNode node) {
	assert(node != NULL);

	return node->value;
}

BListNode blist_find_node(BList blist, Pointer value, CompareFunc compare) {

	for (BListNode node = blist->dummy->next; node != NULL; node = node->next)      //ελέγχω ένα ένα κάθε στοιχείο
		if (compare(value, node->value) == 0)
			return node;	

	return NULL;
}