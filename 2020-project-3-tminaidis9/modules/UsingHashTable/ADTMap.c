/////////////////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Map μέσω Hash Table με seperate chaining 
//
// Στην υλοποίηση μου υπάρχουν τα mapnodes τα οποία είναι ίσα με map->capacity και περιεχονται στο map->array
// και κάθε ένα από αυτά περιέχει μια blist .Στην blist περιέχονται μέσα mapnodes τα οποία έχουν τα key και value
// που πρέπει να έχei τo στοιχείo υτό του map.
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>

#include "ADTMap.h"
#include "ADTBList.h"


// Το μέγεθος του Hash Table ιδανικά θέλουμε να είναι πρώτος αριθμός σύμφωνα με την θεωρία.
// Η παρακάτω λίστα περιέχει πρώτους οι οποίοι έχουν αποδεδιγμένα καλή συμπεριφορά ως μεγέθη.
// Κάθε re-hash θα γίνεται βάσει αυτής της λίστας. Αν χρειάζονται παραπάνω απο 1610612741 στοχεία, τότε σε καθε rehash διπλασιάζουμε το μέγεθος.
int prime_sizes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
	786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

// Χρησιμοποιούμε open addressing, οπότε σύμφωνα με την θεωρία, πρέπει πάντα να διατηρούμε
// τον load factor του  hash table μικρότερο ή ίσο του 0.5, για να έχουμε αποδoτικές πράξεις
#define MAX_LOAD_FACTOR 0.9

// Δομή του κάθε κόμβου που έχει το hash table (με το οποίο υλοιποιούμε το map)
struct map_node{
	Pointer key;		// Το κλειδί που χρησιμοποιείται για να hash-αρουμε
	Pointer value;  	// Η τιμή που αντισοιχίζεται στο παραπάνω κλειδί
	BList blist_key_value; // Δημιουργουμε ενα blist για καθε map_node το οποιο θα αποτελειται και αυτο απο στοιχεια mapnode
	};

// Δομή του Map (περιέχει όλες τις πληροφορίες που χρεαζόμαστε για το HashTable)
struct map {
	MapNode array;				// Ο πίνακας που θα χρησιμοποιήσουμε για το map (remember, φτιάχνουμε ένα hash table)
	int capacity;				// Πόσο χώρο έχουμε δεσμεύσει.
	int size;					// Πόσα στοιχεία έχουμε προσθέσει
	CompareFunc compare;		// Συνάρτηση για σύγκρηση δεικτών, που πρέπει να δίνεται απο τον χρήστη
	HashFunc hash_function;		// Συνάρτηση για να παίρνουμε το hash code του κάθε αντικειμένου.
	DestroyFunc destroy_key;	// Συναρτήσεις που καλούνται όταν διαγράφουμε έναν κόμβο απο το map.
	DestroyFunc destroy_value;
};

static MapNode new_mapnode(Pointer key,Pointer value){     //Συναρτηση για δημιουργια ενος νεου mapnode που περιεχει ενα key και ενα value
		MapNode node = malloc(sizeof(*node));
		node->key = key;
		node->value = value;
		return node;
}


Map map_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
	// Δεσμεύουμε κατάλληλα τον χώρο που χρειαζόμαστε για το hash table
	Map map = malloc(sizeof(*map));
	map->capacity = prime_sizes[0];
	map->array = malloc(map->capacity * sizeof(struct map_node));
	
	for(int i = 0 ; i < map->capacity ; i++){
		map->array[i].blist_key_value = blist_create(destroy_key);  //Δημιουργούμε ένα blist για κάθε ένα mapnode στο map->array
	}

	map->size = 0;
	map->compare = compare;
	map->destroy_key = destroy_key;
	map->destroy_value = destroy_value;

	return map;
}

// Επιστρέφει τον αριθμό των entries του map σε μία χρονική στιγμή.
int map_size(Map map) {
	return map->size;
}

