///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Set μέσω Binary Search Tree (BST) ask5 bonous με χρηση αποκλειστικα set
//
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "ADTSet.h"


// Υλοποιούμε τον ADT Set μέσω BST, οπότε το struct set είναι ένα Δυαδικό Δέντρο Αναζήτησης.
struct set {
	SetNode first,last;
      SetNode root;				// η ρίζα, NULL αν είναι κενό δέντρο
	int size;					// μέγεθος, ώστε η set_size να είναι Ο(1)
	CompareFunc compare;		// η διάταξη
	DestroyFunc destroy_value;	// Συνάρτηση που καταστρέφει ένα στοιχείο του set
};

// Ενώ το struct set_node είναι κόμβος ενός Δυαδικού Δέντρου Αναζήτησης
struct set_node {
      SetNode father; //ο πατερας του κομβου
      SetNode prev,next;
	SetNode left, right;		// Παιδιά
	Pointer value;
};


// Παρατηρήσεις για τις node_* συναρτήσεις
// - είναι βοηθητικές (κρυφές από το χρήστη) και υλοποιούν διάφορες λειτουργίες πάνω σε κόμβους του BST.
// - είναι αναδρομικές, η αναδρομή είναι γενικά πολύ βοηθητική στα δέντρα.
// - όσες συναρτήσεις _τροποποιούν_ το δέντρο, ουσιαστικά ενεργούν στο _υποδέντρο_ με ρίζα τον κόμβο node, και επιστρέφουν τη νέα
//   ρίζα του υποδέντρου μετά την τροποποίηση. Η νέα ρίζα χρησιμοποιείται από την προηγούμενη αναδρομική κλήση.
//
// Οι set_* συναρτήσεις (πιο μετά στο αρχείο), υλοποιούν τις συναρτήσεις του ADT Set, και είναι απλές, καλώντας τις αντίστοιχες node_*.


// Δημιουργεί και επιστρέφει έναν κόμβο με τιμή value (χωρίς παιδιά)

static SetNode node_create(Pointer value,SetNode duddy) {
	SetNode node = malloc(sizeof(*node));
	node->left = NULL;
	node->right = NULL;
	node->value = value;
      node->father = duddy;
	return node;
}

// Επιστρέφει τον κόμβο με τιμή ίση με value στο υποδέντρο με ρίζα node, διαφορετικά NULL

static SetNode node_find_equal(SetNode node, CompareFunc compare, Pointer value) {
	// κενό υποδέντρο, δεν υπάρχει η τιμή
	if (node == NULL)
		return NULL;
	
	// Το πού βρίσκεται ο κόμβος που ψάχνουμε εξαρτάται από τη διάταξη της τιμής
	// value σε σχέση με την τιμή του τρέχοντος κόμβο (node->value)
	//
	int compare_res = compare(value, node->value);			// αποθήκευση για να μην καλέσουμε την compare 2 φορές
	if (compare_res == 0)									// value ισοδύναμη της node->value, βρήκαμε τον κόμβο
		return node;
	else if (compare_res < 0)								// value < node->value, ο κόμβος που ψάχνουμε είναι στο αριστερό υποδέντρο
		return node_find_equal(node->left, compare, value);
	else if (compare_res >0)													// value > node->value, ο κόμβος που ψάχνουμε είνια στο δεξιό υποδέντρο
		return node_find_equal(node->right, compare, value);
      else 
            return NULL;
}


// Επιστρέφει τον μικρότερο κόμβο του υποδέντρου με ρίζα node

static SetNode node_find_min(SetNode node) {
	return node != NULL && node->left != NULL
		? node_find_min(node->left)				// Υπάρχει αριστερό υποδέντρο, η μικρότερη τιμή βρίσκεται εκεί
		: node;									// Αλλιώς η μικρότερη τιμή είναι στο ίδιο το node
}

// Επιστρέφει τον μεγαλύτερο κόμβο του υποδέντρου με ρίζα node

static SetNode node_find_max(SetNode node) {
	return node != NULL && node->right != NULL
		? node_find_max(node->right)			// Υπάρχει δεξί υποδέντρο, η μεγαλύτερη τιμή βρίσκεται εκεί
		: node;									// Αλλιώς η μεγαλύτερη τιμή είναι στο ίδιο το node
}

