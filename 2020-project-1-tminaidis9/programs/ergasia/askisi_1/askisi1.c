

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

typedef char* String;

int main(void){
      String line = malloc(20);
      String ch[100000];
      int x = 0;
      int times = 0;
     
     //δυν. δεσμευση χωρου
     for(int i = 0 ; i < 100000 ; i++)
           ch[i]=(char*)malloc(20*sizeof(char));


      //Ξεκιναει να διαβαζει τις σειρες με την fgets
      while(fgets(line,20,stdin)!= NULL) {
          
            ch[x] = strdup(line);
            x=x+1;
      }

      for(int i = 0; i < x ; i++){
            for (int j = 0 ; j < x ; j++){
                  if((strcmp(ch[i],ch[j])) == 0){
                              
                              times += 1 ;
                  }
            }
            printf("time for %d is %d\n", i , times);
            times = 0 ;
      }
}

