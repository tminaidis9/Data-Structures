
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ADTPriorityQueue.h"


struct priority_queue{
      Set set;
      CompareFunc compare;
      DestroyFunc destroy_value;
      PriorityQueueNode node ;
     
};

struct priority_queue_node{
      SetNode setnode;
};

PriorityQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value, Vector values){
      PriorityQueue pqueue = malloc(sizeof(*pqueue));    //δημιουργια της pqueue
      pqueue->set = set_create(compare,destroy_value);
      pqueue->compare = compare;
      pqueue->destroy_value = destroy_value;
      PriorityQueueNode node = malloc(sizeof(*node));
      node->setnode = NULL;
      pqueue->node = node;
      free(node);

      if(values != NULL){            //εαν δεχεται vector τοτε βαζουμε μια μια τις τιμες του μεσα στην pqueue δηλαδη στο set τη pqueue
            int size = vector_size(values);
            for (int i = 0; i < size; i++) {
                  int* value = vector_get_at(values, i);
                  set_insert(pqueue->set,value);
            }
      }
      return pqueue;
}


int pqueue_size(PriorityQueue pqueue){
      return set_size(pqueue->set);
}


Pointer pqueue_max(PriorityQueue pqueue){
      return set_node_value(pqueue->set,set_last(pqueue->set));
}


PriorityQueueNode pqueue_insert(PriorityQueue pqueue, Pointer value){
      set_insert(pqueue->set,value);
      PriorityQueueNode node = malloc(sizeof(*node));
      node->setnode = set_find_node(pqueue->set,value);
      pqueue->node = node;

      return pqueue->node;
}


void pqueue_remove_max(PriorityQueue pqueue){
      set_remove(pqueue->set,pqueue_max(pqueue));
}



DestroyFunc pqueue_set_destroy_value(PriorityQueue pqueue, DestroyFunc destroy_value){
      DestroyFunc old = pqueue->destroy_value;
      pqueue->destroy_value = destroy_value;
      return old;
}



void pqueue_destroy(PriorityQueue pqueue){
      set_destroy(pqueue->set);
      free(pqueue);
}





Pointer pqueue_node_value(PriorityQueue set, PriorityQueueNode node){
      return set_node_value(set->set,node->setnode);
}



void pqueue_remove_node(PriorityQueue pqueue, PriorityQueueNode node){
      assert(node != NULL);
      int* value = pqueue_node_value(pqueue,node);
      set_remove(pqueue->set,value);     

}



void pqueue_update_order(PriorityQueue pqueue, PriorityQueueNode node){
      
}