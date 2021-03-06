#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXBt_16 16

typedef struct nodoBt_16 {
  long long vals[MAXBt_16];          // Array of values
  int count;              // value counter
  struct nodoBt_16 *C[MAXBt_16+1];  // Array of node pointers
  struct nodoBt_16 *parent;    // We keep trace of the parent node
}NodeBt_16;


NodeBt_16 *newNodeBt_16(long long val) {
  NodeBt_16 *node;
  node = malloc(sizeof(NodeBt_16));
  node->vals[0] = val;
  node->count = 1;
  return node;
}


void splitNodeBt_16(NodeBt_16 **node, NodeBt_16** root){
  NodeBt_16* p = *node;
  long long median = p->vals[(long long) floor(MAXBt_16/2)];
  // We create a new root. 
  NodeBt_16* right_son = newNodeBt_16(median);

  // We save all the values greater than the median
  // into a its new right node
  int i = floor(MAXBt_16/2);
  int k = 0;
  // For each position (except the last one)
  for (i; i < MAXBt_16 ; i++){
    // If the value stored in i is greater, we move the value and the children
    // to the next position
    if (p->vals[i] > median){
      right_son->vals[k] = p->vals[i];
      p->vals[i] = 0;
      right_son->C[k] = p->C[i];
      p->C[i] = (NodeBt_16*) NULL;
      p->count--;
      right_son->count++;
      k++;

    // If the value is the median, we just erase it
    } else if (p->vals[i] == median){
      p->vals[i] = NULL;
      p->count--;
    }
  }

  // We add the las son into the new right node
  right_son->count--;
  right_son->C[k] = p->C[MAXBt_16];
  p->C[MAXBt_16] = NULL;

  // If we are the main root of the tree
  if (p->parent == NULL){
    NodeBt_16* new_root = newNodeBt_16(median);
    // We change the root of the tree.
    new_root->C[0] = p;
    new_root->C[1] = right_son;
    p->parent = new_root;
    right_son->parent = new_root;
    *root = new_root;
    return;
  }

  // If we are not the root, we are in the recursive case
  // We have to add the value into the parent
  NodeBt_16* father = p->parent;
  NodeBt_16* grand = father->parent;
  int j = 0;

  while (j < father->count){
    if (father->vals[j] > median) break;
    j++;
  }

  int aux_j = j;
  // We do the same on the next values of the node.
  while (j <= father->count){
    long long e = father->vals[j];
    NodeBt_16* aux = father->C[j+1];
    right_son->parent = father;
    father->vals[j] = median;
    father->C[j+1] = right_son;
    right_son = aux;
    median = e;
    j++;
  }

  p->parent = father;
  father->C[aux_j] = p;
  father->count++;
  father->parent = grand;

  // If after adding the value into the father's node, the father
  // is now full, we must plit recursively.
  if (father->count == MAXBt_16){
    splitNodeBt_16(&father, root);
  }

  return;
}


void insert_NodeBt_16(NodeBt_16 **node, long long val, NodeBt_16 **root) {
  NodeBt_16* p = *node;
  // If the root is NULL, we just create it
  if (p == NULL) {
    *node = newNodeBt_16(val);
    return;

  // If the root is not null, we proceed
  } else {
    // We must look for the position where we must insert
    int i = 0;
    while(i < p->count){
      // If the value in i is greater than val, and there is a child
      // We insert recursively in that child
      if(p->vals[i] > val && p->C[i] != NULL){
        insert_NodeBt_16(&(p->C[i]), val, root);
        return;

      // If the value i is greater than val, and theres not a child, we found it.
      } else if (p->vals[i] > val && p->C[i] == NULL){
        break;
      }
      i++;
    }

    // If the last value is still smaller, and it theres a children on
    // its right, we go recursively in the last children
    if (p->vals[i] < val && p->C[i] != NULL){
      insert_NodeBt_16(&(p->C[i]), val, root);
      return;
    }
    
    // Now that we have the right position, we move all the values to
    // the right and insert the main val into the node
    while(i <= p->count){
      long long e = p->vals[i];
      p->vals[i] = val;
      val = e;
      i++;
    }
    p->count ++;

    // If the node is now full, we split it
    if(p->count == MAXBt_16){
      splitNodeBt_16(&p, root);
    }
  }
}


void insertBt_16(NodeBt_16 **node, long long val){
  // To avoid loosing trace of the main root, we just give it to the 
  // insert and split functions.
  insert_NodeBt_16(node, val, node);
}


NodeBt_16* searchBt_16(NodeBt_16 **node, long long val) {
  NodeBt_16 *a = *node;
  // If the node is empty, the value doest exist in the node
  if (a == NULL){
    return NULL;
  
  // If the node is not empty,
  } else{
    // We set a counter for the possible possition of the value
    // The first iteration will be with a value of 0
    int i = 0;
    while (i < a->count){
      // If the value stored in i is greater, we search recursively on the child
      if (val < a->vals[i]){
        return searchBt_16(&(a->C[i]), val);
      // if we founded the right value, we just return
      } else if (a->vals[i] == val){
        return a;
      }
      i++;
    }
    // We look in the last child
    return searchBt_16(&(a->C[i]), val);
  }
}