// Επιστρέφει τον προηγούμενο (στη σειρά διάταξης) του κόμβου target στο υποδέντρο με ρίζα node,
// ή NULL αν ο target είναι ο μικρότερος του υποδέντρου. Το υποδέντρο πρέπει να περιέχει τον κόμβο
// target, οπότε δεν μπορεί να είναι κενό.

static SetNode node_find_previous(SetNode node, CompareFunc compare, SetNode target) {
	if (node == target) {
		// Ο target είναι η ρίζα του υποδέντρου, o προηγούμενός του είναι ο μεγαλύτερος του αριστερού υποδέντρου.
		// (Aν δεν υπάρχει αριστερό παιδί, τότε ο κόμβος με τιμή value είναι ο μικρότερος του υποδέντρου, οπότε
		// η node_find_max θα επιστρέψει NULL όπως θέλουμε.)
		return node_find_max(node->left);

	} else if (compare(target->value, node->value) < 0) {
		// Ο target είναι στο αριστερό υποδέντρο, οπότε και ο προηγούμενός του είναι εκεί.
		return node_find_previous(node->left, compare, target);

	} else {
		// Ο target είναι στο δεξί υποδέντρο, ο προηγούμενός του μπορεί να είναι επίσης εκεί, διαφορετικά
		// ο προηγούμενός του είναι ο ίδιος ο node.
		SetNode res = node_find_previous(node->right, compare, target);
		return res != NULL ? res : node;
	}
}

// Επιστρέφει τον επόμενο (στη σειρά διάταξης) του κόμβου target στο υποδέντρο με ρίζα node,
// ή NULL αν ο target είναι ο μεγαλύτερος του υποδέντρου. Το υποδέντρο πρέπει να περιέχει τον κόμβο
// target, οπότε δεν μπορεί να είναι κενό.

static SetNode node_find_next(SetNode node, CompareFunc compare, SetNode target) {
	if (node == target) {
		// Ο target είναι η ρίζα του υποδέντρου, o προηγούμενός του είναι ο μεγαλύτερος του αριστερού υποδέντρου.
		// (Aν δεν υπάρχει αριστερό παιδί, τότε ο κόμβος με τιμή value είναι ο μικρότερος του υποδέντρου, οπότε
		// η node_find_max θα επιστρέψει NULL όπως θέλουμε.)
		return node_find_min(node->right);

	} else if (compare(target->value, node->value) > 0) {
		// Ο target είναι στο αριστερό υποδέντρο, οπότε και ο προηγούμενός του είναι εκεί.
		return node_find_next(node->right, compare, target);

	} else {
		// Ο target είναι στο δεξί υποδέντρο, ο προηγούμενός του μπορεί να είναι επίσης εκεί, διαφορετικά
		// ο προηγούμενός του είναι ο ίδιος ο node.
		SetNode res = node_find_next(node->left, compare, target);
		return res != NULL ? res : node;
	}
}

// Αν υπάρχει κόμβος με τιμή ισοδύναμη της value, αλλάζει την τιμή του σε value, διαφορετικά προσθέτει
// νέο κόμβο με τιμή value. Επιστρέφει τη νέα ρίζα του υποδέντρου, και θέτει το *inserted σε true
// αν έγινε προσθήκη, ή false αν έγινε ενημέρωση.

static SetNode node_insert(Set set,SetNode node, CompareFunc compare, Pointer value, bool* inserted, Pointer* old_value,SetNode duddy) {
	// Αν το υποδέντρο είναι κενό, δημιουργούμε νέο κόμβο ο οποίος γίνεται ρίζα του υποδέντρου
	if (node == NULL) {
		*inserted = true;			// κάναμε προσθήκη
            
		return node_create(value,duddy);
	}

	// Το που θα γίνει η προσθήκη εξαρτάται από τη διάταξη της τιμής
	// value σε σχέση με την τιμή του τρέχοντος κόμβου (node->value)
	//
	int compare_res = compare(value, node->value);
      duddy = node;

	if (compare_res == 0) {
		// βρήκαμε ισοδύναμη τιμή, κάνουμε update
		*inserted = false;
		*old_value = node->value;
		node->value = value;

	} else if (compare_res < 0) {
		// value < node->value, συνεχίζουμε αριστερά.
		node->left = node_insert(set,node->left, compare, value, inserted, old_value,duddy);

	} else {
		// value > node->value, συνεχίζουμε δεξιά
		node->right = node_insert(set,node->right, compare, value, inserted, old_value,duddy);
	}

	return node;	// η ρίζα του υποδέντρου δεν αλλάζει
}

