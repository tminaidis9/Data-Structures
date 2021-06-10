///////////////////////////////////////////////////////////////////
//
// Disease Monitor
//
// Module που παρέχει στατιστικά για την εξέλιξη μιας ασθένειας.
//
///////////////////////////////////////////////////////////////////


#include "ADTList.h"
#include "ADTSet.h"
#include "DiseaseMonitor.h"
#include "ADTPriorityQueue.h"
#include "ADTMap.h"
#include "ADTGraph.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Οι ημερομηνίες δίνονται σαν Strings, σε format YYYY-MM-DD, πχ "2019-10-31"

//Όλες οι συναρτήσεις create-compare που θα χρειαστούμε//////////////////////////////////////////

int my_strcmp (Pointer p1, Pointer p2)
{
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
        return c1 - c2;
    }
  while (c1 == c2);
  return c1 - c2;
}

int* cr_int(int value) {
	int* p = malloc(sizeof(int));
	*p = value;
	return p;
}

int compare_ints(Pointer a , Pointer b){
      return *(int*)a - *(int*)b;
}

int compare_records(Record a,Record b){
      return b->id - a->id;
}

int sec_compare_ints(Pointer a,Pointer b){
      return *(int*)b - *(int*)a;
}


///////////////////////////////////////////////////////////////////////

// Καθολικές μεταβλητές.
static Map based_on_ids; //Με βάση τον id 
static Map countries; // Όλες οι χώρες που έχουν εισαχθεί
static Map map2; // about country and date
static Map top_diseases;   
// static Map priority;


void dm_init(){
      countries = map_create(my_strcmp,NULL,NULL);
      based_on_ids = map_create(compare_ints,NULL,NULL);
      top_diseases = map_create(my_strcmp,NULL,NULL);
      map2 = map_create(my_strcmp,NULL,NULL);

	map_set_hash_function(based_on_ids, hash_int);
	map_set_hash_function(top_diseases, hash_string);
	map_set_hash_function(map2, hash_string);
	map_set_hash_function(countries, hash_string);



}

// Καταστρέφει όλες τις δομές του monitor, απελευθερώνοντας την αντίστοιχη
// μνήμη. ΔΕΝ κάνει free τα records, αυτά δημιουργούνται και καταστρέφονται από
// τον χρήστη.

void dm_destroy(){
      map_destroy(countries);
      map_destroy(based_on_ids);
      map_destroy(top_diseases);
      // map_destroy(priority);
      map_destroy(map2);

}


// Προσθέτει την εγγραφή record στο monitor. Δεν δεσμεύει νέα μνήμη (ούτε
// φτιάχνει αντίγραφα του record), απλά αποθηκεύει τον pointer (η δέσμευση
// μνήμης για τα records είναι ευθύνη του χρήστη). Αν υπάρχει εγγραφή με το ίδιο
// id αντικαθίσταται και επιστρέφεται true, αν όχι επιστρέφεται false.
//
// Οι αλλαγές στα δεδομένα της εγγραφής απαγορεύονται μέχρι να γίνει remove από
// τον monitor.