// Συνάρτηση για την επέκταση του Hash Table σε περίπτωση που ο load factor μεγαλώσει πολύ.
static void rehash(Map map) {
	// Αποθήκευση των παλιών δεδομένων
	int old_capacity = map->capacity;
	MapNode old_array = map->array;

	// Βρίσκουμε τη νέα χωρητικότητα, διασχίζοντας τη λίστα των πρώτων ώστε να βρούμε τον επόμενο. 
	int prime_no = sizeof(prime_sizes) / sizeof(int);	// το μέγεθος του πίνακα
	for (int i = 0; i < prime_no; i++) {					// LCOV_EXCL_LINE
		if (prime_sizes[i] > old_capacity) {
			map->capacity = prime_sizes[i]; 
			break;
		}
	}

	// Αν έχουμε εξαντλήσει όλους τους πρώτους, διπλασιάζουμε
	if (map->capacity == old_capacity)					// LCOV_EXCL_LINE
		map->capacity *= 2;								// LCOV_EXCL_LINE

	// Δημιουργούμε ένα μεγαλύτερο hash table
	map->array = malloc(map->capacity * sizeof(struct map_node));
	

	//Δημιουργουμε μια λιστα και στις καινουργιες θέσεις του πίνακα array
	for (int i = 0; i < map->capacity; i++){
		map->array[i].blist_key_value = blist_create(map->destroy_key);
	}

	// Τοποθετούμε ΜΟΝΟ τα entries που όντως περιέχουν ένα στοιχείο (το rehash είναι και μία ευκαιρία να ξεφορτωθούμε τα deleted nodes)
	map->size = 0;
	MapNode mapnode;
	Pointer key,value;
	for (int i = 0; i < old_capacity; i++){
		for(BListNode bnode = blist_first(old_array[i].blist_key_value) ; bnode != BLIST_EOF ; bnode = blist_next(old_array[i].blist_key_value,bnode)){
			mapnode = blist_node_value(old_array[i].blist_key_value,bnode);
			key = mapnode->key;
			value = mapnode->value;
			map_insert(map,key,value); // Aντιστοιχίζουμε στην σωστη θέση του νέου hash table τα στοιχεία του παλιού
		}
	}

	//Αποδεσμεύουμε τον παλιό πίνακα ώστε να μήν έχουμε leaks
	free(old_array);
}

// Εισαγωγή στο hash table του ζευγαριού (key, item). Αν το key υπάρχει,
// ανανέωση του με ένα νέο value, και η συνάρτηση επιστρέφει true.

