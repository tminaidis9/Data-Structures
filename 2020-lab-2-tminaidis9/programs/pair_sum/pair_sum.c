#include <stdlib.h>
#include <stdio.h>
#include "pair_sum.h"

Pair create_pair(int first,int second){
	Pair res = malloc(sizeof(struct pair));
	res->first = first;
	res->second = second;
	return res;
}


int* create_ints(int value) {
	int* pointer = malloc(sizeof(int));		// δέσμευση μνήμης
	*pointer = value;						// αντιγραφή του value στον νέο ακέραιο
	return pointer; }


Pair pair_sum(int target, Vector numbers) {

	int max = vector_size(numbers);

	for( int i = 0 ; i < max ; i++){
		int* a=vector_get_at(numbers,i);
		//printf("a:%d\n",*a);
            for( int x = 0 ; x < max ; x++){
			int* b = vector_get_at(numbers,x);
			//printf("b:%d\n",*b);
			if ((*a + *b) == *create_ints(target)){
				return create_pair(*a,*b);
			}
		}


	}
                       
	return NULL;
}