bool dm_insert_record(Record record){
      Map priority = map_create(compare_ints,NULL,NULL);
	map_set_hash_function(priority, hash_int);

      Pointer id = cr_int(record->id);
      int id_exist = 0;                                     // Eάν ο id υπάρχει ήδη
	bool ret = false;
      Set aset = set_create(compare_ints,NULL);
      MapNode amapnode;
      
      
      Map second_map = map_create(my_strcmp,NULL,NULL);  
      Map sec_map = map_create(my_strcmp,NULL,NULL);
      map_set_hash_function(second_map, hash_string);
	map_set_hash_function(sec_map, hash_string);
      
      //  printf("exist %d \n",id_exist);


      if (map_size(based_on_ids) != 0){
            // printf("mpike sthn prwth epanalipsi\n");
            if(map_find(based_on_ids,id) != NULL){
                  // printf("mpike sthn deyterh epanalipsi\n");
                  id_exist = 1;                            //ο id υπάρχει
            }

            // printf("exist %d \n",id_exist);

      }
//////////////////// αν υπάρχει ήδη το id κανω αντικατασταση////////////////////////    
      if(id_exist == 1){
            Record old_record = map_find(based_on_ids,id);
            
            Map map_with_diseases = map_find(top_diseases,old_record->country);
            amapnode = map_find_node(map_with_diseases,old_record->disease);
            Pointer counter = map_find(map_with_diseases,old_record->disease);
            Map priority = map_find(countries,old_record->country);
            aset = map_find(priority,counter);
            set_remove(aset,amapnode);
            counter = cr_int(*(int*)counter - 1);
            printf("map_size is %d \n",map_size(map_with_diseases));

            if(*(int*)counter == 0){
                  if(map_find(map_with_diseases,old_record->disease) != NULL)
                        map_remove(map_with_diseases,old_record->disease);
            }
            else{
                  if(map_find_node(priority,counter) != MAP_EOF){
                        aset = map_find(priority,counter);
                  }
                  else{
                        aset = set_create(compare_ints,NULL);
                  }
                        
                  set_insert(aset,amapnode);
                  map_insert(priority,counter,aset);
                  map_insert(map_with_diseases,old_record->disease,counter);
            }
            
            printf("counter == %d \n",*(int*)counter);

            Map map_with_dates = map_find(map2,old_record->country);        
            Map map_with_records =  map_find(map_with_dates,old_record->date);
            map_remove(map_with_records,id);
            printf("counter == %d \n",*(int*)counter);
            
            if(map_size(map_with_records) == 0)
                  map_remove(map_with_dates,old_record->date);
            
            ret = true;

      }      
///////////////////////////////////διαφορετικα..////////////////////////////////////////////////////
      map_insert(based_on_ids,id,record);
      
            // printf("exist %d \n",id_exist);
      if(map_size(countries) != 0){
            // printf("bhma 1 \n");
            if(map_find_node(countries,record->country) == MAP_EOF){                    //Εάν δεν υπάρχει η χώρα από την οποία είναι ο ασθενης
                  // printf("bhma 2 \n");
                  Map priority2 = map_create(compare_ints,NULL,NULL);
                  map_insert(countries,record->country,priority2);  // insert τη χώρα στις υπόλοιπες χώρες
                  Set aset2 = set_create(compare_ints,NULL);
                  Map map = map_create(compare_ints,NULL,NULL);          //Δημιουργία set με records της καθε ημερομηνίας
                  map_set_hash_function(map, hash_int);
                  map_insert(map,id,record);
                  map_insert(second_map,record->date,map);     // records ανά ημερα
                  map_insert(map2,record->country,second_map); // records ανά χώρα

                  map_insert(sec_map,record->disease,cr_int(1));
                  MapNode amapnode2 = map_find_node(sec_map,record->disease);
                  set_insert(aset2,amapnode2);
                  map_insert(priority2,cr_int(1),aset2);
                  map_insert(top_diseases,record->country,sec_map); //Δημιουργία μιας disease στον map και επειδή είναι η πρώτη φορά που συναντάμε την αρρώστια βάζουμε value == 1
                  

                  return ret;
            }
            else{ 
            // printf("bhma 3 \n");
                  MapNode mapnode = map_find_node(map2,record->country);
                  MapNode mapnode2 = map_find_node(top_diseases,record->country);
                  Map priority3 = map_find(countries,record->country);
                  if(map_node_value(map2,mapnode) != NULL)
                        second_map = map_node_value(map2,mapnode);

                  if(map_node_value(top_diseases,mapnode2) != NULL)
                        sec_map = map_node_value(top_diseases,mapnode2);

                  //ψάξε να δεις άμα υπάρχει το date
                  //ψάξε να δεις άμα υπάρχει το disease
                 if ( map_find(second_map,record->date) == NULL){
                        // printf("1 mpike,oti mpainei einai kalo\n");
                  
                  if(map_find(sec_map,record->disease) == NULL){
                        // printf("2 mpike,oti mpainei einai kalo\n\n");
                        Map map = map_create(compare_ints,NULL,NULL);
                        map_set_hash_function(map, hash_int);
                        map_insert(map,id,record);
                        map_insert(second_map,record->date,map);
                        map_insert(map2,record->country,second_map);
                       
                        map_insert(sec_map,record->disease,cr_int(1));
                        MapNode amapnode5 = map_find_node(sec_map,record->disease);
                        Set aset5 = set_create(compare_ints,NULL);
                        set_insert(aset5,amapnode5);
                        map_insert(top_diseases,record->country,sec_map);
                        map_insert(priority3,cr_int(1),aset5);
                  
                        return ret;
                  }
                  else{
                        // printf("3 mpike,oti mpainei einai kalo\n\n");
                        
                        Map map = map_create(compare_ints,NULL,NULL);
                        map_set_hash_function(map, hash_int);
                        map_insert(map,id,record);
                        map_insert(second_map,record->date,map);
                        map_insert(map2,record->country,second_map);
                        
                        MapNode node2 = map_find_node(sec_map,record->disease);
                        Pointer integer = map_find(sec_map,record->disease);
                        Set aset7 = map_find(priority3,integer);
                        set_remove(aset7,node2);
                        integer = cr_int(*(int*)integer + 1);
                        if(map_find_node(priority3,integer) == MAP_EOF){
                              Set set9 = set_create(compare_ints,NULL);
                              set_insert(set9,node2);
                              map_insert(priority3,integer,set9);
                        }
                        else{
                              Set set9 = map_find(priority3,integer);
                              set_insert(set9,node2);

                        }

                        map_insert(sec_map,record->disease,integer);
                        map_insert(top_diseases,record->country,sec_map);
                        
                        return ret;
                  }

                 }
                  else if(map_find_node(sec_map,record->disease) == MAP_EOF){
                        // printf("4 mpike,oti mpainei einai kalo\n\n");
                        
                        Map map = map_find(second_map,record->date);
                        map_insert(map,id,record);
                        map_insert(second_map,record->date,map);
                        map_insert(map2,record->country,second_map);

                        map_insert(sec_map,record->disease,cr_int(1));
                        MapNode mapn = map_find_node(sec_map,record->disease);
                        if(map_find_node(priority3,cr_int(1)) == MAP_EOF){
                              Set oneset = set_create(compare_ints,NULL);
                              set_insert(oneset,mapn);
                              map_insert(priority3,cr_int(1),oneset);
                        }
                        else{
                              Set set9 = map_find(priority3,cr_int(1));
                              set_insert(set9,mapn);

                        }
                        map_insert(top_diseases,record->country,sec_map);

                        return ret;
                  }
                  else{
                        // printf("5 mpike,oti mpainei einai kalo\n\n");
                        
                        Map map = map_find(second_map,record->date);
                        map_insert(map,id,record);
                        map_insert(second_map,record->date,map);
                        map_insert(map2,record->country,second_map);
                        
                        MapNode mapno = map_find_node(sec_map,record->disease);
                        
                        Pointer integer = map_find(sec_map,record->disease);
                        Set set10 = map_find(priority3,integer);
                        set_remove(set10,mapno);

                        integer = cr_int(*(int*)integer + 1);
                        if(map_find_node(priority3,cr_int(1)) == MAP_EOF){
                              Set one_set = set_create(compare_ints,NULL);
                              set_insert(one_set,mapno);
                              map_insert(priority3,cr_int(1),one_set);
                        }
                        else{
                              Set set9 = map_find(priority3,cr_int(1));
                              set_insert(set9,mapno);

                        }
                        map_insert(sec_map,record->disease,integer);
                        map_insert(top_diseases,record->country,sec_map);
                        return ret;
                  }
            }     
      }
      else{  // δεν υπάρχει καμία καταχώρηση,αυτή είναι η πρώτη
                  map_insert(countries,record->country,priority);  // insert τη χώρα στις υπόλοιπες χώρες                  
                  Map map = map_create(compare_ints,NULL,NULL);          //Δημιουργία set με records της καθε ημερομηνίας
                  map_set_hash_function(map, hash_int);
                  map_insert(map,id,record);
                  map_insert(second_map,record->date,map);     // records ανά ημερα                  
                  map_insert(map2,record->country,second_map); // records ανά χώρα
                  map_insert(sec_map,record->disease,cr_int(1));
                  map_insert(top_diseases,record->country,sec_map); //Δημιουργία μιας disease στον map και επειδή είναι η πρώτη φορά που συναντάμε την αρρώστια βάζουμε value == 1                  
                  MapNode amapnode = map_find_node(sec_map,record->disease);
                  set_insert(aset,amapnode);
                  map_insert(priority,cr_int(1),aset);
                  return ret;
      }
      
}