void map_insert(Map map, Pointer key, Pointer value) {
	// Σκανάρουμε το Hash Table μέχρι να βρούμε διαθέσιμη θέση για να τοποθετήσουμε το ζευγάρι,
	// ή μέχρι να βρούμε το κλειδί ώστε να το αντικαταστήσουμε.
	MapNode mapnode = malloc(sizeof(*mapnode)); 
	bool already_in_map = false;
	uint pos = map->hash_function(key) % map->capacity; // το pos περιέχει μόνο το position που θέλουμε εφόσον αν το στοιχείο υπάρχει θα βρίσκεται αναγκαστικά στο blist του map->array[pos]
	BListNode insert_node = BLIST_EOF; //Αρχικοποιούμε με EOF τον insert_node ο οποίος θα μας δείξει αν το κλειδί του κόμβου που θελουμε να βάλουμε υπάρχει ηδη
	MapNode keyvalueforinsert = new_mapnode(key,value); //Το νέο mapnode που θέλουμε να εισάγουμε 
	for(BListNode node = blist_first(map->array[pos].blist_key_value) ; node != BLIST_EOF ; node = blist_next(map->array[pos].blist_key_value,node)){
		if(blist_size(map->array[pos].blist_key_value) >= 1){ //Αν το blist έχει στοιχεία μέσα του
			mapnode = blist_node_value(map->array[pos].blist_key_value,node); //Ψάξε να δεις άμα υπάρχει κάπου το κλειδί που θέλουμε να εισαγουμε
			if(map->compare(mapnode->key,key) == 0) // Αν υπάρχει
				insert_node = node;
				break;
		}
	}
	
	if (insert_node != BLIST_EOF) {
		already_in_map = true;
		}

	if(already_in_map){	//Aν υπάρχει βάλε κατ ευθείαν το νέο key και value στο blist και διεγραψε το παλιό key με το δικο του value
		blist_insert(map->array[pos].blist_key_value,insert_node,keyvalueforinsert);
		blist_remove(map->array[pos].blist_key_value,insert_node);
	}
	else{ //αλλιώς απλά βαλε το στο blist και αυξησε το μέγεθος
		// Νέο στοιχείο, αυξάνουμε τα συνολικά στοιχεία του map
		map->size++;
		// Προσθήκη τιμών στον κόμβο κα στο καταλληλο list
		if(blist_size(map->array[pos].blist_key_value) == 0){
			blist_insert(map->array[pos].blist_key_value,BLIST_EOF,keyvalueforinsert);
		}
		else{
			for(BListNode node = blist_first(map->array[pos].blist_key_value) ; node != BLIST_EOF ; node = blist_next(map->array[pos].blist_key_value,node)){
				MapNode blistmapnode = blist_node_value(map->array[pos].blist_key_value,node);
				if(map->compare(key,blistmapnode->key) >= 0){
					blist_insert(map->array[pos].blist_key_value,node,keyvalueforinsert);
					break;
				}
				if(node == blist_last(map->array[pos].blist_key_value)){
					blist_insert(map->array[pos].blist_key_value,BLIST_EOF,keyvalueforinsert);
					break;
				}
			}
		}
	}

	// Αν με την νέα εισαγωγή ξεπερνάμε το μέγιστο load factor, πρέπει να κάνουμε rehash
	float load_factor = (float)map->size / map->capacity;
	if (load_factor > MAX_LOAD_FACTOR)
		rehash(map);
}

// Διαργραφή απο το Hash Table του κλειδιού με τιμή key
bool map_remove(Map map, Pointer key) {	
	MapNode mapnode = malloc(sizeof(*mapnode));
	BListNode remove_node = BLIST_EOF; //Ο κομβος πορς διαγραφή
	uint pos = map->hash_function(key) % map->capacity; //το position

	for(BListNode node = blist_first(map->array[pos].blist_key_value);  
	node != BLIST_EOF;                                                                                              
	node = blist_next(map->array[pos].blist_key_value,node)){	
		mapnode = blist_node_value(map->array[pos].blist_key_value,node); //Το mapnode που βρίσκεται μέσα σε κάθε κόμβο του blist
		if(map->compare(mapnode->key,key) == 0)
			remove_node = node;
		

	}
	
	if(remove_node == BLIST_EOF){
		return false;
	}
	

	// destroy
	blist_remove(map->array[pos].blist_key_value,remove_node);

	// θέτουμε ως "deleted", ώστε να μην διακόπτεται η αναζήτηση, αλλά ταυτόχρονα να γίνεται ομαλά η εισαγωγή
	map->size--;

	return true;
}

// Αναζήτηση στο map, με σκοπό να επιστραφεί το value του κλειδιού που περνάμε σαν όρισμα.

Pointer map_find(Map map, Pointer key) {
	int count = 0;
	MapNode mapnode;
	uint pos = map->hash_function(key) % map->capacity;
	for(BListNode node = blist_first(map->array[pos].blist_key_value);  
	node != BLIST_EOF;                                                                                               
	node = blist_next(map->array[pos].blist_key_value,node)){	 //sto blist του mapnode στην θεση pos ψαξε το mapnode με mapnode->key == key
		mapnode = blist_node_value(map->array[pos].blist_key_value,node);
		if(map->compare(mapnode->key,key) == 0){
			count = 1;
			break;
		}	
	}
	if (count == 1)
		return mapnode->value;
	else
		return NULL;
}


