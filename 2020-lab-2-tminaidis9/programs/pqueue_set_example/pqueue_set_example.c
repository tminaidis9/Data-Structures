#include <stdio.h>

#include <stdlib.h>

#include <time.h>

#include "ADTPriorityQueue.h"

#include "ADTSet.h"

typedef void* Pointer ;

int* compare_ints(Pointer a, Pointer b) {
      int* ia = a;
      int* ib = b;
      return ia - ib;
}


int* create_ints(int x){
      int* p = malloc(sizeof(int));
      *p = x;
      return p;
}

int main(void){

      //το πρωτο κομμματι που αφορα την priority queue
      PriorityQueue pq = pqueue_create(compare_ints,NULL,NULL);
      long curtime;
      curtime=time(NULL);
      srand((unsigned int)curtime); //απαραιτητες διαδικασιες για rand

      
      for( int i = 0 ; i < 19 ; i++ ){   // Τοποθετω τα 20 στοιχεια στην pq
            int x = rand();
            pqueue_insert(pq, *create_ints(x));
      }

       printf("no of pq is : %d\n", pqueue_size(pq) );
      
      for( int i = 0 ; i< 19 ; i++){
            printf("pq max is : %d\n", pqueue_max(pq));
            pqueue_remove_max(pq);
            printf("no of pq is : %d\n",pqueue_size(pq) );
      }
     
            printf("\n\n\n");


      //το κομματι που αφορα το set

      Set set1 = set_create(compare_ints, NULL);

      for(int i = 0 ; i < 19 ; i++){
            int y = rand();
            set_insert(set1, create_ints(y));
      }
      
      printf(" set max is : %d\n", set_first(set1));
      Pointer m = set_next(set1, set_first(set1));
       printf("next max is : %d\n", m);

      for (int i = 0 ; i < 17 ; i++){
            m = set_next(set1, m); 
             printf("next max is : %d\n", m);

            
      }


}