// Αφαιρεί την εγγραφή με το συγκεκριμένο id από το σύστημα (χωρίς free, είναι
// ευθύνη του χρήστη). Επιστρέφει true αν υπήρχε τέτοια εγγραφή, αλλιώς false.

bool dm_remove_record(int id){
      // int* pointer = &id;
      // for (MapNode node = map_first(based_on_ids) ; node != MAP_EOF ; node = map_next(based_on_ids,node)){
      //       int* key = (int*)map_node_key(based_on_ids,node);
      //       printf("map == %d \n", *(int*)map_node_key(based_on_ids,node));
      //       printf("id == %d\n",*(int*)pointer);

      // }
      if (map_find_node(based_on_ids,&id) != MAP_EOF){
            Record record_to_remove = map_find(based_on_ids,&id);
            // printf("mpainei swsta\n");

            Map map_with_diseases = map_find(top_diseases,record_to_remove->country);
            Pointer counter = map_find(map_with_diseases,record_to_remove->disease);
            counter = cr_int(*(int*)counter - 1);
            map_insert(map_with_diseases,record_to_remove->disease,counter);

            Map map_with_dates = map_find(map2,record_to_remove->country);        
            Map map_with_records =  map_find(map_with_dates,record_to_remove->date);
            map_remove(map_with_records,&record_to_remove->id);
            
            // Εάν δεν υπάρχει άλλη καταχώρηση με το ίδιο country κάνε delete το country
            if (map_size(map_with_dates) == 0)
                  map_remove(countries,record_to_remove->country);
            
            map_remove(based_on_ids,&id);
            return true; 
      }      
      return false;
}

