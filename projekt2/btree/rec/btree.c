/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bst_node_t *ptrNode = tree;

  if(ptrNode == NULL){
    return false;
  }else{
    if(ptrNode->key == key){
      (*value) = ptrNode->value;
      return true;
    }else{
      if(ptrNode->key > key) return bst_search(ptrNode->left, key, value);
      else return bst_search(ptrNode->right, key, value);
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *ptrNode = (*tree);

  if(ptrNode == NULL){
    bst_node_t *newNode = malloc(sizeof(struct bst_node));
    newNode->key = key;
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;

    if((*tree) == NULL){
      (*tree) = newNode;
      return;
    }else{
      ptrNode = newNode;
    }
  }else if(ptrNode->key == key){
    ptrNode->value = value;
  }else if(ptrNode->key > key){
    if(ptrNode->left == NULL){
      bst_node_t *newNode = malloc(sizeof(struct bst_node));
      newNode->key = key;
      newNode->value = value;
      newNode->left = NULL;
      newNode->right = NULL;
      ptrNode->left = newNode;
      return;
    }
    bst_insert(&ptrNode->left, key, value);
    return;
  }
  else{
    if(ptrNode->right == NULL){
      bst_node_t *newNode = malloc(sizeof(struct bst_node));
      newNode->key = key;
      newNode->value = value;
      newNode->left = NULL;
      newNode->right = NULL;
      ptrNode->right = newNode;
      return;
    }
    bst_insert(&ptrNode->right, key, value);
    return;
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *ptrNode = (*tree);

  if(ptrNode->right == NULL){
    target->key = ptrNode->key;
    target->value = ptrNode->value;

    if(ptrNode->left != NULL){
      target->left = ptrNode->left;
    }else{
      target->left = NULL;
    }
    free(ptrNode);
    return;

  }else if(ptrNode->right->right == NULL){
    target->key = ptrNode->right->key;
    target->value = ptrNode->right->value;

    if(ptrNode->right->left == NULL){
      free(ptrNode->right);
      ptrNode->right = NULL;
      return;
    }else{
      bst_node_t *ptrDel = ptrNode->right;
      ptrNode->right = ptrNode->right->left;
      free(ptrDel);
      return;
    }
  }

  bst_replace_by_rightmost(target,&ptrNode->right);

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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *ptrNode = (*tree);
  bst_node_t *ptrDel = NULL;

  if(ptrNode == NULL) return;
  
  if(ptrNode->key < key && ptrNode->right != NULL){ 
    if(ptrNode->right->key == key){
      if(ptrNode->right->right != NULL && ptrNode->right->left != NULL)
      {
        bst_replace_by_rightmost(ptrNode->right, &ptrNode->right->left);
        return;
      }else if(ptrNode->right->right != NULL){
        ptrDel = ptrNode->right;
        ptrNode->right = ptrNode->right->right;
      }
      else if(ptrNode->right->left != NULL){
        ptrDel = ptrNode->right;
        ptrNode->right = ptrNode->right->left;
      }else{
        ptrDel = ptrNode->right;
        ptrNode->right = NULL;
      }
      free(ptrDel);
      return;
    }else{
      bst_delete(&ptrNode->right,key);
      return;
    }
  }else if(ptrNode->key > key && ptrNode->left != NULL){
    if(ptrNode->left->key == key){
      if(ptrNode->left->right != NULL && ptrNode->left->left != NULL)
      {
        bst_replace_by_rightmost(ptrNode->left, &ptrNode->left->left);
        return;
      }else if(ptrNode->left->right != NULL){
        ptrDel = ptrNode->left;
        ptrNode->left = ptrNode->left->right;
      }
      else if(ptrNode->left->left != NULL){
        ptrDel = ptrNode->left;
        ptrNode->left = ptrNode->left->left;
      }else{
        ptrDel = ptrNode->left;
        ptrNode->left = NULL;
      }
      free(ptrDel);
      return;
    }else{
      bst_delete(&ptrNode->left,key);
      return;
    }
  }else if(ptrNode->key == key){
    if(ptrNode->right != NULL && ptrNode->left != NULL){
      bst_replace_by_rightmost(ptrNode, &ptrNode->left);
      return;
    }else if(ptrNode->right == NULL && ptrNode->left != NULL){
      ptrDel = ptrNode;
      ptrNode = ptrNode->left;
    }else if(ptrNode->right != NULL && ptrNode->left == NULL){
      ptrDel = ptrNode;
      ptrNode = ptrNode->right;
    }else{
      ptrDel = ptrNode;
      ptrNode = NULL;
    }
    free(ptrDel);
    return;
  }

  return;
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  bst_node_t *ptrNode = (*tree);

  if(ptrNode != NULL){
    bst_dispose(&ptrNode->right);
    bst_dispose(&ptrNode->left);
    free(ptrNode);
    (*tree) = NULL;
  }else{
    return;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  bst_node_t *ptrNode = tree;

  if(ptrNode != NULL){
    bst_print_node(ptrNode);
    bst_preorder(ptrNode->left);
    bst_preorder(ptrNode->right);

  }else{
    return;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  bst_node_t *ptrNode = tree;

  if(ptrNode != NULL){
    bst_inorder(ptrNode->left);
    bst_print_node(ptrNode);
    bst_inorder(ptrNode->right);
  }else{
    return;
  }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  bst_node_t *ptrNode = tree;

  if(ptrNode != NULL){
    bst_postorder(ptrNode->left);
    bst_postorder(ptrNode->right);
    bst_print_node(ptrNode);
  }else{
    return;
  }
}
