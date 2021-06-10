
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ADTVector.h"

typedef char* String;

int* create_int(int i){
      int* p = malloc(1);
      *p = i;
      return p;
}

int main(int agrc,char* argv[]){

      Vector vec = vector_create(0,NULL);
      int steps = 0;
      int N = 10000;
      double am_steps ;

      if(strcmp(argv[1],"real")== 0){


            for(int i = 0 ; i < N ; i++){
                  vector_insert_last(vec,&i);      //ορισθηκη στο τελος και μετρα τα βηματα
                  steps = vector_steps(vec);
                  printf("%d,%d\n",i+1 , steps);
            }

      }
      else if(strcmp(argv[1],"amortized")== 0){

            for(int i = 1 ; i <= N ; i++){
                  vector_insert_last(vec,&i); //προσθηκη στο τελος και αθροισμα βηματων και μετα διαρεση με i
                  steps += vector_steps(vec);
                  am_steps = (double)steps/(double)i;
                  printf("%d,%0.3f\n",i , am_steps);
            }

      }
      else{
            fprintf(stderr,"!= real or amortized\n");
            return 0;

      }
            vector_destroy(vec);

}