// Επιστρέφει λίστα με τα Records που ικανοποιούν τα συγκεκριμένα κριτήρια, σε
// οποιαδήποτε σειρά.

List dm_get_records(String disease, String country, Date date_from, Date date_to){
      List get_records = list_create(NULL);
      MapNode date_search_2 ;
      MapNode date_search;
      Map map_with_dates;
      Record rec;
      MapNode previous;

      if(map_size(map2) == 0) // αν δεν υπάρχουν χώρες επέστρεψε πάλι άδεια λιστα
            return get_records;
      
      if(country != NULL)
            if(map_find(map2,country) == NULL) //Αν η χώρα που ζητάει ο χρήστης δεν υπάρχει επιστρέφουμε άδεια λίστα
                  return get_records;
      if(country == NULL){ // αν θέλει να αναζητήσω σε κάθε χώρα 
      // printf("mpike swsta\n");
            for(MapNode node = map_first(map2) ; node != MAP_EOF ; node = map_next(map2,node)){ //Για κάθε χώρα
                  // printf("map2 size == %d \n",map_size(map2));
                  printf("country -> %s \n",(char*)map_node_key(map2,node));
                  map_with_dates = map_node_value(map2,node); //map2 -> key == country, value == map_with_dates 
                  date_search = map_first(map_with_dates); // map_wth_dates -> key == date , value == set_with_records
                  int right = 0;
                  int go_out = 0;
                  if(date_from != NULL){ 
                        if (map_find(map_with_dates,date_from) != NULL){ // αν η date υπάρχει                                    
                                   date_search = map_find_node(map_with_dates,date_from);
                        }
                        else{ // Αν δεν υπάρχει ψάξε την πιο κοντινή ημερομηνία
                              Date second_date_from = date_from;
                              if(date_to != NULL){
                                    while (strcmp(second_date_from,date_to) <= 0 && right == 0 && go_out == 0){ 
                                          second_date_from = map_node_key(map_with_dates,date_search);
                                          if(strcmp(second_date_from,date_from) >= 0 && strcmp(second_date_from,date_to) <= 0){
                                                right = 1;
                                          }                        
                                          else if(strcmp(second_date_from,date_from) <= 0){
                                                if(map_next(map_with_dates,date_search) != MAP_EOF)
                                                      date_search = map_next(map_with_dates,date_search);
                                                else if(date_search == map_first(map_with_dates))
                                                      go_out = 1;
                                                else
                                                      right = 1;
                                          }
                                          else{
                                                go_out = 1;
                                          }     
                                    }
                              }
                              else {
                                     while (right == 0 && go_out == 0){ 
                                          second_date_from = map_node_key(map_with_dates,date_search);
                                          if(strcmp(second_date_from,date_from) >= 0){
                                                right = 1;
                                          }       
                                          else if(strcmp(second_date_from,date_from) <= 0){
                                                if(map_next(map_with_dates,date_search) != MAP_EOF)
                                                      date_search = map_next(map_with_dates,date_search);
                                                else if (date_search == map_first(map_with_dates))
                                                      go_out = 1;
                                                else 
                                                      right = 1;
                                          }     
                                          else{
                                                go_out = 1;
                                          }
                                    }
                              }
                        }
                  }

                  if(go_out == 1)
                        continue;
                  
                  go_out = 0;

                  if(date_from == NULL)
                  date_search = map_first(map_with_dates);

                  int right_2 = 0;
                  if(date_to != NULL){
                        printf("kollaei edw\n");

                        if (map_find(map_with_dates,date_to) != NULL){
                              date_search_2 = map_find_node(map_with_dates,date_to);                        }
                        else{
                              previous = date_search;
                              date_search_2 = date_search;
                              if(date_from != NULL){
                              Date second_date_to = date_from; // O ίδιος αλγόριθμος με τον date from
                              
                                    while (strcmp(second_date_to,date_from) >= 0 && right_2 == 0 && go_out == 0){ 
                                          second_date_to = map_node_key(map_with_dates,date_search_2);
                                          if(strcmp(second_date_to,date_from) >= 0 && strcmp(second_date_to,date_to) <= 0){
                                                if(map_next(map_with_dates,date_search_2) != MAP_EOF){
                                                      previous = date_search_2;
                                                      date_search_2 = map_next(map_with_dates,date_search_2);
                                                }
                                                else{
                                                      right_2 = 1; 
                                                }
                                          }
                                          else if(strcmp(second_date_to,date_to) >= 0){
                                                if(date_search_2 == map_first(map_with_dates)){
                                                      go_out = 1;
                                                }
                                                else{
                                                      right_2 = 1;
                                                      date_search_2 = previous;
                                                }
                                          }                           
                                    }
                              }
                              else{
                                    previous = date_search;
                                    Date second_date_to; 
                                    while (right_2 == 0 && go_out == 0){ 
                                          second_date_to = map_node_key(map_with_dates,date_search_2);
                                          if(strcmp(second_date_to,date_to) <= 0){
                                                   previous = date_search_2;
                                                if(map_next(map_with_dates,date_search_2) != MAP_EOF){
                                                      date_search_2 = map_next(map_with_dates,date_search_2);
                                                }
                                                else{
                                                      right_2 = 1;
                                                }
                                          } 
                                          else if(strcmp(second_date_to,date_to) >= 0){
                                                if(date_search_2 == map_first(map_with_dates)){
                                                      go_out = 1;
                                                }
                                                else{
                                                      right_2 = 1;
                                                      date_search_2 = previous;
                                                } 
                                          }
                                    }
                              }
                        }
                  }

                  if (go_out == 1)
                        continue;

                  if(date_to == NULL){
                        
                        for(MapNode node = map_first(map_with_dates) ; node != MAP_EOF ; node = map_next(map_with_dates,node)){
                              printf("look here\n");
                              printf("date is %s \n",(char*)map_node_key(map_with_dates,node));
                        }

                        for(MapNode node = date_search ; node != MAP_EOF ; node = map_next(map_with_dates,node)){
                              // printf("date is %s \n",(char*)map_node_key(map_with_dates,node));
                              Map map = map_node_value(map_with_dates,node);
                              //      printf("map size iiiis %d \n",map_size(map_with_dates));
                              if (map_size(map) != 0){
                                    for(MapNode mapnode = map_first(map) ; mapnode != MAP_EOF ; mapnode = map_next(map,mapnode)){                                    
                                          rec = map_node_value(map,mapnode);
                                          if((disease != NULL && strcmp(disease,rec->disease) == 0) || (disease == NULL))
                                          list_insert_next(get_records,LIST_EOF,map_node_value(map,mapnode));

                                    }
                              }
                        }
                  }
                  else{
                        if(date_search == date_search_2){
                        printf("337\n");
                             
                             Map map = map_node_value(map_with_dates,date_search);

                              for(MapNode mapnode = map_first(map) ; mapnode != MAP_EOF ; mapnode = map_next(map,mapnode)){
                                    rec = map_node_value(map,mapnode);
                                    if((disease != NULL && strcmp(disease,rec->disease) == 0) || (disease == NULL))
                                          list_insert_next(get_records,LIST_EOF,map_node_value(map,mapnode));
                              } 
                        }
                        else{
                                    
                              if(map_next(map_with_dates,date_search_2) != MAP_EOF){      
                                    for(MapNode node = date_search ; node != map_next(map_with_dates,date_search_2) ; node = map_next(map_with_dates,node)){
                                    printf("334\n");

                                          Map map = map_node_value(map_with_dates,node);
                                          for(MapNode mapnode = map_first(map) ; mapnode != MAP_EOF ; mapnode = map_next(map,mapnode)){
                                                rec = map_node_value(map,mapnode);
                                                if((disease != NULL && strcmp(disease,rec->disease) == 0) || (disease == NULL))
                                                      list_insert_next(get_records,LIST_EOF,map_node_value(map,mapnode));
                                          }

                                    }
                              }
                              else {
                                    for(MapNode node = date_search ; node != MAP_EOF ; node = map_next(map_with_dates,node)){
                                    printf("336\n");

                                          Map map = map_node_value(map_with_dates,node);
                                          for(MapNode mapnode = map_first(map) ; mapnode != MAP_EOF ; mapnode = map_next(map,mapnode)){
                                                rec = map_node_value(map,mapnode);
                                                if((disease != NULL && strcmp(disease,rec->disease) == 0) || (disease == NULL))
                                                      list_insert_next(get_records,LIST_EOF,map_node_value(map,mapnode));
                                          }

                                    }
                              }
                        }
                  }
            }
                  return get_records;           
      }
      

            map_with_dates = map_find(map2,country);
            date_search = map_first(map_with_dates);
            
            
            int right = 0;
            int go_out = 0;
            if(date_from != NULL){ 
                  if (map_find(map_with_dates,date_from) != NULL){ // αν η date υπάρχει 
                              date_search = map_find_node(map_with_dates,date_from);
                  }
                  else{ // Αν δεν υπάρχει ψάξε την πιο κοντινή ημερομηνία
                        Date second_date_from = date_from;
                        if(date_to != NULL){
                              while (strcmp(second_date_from,date_to) <= 0 && right == 0 && go_out == 0){ 
                                   second_date_from = map_node_key(map_with_dates,date_search);
                                          if(strcmp(second_date_from,date_from) >= 0 && strcmp(second_date_from,date_to) <= 0){
                                                right = 1;
                                          }                        
                                          else if(strcmp(second_date_from,date_from) <= 0){
                                                if(map_next(map_with_dates,date_search) != MAP_EOF)
                                                      date_search = map_next(map_with_dates,date_search);
                                                else if(date_search == map_first(map_with_dates))
                                                      go_out = 1;
                                                else
                                                      right = 1;
                                          }
                                          else{
                                                go_out = 1;
                                          }       
                              }
                        }
                        else{
                              while (right == 0 && go_out == 0){ 
                                    second_date_from = map_node_key(map_with_dates,date_search);
                                    if(strcmp(second_date_from,date_from) >= 0){
                                          right = 1;
                                    }       
                                    else if(strcmp(second_date_from,date_from) <= 0){
                                          if(map_next(map_with_dates,date_search) != MAP_EOF)
                                                date_search = map_next(map_with_dates,date_search);
                                          else if (date_search == map_first(map_with_dates))
                                                go_out = 1;
                                          else 
                                                right = 1;
                                    }     
                                    else{
                                          go_out = 1;
                                    }
                              }
                        }
                        
                  }
            }

            if(go_out == 1)
                  return get_records;
            
            // Εάν το date to η το date_from ειναι null
            if(date_from == NULL)
                  date_search = map_first(map_with_dates);   

            int right_2 = 0;
            if(date_to != NULL){
                  if (map_find(map_with_dates,date_to) != NULL){
                        date_search_2 = map_find_node(map_with_dates,date_to);
                  }
                  else{
                        date_search_2 = date_search;
                        previous = date_search;
                        if(date_from != NULL){
                              Date second_date_to = date_to; // O ίδιος αλγόριθμος με τον date from
                              while (strcmp(second_date_to,date_from) >= 0 && right_2 == 0 && go_out == 0){ 
                                    second_date_to = map_node_key(map_with_dates,date_search_2);
                                    if(strcmp(second_date_to,date_from) >= 0 && strcmp(second_date_to,date_to) <= 0){
                                          if(map_next(map_with_dates,date_search_2) != MAP_EOF){
                                                previous = date_search_2;
                                                date_search_2 = map_next(map_with_dates,date_search_2);
                                          }else{
                                                right_2 = 1;
                                          }                                    
                                    }                        
                                    else if (strcmp(second_date_to,date_to) >= 0){
                                          if(date_search_2 == map_first(map_with_dates)){
                                                      go_out = 1;
                                          }
                                          else{
                                                right_2 = 1;
                                                date_search_2 = previous;
                                          }
                                          
                                    }    
                              }
                        }
                        else {
                              Date second_date_to = date_to; // O ίδιος αλγόριθμος με τον date from
                              previous = date_search;
                              while (right_2 == 0 && go_out == 0){ 
                                    second_date_to = map_node_key(map_with_dates,date_search);
                                    if(strcmp(second_date_to,date_to) <= 0){
                                          previous = date_search_2;
                                          if(map_next(map_with_dates,date_search) != MAP_EOF){
                                                previous = date_search_2;
                                                date_search_2 = map_next(map_with_dates,date_search);
                                          }else{
                                                right_2 = 1;
                                          }
                                    }                        
                                    else if (strcmp(second_date_to,date_to) >= 0){
                                          if(date_search_2 == map_first(map_with_dates)){
                                                      go_out = 1;
                                                }
                                                else{
                                                      right_2 = 1;
                                                      date_search_2 = previous;
                                                } 
                                    }  
                              }
                        }
                  }
            }

            if (go_out == 1)
                  return get_records;

      if(date_to == NULL){
            for(MapNode node = date_search ; node != MAP_EOF ; node = map_next(map_with_dates,node)){
                  Map map = map_node_value(map_with_dates,node);
                  if(map_size(map) != 0){
                        for(MapNode mapnode = map_first(map) ; mapnode != MAP_EOF ; mapnode = map_next(map,mapnode)){
                              rec = map_node_value(map,mapnode);
                              if((disease != NULL && strcmp(disease,rec->disease) == 0) || (disease == NULL))
                                    list_insert_next(get_records,LIST_EOF,map_node_value(map,mapnode));
                        }
                  }
            }
      }
      else{
            if(map_next(map_with_dates,date_search_2) != MAP_EOF){      
                  for(MapNode node = date_search ; node != map_next(map_with_dates,date_search_2) ; node = map_next(map_with_dates,node)){
                  printf("334\n");

                        Map map = map_node_value(map_with_dates,node);
                        for(MapNode mapnode = map_first(map) ; mapnode != MAP_EOF ; mapnode = map_next(map,mapnode)){
                              rec = map_node_value(map,mapnode);
                              if((disease != NULL && strcmp(disease,rec->disease) == 0) || (disease == NULL))
                                    list_insert_next(get_records,LIST_EOF,map_node_value(map,mapnode));
                        }

                  }
            }
            else {
                  for(MapNode node = date_search ; node != MAP_EOF ; node = map_next(map_with_dates,node)){
                  printf("336\n");

                        Map map = map_node_value(map_with_dates,node);
                        for(MapNode mapnode = map_first(map) ; mapnode != MAP_EOF ; mapnode = map_next(map,mapnode)){
                              rec = map_node_value(map,mapnode);
                              if((disease != NULL && strcmp(disease,rec->disease) == 0) || (disease == NULL))
                                    list_insert_next(get_records,LIST_EOF,map_node_value(map,mapnode));
                        }

                  }
            }
            
      }


       return get_records;
 }

