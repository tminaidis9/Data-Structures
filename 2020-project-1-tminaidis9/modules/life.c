//life.c

#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include "life.h"
#include "ADTMap.h"
#include <string.h>


struct lifestate {
	Map alive;
	LifeCell last_cell; //αποθηκεύεται το μεγαλύτερο i που έχει ένα ζωντανό κελί και το μεγαλύτερο j
};                        //που έχει ένα άλλο ζωντανό κελί.

LifeCell create_cell(int i,int j){       //Συνάρτηση για δημιουργία cell
	LifeCell cell = malloc(sizeof(struct lifecell));
	cell->x = i;
	cell->y = j;
	return cell;
}

int compare_cell(Pointer a, Pointer b) {     //compare για map
	LifeCell cell1 = a;
	LifeCell cell2 = b;
	if(cell1->x != cell2->x){
		return cell1->x - cell2->x;
	
	}
	else{
			return cell1->y - cell2->y;
	
	}
}


int* create_int(int a){
	int* p =malloc(sizeof(int));
	*p =a;
	return p;
}


// Δημιουργεί μια κατάσταση του παιχνιδιού όπου όλα τα κελιά είναι νεκρά. DONE
LifeState life_create(){

	LifeState state = malloc(sizeof(struct lifestate));
      Map map = map_create(compare_cell, free , free );
	char* file = "file.txt";
	FILE * fp;
	fp = fopen(file,"w");
	fprintf(fp,"%s","bbbbbb");
      fclose(fp);
	state->alive = map;
	state->last_cell = create_cell(-1,-1);
	return state;
}
// Δημιουργεί μία κατάσταση του παιχνιδιού με βάση τα δεδομένα του αρχείο file (RLE format) DONE
LifeState life_create_from_rle(char* file){

	LifeState state = life_create();
	// Map map = map_create(compare_int,NULL,NULL);
	char c;
	FILE * fp;
	LifeCell new_cell;
	fp = fopen(file,"r");
      int number = 0;
      int i = 0;
      int j = 0;
      int max_i = -1;
	int max_j = -1;

//Γέμισμα του πίνακα alive με βάση το RLE
	while(( c = fgetc(fp)) != EOF){

//Κάθε φορά που ο χαρακτήρας του RLE είναι αριθμός		
		if(number != 0){
			for(int a = 1; a <= number ; a++){
				if(c == 'o'){
					new_cell = create_cell(i,j);
					map_insert(state->alive,new_cell,create_int(1));
					if(max_i <= i) max_i = i;
					i++;
				}
				else{
                              i = i + 1;

				}
			}
			number = 0;
			continue;
		}

		if(c == 'b'){
			if(max_i <= i) max_i = i;
			i++;
			continue;
		}
		else if(c == 'o'){
			new_cell = create_cell(i,j);
			map_insert(state->alive,new_cell,create_int(1));
			 if(max_i <= i) max_i = i;
		            max_j = j;
			i++;
		}
		else if(c == '$'){
			j++;
			i = 0;
		}
		else{
			number = c - '0';
			}	
		
	}
	fclose(fp);

			

	state->last_cell = create_cell(max_i,max_j);
	
	return state;	
}


// Αποθηκεύει την κατάσταση state στο αρχείο file (RLE format)
void life_save_to_rle(LifeState state, char* file){
	LifeCell cell , next_cell;
	char num;
	int number = 1 , number2 = 1;
	int size = map_size(state->alive);
	if(size == 0){
		return;
	}
	else {
		int x = state->last_cell->x;
		int y = state->last_cell->y;
		int sum = (x+1) * (y+1);
		int counter = 0;
	      char f[sum]; 
		for(int i = 0; i <= y ; i++){     //αναζήτηση κάθετα

			if(i > 0){
				if(f[counter - 1] == 'b'){

					if((f[counter - 2] >= '0') && (f[counter - 2] <= '9')) {
						f[counter - 2] = ' ';
						f[counter - 1] = ' ';
						counter = counter - 2;
					}
						f[counter - 1] = ' ';
						counter = counter - 1;
				} 
				else{   
				     f[counter] = '$';        //κάθε φορά που αλλάζει το i αλλάζει και γραμμή άρα στην 
				      counter += 1;           //συμβολοσειρά βάζουμε το '$'
			      }  
			}

			for(int j=0 ; j<=x ; j++){    
				cell = create_cell(j,i);      //αναζήτηση στοιχείων στην ίδια γραμμή
				next_cell = create_cell(j+1,i);
                        if(number2 == 1){
				     if(life_get_cell(state,cell) == 1){ //έλεγχος για ζωντάνα στοιχεία 
					     if(life_get_cell(state,next_cell) == 1){
						     number +=1;
						     continue;
					      }
						else if(number != 1){
							num = number + '0';
							f[counter] = num;
							f[counter+1] = 'o';
							number = 1;
							counter += 2;
						}
						else{
							f[counter] = 'o';
							counter +=1;
						}
				      }  
				}              
				
			if(life_get_cell(state,cell) != 1){     //έλεγχος για νεκρά στοιχεία
					if(life_get_cell(state,next_cell) != 1){
						number2 +=1;
						if(j+1 >= x){ 
						     number2 = 1;}
						continue;
					}
					else if(number2 != 1){
                                    num = number2 + '0';
						f[counter] = num ;
						f[counter+1] = 'b';
						number2 = 1;
						counter += 2;
					}
					else{
						f[counter] = 'b';
						counter +=1;
					}                
				}
			}
		}
		f[counter] = '!';
		FILE* fp;
		fp = fopen(file,"w");
		fputs(f,fp);
		fclose(fp);
		}
}
	
	

// Επιστρέφει την τιμή του κελιού cell στην κατάσταση state (true: ζωντανό, false: νεκρό) DONE
bool life_get_cell(LifeState state, LifeCell cell){
	//printf("map size is %d\n\n", map_size(state->alive));
	if(map_find_node(state->alive,cell) != MAP_EOF){
		return 1;
	}
	else{
		return 0;}
}

// Αλλάζει την τιμή του κελιού cell στην κατάσταση state DONE 
void life_set_cell(LifeState state, LifeCell cell, bool value){	 
      LifeCell new_cell = create_cell(cell->x,cell->y); 
      if(value == true){
		if (life_get_cell(state,cell) == 1)
		map_insert(state->alive,new_cell,create_int(1));
	}
	else{
		if (life_get_cell(state,cell) == 1)
		     map_remove(state->alive,cell);
	}
}

// Παράγει και επιστρέφει μια νέα κατάσταση που προκύπτει από την εξέλιξη της κατάστασης state.
// Η ίδια η state δε μεταβάλλεται (ούτε καταστρέφεται).
LifeState life_evolve(LifeState state){
	return 0;

}

// Καταστρέφει την κατάσταση ελευθερώντας οποιαδήποτε μνήμη έχει δεσμευτεί done
void life_destroy(LifeState state){
	map_destroy(state->alive);
	free(state->last_cell);
}