// Αφαιρεί και αποθηκεύει στο min_node τον μικρότερο κόμβο του υποδέντρου με ρίζα node.
// Επιστρέφει τη νέα ρίζα του υποδέντρου.

static SetNode node_remove_min(SetNode node, SetNode* min_node) {
	if (node->left == NULL) {
		// Δεν έχουμε αριστερό υποδέντρο, οπότε ο μικρότερος είναι ο ίδιος ο node
		*min_node = node;
		return node->right;		// νέα ρίζα είναι το δεξιό παιδί

	} else {
		// Εχουμε αριστερό υποδέντρο, οπότε η μικρότερη τιμή είναι εκεί. Συνεχίζουμε αναδρομικά
		// και ενημερώνουμε το node->left με τη νέα ρίζα του υποδέντρου.
		node->left = node_remove_min(node->left, min_node);
		return node;			// η ρίζα δεν μεταβάλλεται
	}
}

// Διαγράφει το κόμβο με τιμή ισοδύναμη της value, αν υπάρχει. Επιστρέφει τη νέα ρίζα του
// υποδέντρου, και θέτει το *removed σε true αν έγινε πραγματικά διαγραφή.

static SetNode node_remove(Set set,SetNode node, CompareFunc compare, Pointer value, bool* removed, Pointer* old_value) {
	if (node == NULL) {
		*removed = false;		// κενό υποδέντρο, δεν υπάρχει η τιμή
		return NULL;
	}


	int compare_res = compare(value, node->value);

	if (compare_res == 0) {
		// Βρέθηκε ισοδύναμη τιμή στον node, οπότε τον διαγράφουμε. Το πώς θα γίνει αυτό εξαρτάται από το αν έχει παιδιά.
		*removed = true;
		*old_value = node->value;

            if(node == set->first){
                  if(node->next != NULL)
                        set->first = node->next;
                  else
                        set->first = NULL;
            } 
            if(node == set->last){
                  if(node->prev != NULL)
                        set->last = node->prev;
                  else
                        set->last = NULL;
            }
            

		if (node->left == NULL) {
			// Δεν υπάρχει αριστερό υποδέντρο, οπότε διαγράφεται απλά ο κόμβος και νέα ρίζα μπαίνει το δεξί παιδί
			SetNode right = node->right;	// αποθήκευση πριν το free!
			
                  if(node->next != NULL)
                        node->next->prev = node->prev;
                  
                  if(node->prev != NULL)
                        node->prev->next = node->next;                  
                  free(node);
			return right;

		} else if (node->right == NULL) {
			// Δεν υπάρχει δεξί υποδέντρο, οπότε διαγράφεται απλά ο κόμβος και νέα ρίζα μπαίνει το αριστερό παιδί
			SetNode left = node->left;		// αποθήκευση πριν το free!                  
                  node->prev->next = node->next;
                  
                  if(node->next != NULL)
                        node->next->prev = left;
			
                  free(node);
			return left;

		} else {
			// Υπάρχουν και τα δύο παιδιά. Αντικαθιστούμε την τιμή του node με την μικρότερη του δεξιού υποδέντρου, η οποία
			// αφαιρείται. Η συνάρτηση node_remove_min κάνει ακριβώς αυτή τη δουλειά.
			SetNode min_right;
			node->right = node_remove_min(node->right, &min_right);
			// Σύνδεση του min_right στη θέση του node
			min_right->left = node->left;
			min_right->right = node->right;
                  min_right->prev = node->prev;
                  if(min_right->next != NULL){
                        min_right->next = min_right->next;
                        min_right->next->prev = min_right;
                  }
                  min_right->prev->next = min_right;
                  
			free(node);
			return min_right;
		}
	}

	// compare_res != 0, συνεχίζουμε στο αριστερό ή δεξί υποδέντρο, η ρίζα δεν αλλάζει.
	if (compare_res < 0)
		node->left  = node_remove(set,node->left,  compare, value, removed, old_value);
	else
		node->right = node_remove(set,node->right, compare, value, removed, old_value);

	return node;
}

// Καταστρέφει όλο το υποδέντρο με ρίζα node