// Επιστρέφει τον αριθμό εγγραφών που ικανοποιούν τα συγκεκριμένα κριτήρια.

int dm_count_records(String disease, String country, Date date_from, Date date_to){
      int counter = 0;
      // if(country != NULL && date_from != NULL && date_to != NULL && disease != NULL){
           
      // }
      // else if(country != NULL && date_from != NULL && date_to != NULL  && disease == NULL){
                  
      //       }
      // }
      // else if(country != NULL && date_from == NULL && date_to != NULL && disease != NULL){

      // }
      // else if(country == NULL && date_from != NULL && date_to != NULL && disease != NULL){

      // }
      // else if(country == NULL && date_from == NULL && date_to == NULL && disease != NULL){

      // }
      // else if(country != NULL && date_from == NULL && date_to == NULL && disease == NULL){

      // }else if(country != NULL && date_from == NULL && date_to == NULL && disease != NULL){

      // }else if(country != NULL && date_from != NULL && date_to == NULL && disease != NULL){

      // }else if(country == NULL && date_from != NULL && date_to != NULL && disease == NULL){

      // }else if(country != NULL && date_from != NULL && date_to == NULL && disease == NULL){

      // }else if(country != NULL && date_from == NULL && date_to == NULL && disease != NULL){

      // }else if(country != NULL && date_from == NULL && date_to != NULL && disease == NULL){

      // }else if(country == NULL && date_from != NULL && date_to == NULL && disease != NULL){

      // }else if(country == NULL && date_from != NULL && date_to == NULL && disease == NULL){

      // }else if(country == NULL && date_from == NULL && date_to != NULL && disease == NULL){

      // }



      return counter;
}

// Επιστρέφει τις k ασθένειες με τις περισσότερες εγγραφές που ικανοποιούν τo
// κριτήριο country (μπορεί να είναι NULL) _ταξινομημένες_ με βάση τον αριθμό
// εγγραφών (πρώτα η ασθένεια με τις περισσότερες).
//
// Πχ η dm_top_diseases(3, "Germany") επιστρέφει τις 3 ασθένειες με τις
// περισσότερες εγγραφές στη Γερμανία. Επιστρέφονται _μόνο_ ασθένειες με
// __τουλάχιστον 1 εγγραφή__ (που ικανοποιεί τα κριτήρια).

List dm_top_diseases(int k, String country){
      // int no_1 = 1;
       List list = list_create(NULL);
      // if(map_find(top_diseases,country) != NULL){
      //       Map based_on_disease = map_find(top_diseases,country);
            
      // }
      return list;
}

