
#include <stdbool.h> 

struct lifecell {
	int x;
	int y;
};

typedef struct lifecell* LifeCell; 
typedef struct lifestate* LifeState;
typedef char* String;


// Δημιουργεί μια κατάσταση του παιχνιδιού όπου όλα τα κελιά είναι νεκρά.
LifeState life_create();

// Δημιουργεί μία κατάσταση του παιχνιδιού με βάση τα δεδομένα του αρχείο file (RLE format)
LifeState life_create_from_rle(char* file);

// Αποθηκεύει την κατάσταση state στο αρχείο file (RLE format)
void life_save_to_rle(LifeState state, char* file);

// Επιστρέφει την τιμή του κελιού cell στην κατάσταση state (true: ζωντανό, false: νεκρό)
bool life_get_cell(LifeState state, LifeCell cell);

// Αλλάζει την τιμή του κελιού cell στην κατάσταση state
void life_set_cell(LifeState state, LifeCell cell, bool value);

// Παράγει και επιστρέφει μια νέα κατάσταση που προκύπτει από την εξέλιξη της κατάστασης state.
// Η ίδια η state δε μεταβάλλεται (ούτε καταστρέφεται).
LifeState life_evolve(LifeState state);

// Καταστρέφει την κατάσταση ελευθερώντας οποιαδήποτε μνήμη έχει δεσμευτεί
void life_destroy(LifeState state);