static void node_destroy(SetNode node, DestroyFunc destroy_value) {
	if (node == NULL)
		return;
	
	// πρώτα destroy τα παιδιά, μετά free το node
	node_destroy(node->left, destroy_value);
	node_destroy(node->right, destroy_value);

	if (destroy_value != NULL)
		destroy_value(node->value);

	free(node);
}


//// Συναρτήσεις του ADT Set. Γενικά πολύ απλές, αφού καλούν τις αντίστοιχες node_*

Set set_create(CompareFunc compare, DestroyFunc destroy_value) {
	assert(compare != NULL);	// LCOV_EXCL_LINE

	// δημιουργούμε το stuct
	Set set = malloc(sizeof(*set));
	set->root = NULL;			// κενό δέντρο
	set->size = 0;
	set->compare = compare;
	set->destroy_value = destroy_value;
      set->first = NULL;
      set->last = NULL;

	return set;
}

int set_size(Set set) {
	return set->size;
}

void set_insert(Set set, Pointer value) {
	bool inserted;
	Pointer old_value;
      SetNode duddy = malloc(sizeof(*duddy));
	set->root = node_insert(set,set->root, set->compare, value, &inserted, &old_value,duddy);
      SetNode newnode = set_find_node(set,value);
	
	if(newnode == set->root)
		newnode->father = NULL;
      
	if(set->size == 0){
            set->first = newnode;
            set->last = newnode;
            newnode->next = NULL;
            newnode->prev = NULL;
      }
      else{
            if(newnode == node_find_max(set->root)){
                  set->last = newnode;
                  newnode->next = NULL;
                  newnode->prev = node_find_previous(set->root,set->compare,newnode);
                  newnode->prev->next = newnode;
            }
            if(newnode == node_find_min(set->root)){
                  set->first = newnode;
                  newnode->prev = NULL;
                  newnode->next = node_find_next(set->root,set->compare,newnode);
                  newnode->next->prev = newnode;
            }
            if(newnode != node_find_min(set->root) && newnode != node_find_max(set->root)){
                  newnode->prev = node_find_previous(set->root,set->compare,newnode);
                  newnode->next = node_find_next(set->root,set->compare,newnode);
                  newnode->prev->next = newnode;
                  newnode->next->prev = newnode;
            }
            
      }

	// Το size αλλάζει μόνο αν μπει νέος κόμβος. Στα updates κάνουμε destroy την παλιά τιμή
	if (inserted)
		set->size++;
	else if (set->destroy_value != NULL)
		set->destroy_value(old_value);
}

bool set_remove(Set set, Pointer value) {
	bool removed;
	Pointer old_value = NULL;
	set->root = node_remove(set,set->root, set->compare, value, &removed, &old_value);

	// Το size αλλάζει μόνο αν πραγματικά αφαιρεθεί ένας κόμβος
	if (removed) {
		set->size--;

		if (set->destroy_value != NULL)
			set->destroy_value(old_value);
	}

	return old_value != NULL;
}

Pointer set_find(Set set, Pointer value) {
	SetNode node = node_find_equal(set->root, set->compare, value);
	return node == NULL ? NULL : node->value;
}

DestroyFunc set_set_destroy_value(Set vec, DestroyFunc destroy_value) {
	DestroyFunc old = vec->destroy_value;
	vec->destroy_value = destroy_value;
	return old;
}

void set_destroy(Set set) {
	node_destroy(set->root, set->destroy_value);
	free(set);
}

SetNode set_first(Set set) {
	return set->first;
}

SetNode set_last(Set set) {
	return set->last;
}

SetNode set_previous(Set set, SetNode node) {
	return node->prev;
}

SetNode set_next(Set set, SetNode node) {
	return node->next;
}

Pointer set_node_value(Set set, SetNode node) {
	return node->value;
}


SetNode set_find_node(Set set, Pointer value) {
	return node_find_equal(set->root, set->compare, value);
}