DestroyFunc map_set_destroy_key(Map map, DestroyFunc destroy_key) {
	DestroyFunc old = map->destroy_key;
	map->destroy_key = destroy_key;
	return old;
}

DestroyFunc map_set_destroy_value(Map map, DestroyFunc destroy_value) {
	DestroyFunc old = map->destroy_value;
	map->destroy_value = destroy_value;
	return old;
}

// Απελευθέρωση μνήμης που δεσμεύει το map
void map_destroy(Map map) {
	// for (int i = 0; i < map->capacity; i++) {
	// 	blist_destroy(map->array[i].blist_key_value);
	// }

	free(map->array);
	free(map);
}

/////////////////////// Διάσχιση του map μέσω κόμβων ///////////////////////////

MapNode map_first(Map map) {
	//Ξεκινάμε την επανάληψή μας απο το 1ο στοιχείο, μέχρι να βρούμε κάτι όντως τοποθετημένο
	for (int i = 0; i < map->capacity; i++)
		if (blist_size(map->array[i].blist_key_value) >= 1)
			return blist_node_value(map->array[i].blist_key_value,blist_first(map->array[i].blist_key_value));

	return MAP_EOF;
}

MapNode map_next(Map map, MapNode node) {
	// Το node είναι pointer στο i-οστό στοιχείο του array, οπότε node - array == i  (pointer arithmetic!)
	MapNode mapnode;
	BListNode bnode;
	for(int i = 0; i < map->capacity; i++){
		if(blist_find_node(map->array[i].blist_key_value,node,map->compare) == BLIST_EOF)
			continue;
		
		bnode = blist_find_node(map->array[i].blist_key_value,node,map->compare);
		
		mapnode = blist_node_value(map->array[i].blist_key_value,bnode);
		
			if(blist_next(map->array[i].blist_key_value,bnode) != BLIST_EOF){
				return blist_node_value(map->array[i].blist_key_value,blist_next(map->array[i].blist_key_value,bnode));
			}
			else{
				for(int j = i+1 ; j < map->capacity ; j++){
					if(blist_size(map->array[j].blist_key_value) >= 1 ){
					mapnode = blist_node_value(map->array[j].blist_key_value,blist_first(map->array[j].blist_key_value));
						
						return mapnode;
				}	}
			}
	}

	return MAP_EOF;
}

Pointer map_node_key(Map map, MapNode mapnode) {
	return mapnode->key;
}

Pointer map_node_value(Map map, MapNode mapnode) {
	return mapnode->value;
}

MapNode map_find_node(Map map, Pointer key) {
	MapNode mapnode;
	uint pos = map->hash_function(key) % map->capacity;
	if(blist_size(map->array[pos].blist_key_value) >= 1){
		for(BListNode node = blist_first(map->array[pos].blist_key_value); node != BLIST_EOF ; node = blist_next(map->array[pos].blist_key_value,node)){
			mapnode = blist_node_value(map->array[pos].blist_key_value,node);
			if(map->compare(mapnode->key,key) == 0)
				return mapnode;
	}		
	}

	return MAP_EOF;
}

// Αρχικοποίηση της συνάρτησης κατακερματισμού του συγκεκριμένου map.
void map_set_hash_function(Map map, HashFunc func) {
	map->hash_function = func;
}

uint hash_string(Pointer value) {
	// djb2 hash function, απλή, γρήγορη, και σε γενικές γραμμές αποδοτική
    uint hash = 5381;
    for (char* s = value; *s != '\0'; s++)
		hash = (hash << 5) + hash + *s;			// hash = (hash * 33) + *s. Το foo << 5 είναι γρηγορότερη εκδοχή του foo * 32.
    return hash;
}

uint hash_int(Pointer value) {
	return *(int*)value;
}

uint hash_pointer(Pointer value) {
	return (size_t)value;				// cast σε sizt_t, που έχει το ίδιο μήκος με έναν pointer
}