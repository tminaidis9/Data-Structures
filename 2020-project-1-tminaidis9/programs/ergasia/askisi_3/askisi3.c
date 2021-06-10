#include <stdio.h>
#include <stdlib.h>
#include "ADTSet.h"
#include <string.h>

typedef char* String;
char * strdup(const char *str1);


int compare_ch (Pointer a , Pointer b) {
      return strcmp(a,b);
}


int main(void) {

      Set set = set_create(compare_ch,NULL);
      String line = malloc(200);
      int counter = 0 ;
      String name ;
      String smallest;
      int c ;
      
      while (fgets(line , 200 , stdin) != NULL) {

            if ( counter > 0){
                  
                  c = 0 ;
                  smallest = set_node_value(set,set_first(set));
                  for(SetNode node = set_first(set);          
                  node != SET_EOF;                        
                  node = set_next(set, node)) {
                        
        
                        name = set_node_value(set, node);
                       
                              if (strcmp(name,line) >= 0){

                                    if(strcmp(smallest,line)<= 0){
                                          
                                          smallest = strdup(name);
                                          c = 1 ; 

                                    }
                                    
                                    if (strcmp(smallest,name) >= 0){

                                           c = 1;
                                          smallest =strdup(name);

                                    } 
                              }
                  }

                  if (c == 0) {
                      printf("<none>\n");
                  } 
                  else {
                      printf("%s\n", smallest);
                  }

                  set_insert(set , strdup(line));
                  counter = counter + 1 ;

            }
            else {
                  set_insert(set , strdup(line));
                  counter = counter + 1 ;
                      printf("<none>\n");

            }
            
      }
}