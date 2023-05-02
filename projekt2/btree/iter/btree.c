/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  (*tree) = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bst_node_t *ptrNode = tree;

  while (ptrNode != NULL)
  {
    if(ptrNode->key == key){
      (*value) = ptrNode->value;
      return true;
    }else if(key < ptrNode->key){
      ptrNode = ptrNode->left;
    }else{
      ptrNode = ptrNode->right;
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *ptrNode = (*tree);
  bst_node_t *newNode = malloc(sizeof(struct bst_node));
  newNode->key = key;
  newNode->value = value;
  newNode->left = NULL;
  newNode->right = NULL;

  if(ptrNode == NULL){  
    (*tree) = newNode;
  }else{
    while (ptrNode != NULL)
    {
      if(ptrNode->key == key){
        ptrNode->value = value;
        free(newNode);
        return;
      }else if(key < ptrNode->key){
        if(ptrNode->left != NULL){
          ptrNode = ptrNode->left;
        }else{
          ptrNode->left = newNode;
          return;
        }
      }else{
        if(ptrNode->right != NULL){
          ptrNode = ptrNode->right;
        }else{
          ptrNode->right = newNode;
          return;
        }
      }
    }
  }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *ptrNode = (*tree);
  bst_node_t *prevNode = NULL;
  while ( ptrNode != NULL)
  {
    if(ptrNode->right != NULL){
      prevNode = ptrNode;
      ptrNode = ptrNode->right;
    }else{
      target->key = ptrNode->key;
      target->value = ptrNode->value;
      if(prevNode == NULL) (*tree) = ptrNode->left;
      else prevNode->right = ptrNode->left;
      free(ptrNode);
      return;
    }
  }
  
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *ptrNode = (*tree);
  bst_node_t *prevNode = NULL;
  bool found = false;
  bool side = false; // left = false, right = true;
  
  if(ptrNode != NULL){
    while (ptrNode != NULL && !found)
    {
      if(ptrNode->key == key){
        found = true;
      }else if(key < ptrNode->key){
        side = false;
        prevNode = ptrNode;
        ptrNode = ptrNode->left;
      }else{
        side = true;
        prevNode = ptrNode;
        ptrNode = ptrNode->right;
      }
    }   

    if(!found) return;
    else if(ptrNode->right == NULL && ptrNode->left == NULL){
      if(prevNode == NULL) (*tree) = NULL;
      else{
        if(side) prevNode->right = NULL;
        else prevNode->left = NULL;
      }
    }else if(ptrNode->right == NULL && ptrNode->left != NULL){
      if(prevNode == NULL) (*tree) = NULL;
      else{
        if(side) prevNode->right = ptrNode->left;
        else prevNode->left = ptrNode->left;
      }
    }else if(ptrNode->right != NULL && ptrNode->left == NULL){
      if(prevNode == NULL) (*tree) = NULL;
      else{
        if(side) prevNode->right = ptrNode->right;
        else prevNode->left = ptrNode->right;
      }
    }else{
      bst_replace_by_rightmost(ptrNode,&(ptrNode->left));
      return;
    }

    free(ptrNode);
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_node_t *ptrNode = (*tree);
  bst_node_t *prevNode = NULL;
  while (ptrNode != NULL)
  {
    if(ptrNode->right != NULL) stack_bst_push(&stack, ptrNode->right);
    if(ptrNode->left != NULL){
      prevNode = ptrNode;
      ptrNode = ptrNode->left;
      free(prevNode);
    }else{
      free(ptrNode);
      if(stack_bst_top(&stack) != NULL){
        ptrNode=stack_bst_pop(&stack);
      }else{
        ptrNode=NULL;
        (*tree)=NULL;
      }
    }
  }
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *ptrNode = tree;

  while (ptrNode != NULL)
  {
    if(ptrNode->right != NULL) stack_bst_push(to_visit,ptrNode->right);
    bst_print_node(ptrNode);
    ptrNode = ptrNode->left;
  }
  
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_node_t *ptrNode = NULL;

  bst_leftmost_preorder(tree,&stack);
  if(stack_bst_top(&stack) != NULL) ptrNode = stack_bst_pop(&stack);
  
  while (ptrNode != NULL)
  {  
    bst_print_node(ptrNode);
    if(ptrNode->right != NULL) stack_bst_push(&stack,ptrNode->right);
    if(ptrNode->left != NULL) ptrNode=ptrNode->left;
    else if(stack_bst_top(&stack) != NULL) ptrNode = stack_bst_pop(&stack);
    else ptrNode = NULL;
  }
  
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  bst_node_t *ptrNode = tree;
  while (ptrNode != NULL)
  {
    stack_bst_push(to_visit,ptrNode);
    ptrNode = ptrNode->left;
  }
  
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_node_t *ptrNode = NULL;

  bst_leftmost_inorder(tree,&stack);
  if(stack_bst_top(&stack) != NULL) ptrNode = stack_bst_pop(&stack);

  while (ptrNode != NULL)
  {
    bst_print_node(ptrNode);
    if(ptrNode->right != NULL) bst_leftmost_inorder(ptrNode->right,&stack);
    if(stack_bst_top(&stack) != NULL) ptrNode = stack_bst_pop(&stack);
    else ptrNode = NULL;
  }
  
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
  bst_node_t *ptrNode = tree;

  while(ptrNode != NULL){
    stack_bst_push(to_visit,ptrNode);
    stack_bool_push(first_visit,true);

    ptrNode = ptrNode->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  stack_bst_t nStack;
  stack_bst_init(&nStack);
  stack_bool_t bStack;
  stack_bool_init(&bStack);
  bst_node_t *ptrNode = tree;
  bool secVisit = false;

  bst_leftmost_postorder(tree,&nStack,&bStack);

  while (stack_bst_top(&nStack) != NULL)
  {
      ptrNode = stack_bst_top(&nStack);
      secVisit = stack_bool_pop(&bStack);

    if(!secVisit){
      stack_bst_pop(&nStack);
      bst_print_node(ptrNode);
    }else{
      stack_bool_push(&bStack,false);
      bst_leftmost_postorder(ptrNode->right, &nStack, &bStack);
    }
  }
}
