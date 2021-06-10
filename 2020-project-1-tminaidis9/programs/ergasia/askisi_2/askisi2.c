
#include <stdio.h>
#include "ADTMap.h"
#include <stdlib.h>
#include <string.h>

typedef char* String ;
char* strdup(const char *str1);


int* create_ints(int a) {
      int* p=malloc(sizeof(int));
      *p = a ;
      return p;
}

 int compare_ch(Pointer a, Pointer b){
       return strcmp(a,b);
}

int main(void) {
      
      Pointer keys ;
      int counter = 0 ;
      int stoixeia_map = 0 ;
      Map map = map_create(compare_ch, NULL , NULL);
      MapNode node ;
      String line = malloc(200 * sizeof(char));
      int mapnodevalue;
      //Pointer y = create_ints(1) ;
      
      while(fgets(line , 200 , stdin) != NULL) {
            // printf("line %s", line);
            
            if (counter > 0){

                 node = map_first(map) ;
                 keys = map_node_key(map,node);

                 for (int i = 1; i <= stoixeia_map ; i++){
                 
                       if (strcmp(line,(char*)keys) == 0 ){
                            mapnodevalue = *(int*)map_node_value(map,node);
                            mapnodevalue++;
                             map_remove(map, line);
                             map_insert(map,strdup(line),create_ints(mapnodevalue)) ; 
                              break;
                        }
                        else if (i+1 <= stoixeia_map){

                              node = map_next(map,node);
                              keys = map_node_key(map,node);
                        }
                        else {

                              map_insert(map, strdup(line) , create_ints(1));
                              counter += 1;
                              break;
                        }
                  }
            }
            else {

                  map_insert(map, strdup(line) , create_ints(1)) ;
                  counter = counter + 1;
            }
            
            stoixeia_map = counter ;
      }
      
}