void set_remove_node(Set set,SetNode node){
	//Αν δωθει κομβος που δωθει δεν υπαρχει
	if (node == NULL) {
		return ;
	}

	if(set->first == node){        //φτιαχνω τα first και last στοιχεια απο την αρχή
		if(node->next != NULL){
			set->first = node->next;
		}		
            else{
                  set->first = NULL;
		}
	}
      if(set->last == node){
            if(node->prev != NULL)
                  set->last = node->prev;
            else
                  set->last = NULL;
      }

//παιρνω περιπτωσεις αναλογα με το ποσα παιδια εχει ο κομβος που διαγραφουμε
	if(node->left == NULL && node->right == NULL){              //αν δεν εχει παιδια
		if(node->prev != NULL && node->next != NULL){
			node->prev->next = node->next;                        // απλα διορθωνω prev,next
			node->next->prev = node->prev;
		}
		else if(node->prev == NULL && node->next != NULL){
			node->next->prev = NULL;
			node->next->left = NULL;
		}
		else if(node->next == NULL && node->prev != NULL)
			node->prev->next = NULL;
		else{
			set->first = NULL;
			set->last = NULL;
		} 
		
		free(node);
			
	}
	else if(node->right == NULL){                               //αν εχει μονο αριστερο παιδι
            if(node->father != NULL){                             //και εχει και πατερα
                  if(set->compare(node->father->value,node->value) > 0){ //δες αν ειναι δεξια η αριστερα του πατερα
                        node->father->left = node->left;
                  }
                  else{
                        node->father->right = node->left;
                  }
                  node->left->father = node->father;
            }
            else{                                                  //αν δεν εχει πατερα
			node->prev->next = NULL;
			node->left->father = NULL;
            }

            if(node->prev != NULL)                    
                  node->prev->next = node->next;
            if(node->next != NULL)
                  node->next->prev = node->prev;

		if(node == set->root)                                //αν ο node ειναι ο πρωτος κομβος
			node->next->father = NULL;

		free(node);     
	}
	else if(node->left == NULL){                                //αν εχει μονο δεξι παιδι            		
		if(node->father != NULL){                             //αν εχει πατερα

			if(set->compare(node->father->value,node->value) > 0)
                        node->father->left = node->right;
                  else
                        node->father->right = node->right;
                  
                  node->right->father = node->father;
            }
            else {
			node->next->prev = NULL;
			node->right->father = NULL;
			
            }

		if(node == set->root){
			node->next->father = NULL;
		}

            if(node->prev != NULL)
                  node->prev->next = node->next;
            if(node->next != NULL)
                  node->next->prev = node->prev;
		
		free(node);
            

	}
	else{
		
			SetNode min_right = node->next;
			Pointer value = node->next->value;

			min_right->value = value;
			min_right->left = node->left;
			min_right->right = node->right;
			min_right->prev = node->prev;
			if(node->prev != NULL)
				min_right->prev->next = min_right;

			if(min_right->next != NULL){
				min_right->next = min_right->next;
				min_right->next->prev = min_right;
			}
			min_right->father = NULL;

			free(node);
		
	}

	set->size--;
	if(set->destroy_value != NULL)
		set->destroy_value(node);

	

}

// Συναρτήσεις που δεν υπάρχουν στο public interface αλλά χρησιμοποιούνται στα tests.
// Ελέγχουν ότι το δέντρο είναι ένα σωστό BST.

// LCOV_EXCL_START (δε μας ενδιαφέρει το coverage των test εντολών, και επιπλέον μόνο τα true branches εκτελούνται σε ένα επιτυχημένο test)

static bool node_is_bst(SetNode node, CompareFunc compare) {
	if (node == NULL)
		return true;

	// Ελέγχουμε την ιδιότητα:
	// κάθε κόμβος είναι > αριστερό παιδί, > δεξιότερο κόμβο του αριστερού υποδέντρου, < δεξί παιδί, < αριστερότερο κόμβο του δεξιού υποδέντρου.
	// Είναι ισοδύναμη με την BST ιδιότητα (κάθε κόμβος είναι > αριστερό υποδέντρο και < δεξί υποδέντρο) αλλά ευκολότερο να ελεγθεί.
	bool res = true;
	if(node->left != NULL)
		res = res && compare(node->left->value, node->value) < 0 && compare(node_find_max(node->left)->value, node->value) < 0;
	if(node->right != NULL)
		res = res && compare(node->right->value, node->value) > 0 && compare(node_find_min(node->right)->value, node->value) > 0;

	return res &&
		node_is_bst(node->left, compare) &&
		node_is_bst(node->right, compare);
}

bool set_is_proper(Set node) {
	return node_is_bst(node->root, node->compare);
}

// LCOV_EXCL_STOP