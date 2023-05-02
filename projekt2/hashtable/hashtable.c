/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  for(int i = 0; i < HT_SIZE; i++){
    (*table)[i] = NULL; 
  }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  
  if(table == NULL){
    return NULL;
  }else{
    int index = get_hash(key);
    ht_item_t *HTitem = (*table)[index];
    while (HTitem != NULL ){
      if (HTitem->key == key)
      {
        return HTitem;
      }
      HTitem = HTitem->next;
    }
  }
  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  ht_item_t *HTitem = ht_search(table, key);
  if(HTitem == NULL){
    ht_item_t *HTnew = malloc(sizeof(struct ht_item));
    if(HTnew == NULL){
      return;
    }
    HTnew->key = key;
    HTnew->value = value;

    int index = get_hash(key);
    HTnew->next = (*table)[index];
    (*table)[index] = HTnew;
  }else{
    HTitem->value = value;
  }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  if(ht_search(table,key) != NULL){
    return &(ht_search(table,key)->value);
  }else{
    return NULL;
  }
  
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  int index = get_hash(key);
  if((*table)[index] != NULL){
    ht_item_t *HTdel = (*table)[index];
    if(HTdel->key == key){
      (*table)[index] = HTdel->next;
    }else{
      while(HTdel->next != NULL){
        if(HTdel->next->key == key){
          ht_item_t *freeHT = HTdel->next;
          HTdel->next = HTdel->next->next;
          free(freeHT);
        }
        HTdel = HTdel->next;
      }
    }
  }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  ht_item_t *HTptr;
  ht_item_t *next;

  for(int i = 0; i < HT_SIZE; i++){
    if((*table)[i] != NULL){
      HTptr = (*table)[i];
      do{
			next = HTptr->next;
			free(HTptr);
			HTptr = next;
		  }while(HTptr != NULL);
      (*table)[i] = NULL;
    }
  }
}
