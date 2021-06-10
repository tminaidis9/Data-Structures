
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ADTPriorityQueue.h"

typedef char* String;

int* create_int(int i){
      int* p = malloc(1);
      *p = i;
      return p;
}

int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}


int main(int agrc,char* argv[]){

      PriorityQueue pqueue = pqueue_create(compare_ints,free,NULL);
      int steps = 0;
      int N = 10000;
      double am_steps ;

      if(strcmp(argv[1],"real")== 0){


            for(int i = 0 ; i < N ; i++){
                  pqueue_insert(pqueue,create_int(i));      //ορισθηκη στο τελος και μετρα τα βηματα
                  steps = pqueue_steps(pqueue);
                  printf("%d,%d\n",i+1 , steps);
            }

      }
      else if(strcmp(argv[1],"amortized")== 0){

            for(int i = 1 ; i < N ; i++){
                  pqueue_insert(pqueue,&i); //προσθηκη στο τελος και αθροισμα βηματων και μετα διαρεση με i
                  steps += pqueue_steps(pqueue);
                  am_steps = (double)steps/(double)i;
                  printf("%d,%0.3f\n",i , am_steps);
            }

      }
      else{
            fprintf(stderr,"!= real or amortized\n");
            return 0;

      }
            pqueue_destroy(pqueue);

}