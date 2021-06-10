
#include "ADTMap.h"
#include "life.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LifeCell create_cell(int i,int j);

void create_file(char* file){
      FILE* fp;
      fp = fopen(file,"w");
      fprintf(fp,"%s","3o$2o$bobo");
      fclose(fp);
}

 int main(void){
      LifeCell cell = create_cell(1,3);
      char* file = "file.txt";
      create_file(file);
      LifeState state = life_create_from_rle(file);
      int i = life_get_cell(state,cell);
      printf("i is %d\n", i);
     life_save_to_rle(state, file);
      
}