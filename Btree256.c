#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXBt_256 256

typedef struct nodoBt_256 {
  long vals[MAXBt_256];          // Array of values
  int count;              // value counter
  struct nodoBt_256 *C[MAXBt_256+1];  // Array of node pointers
  struct nodoBt_256 *parent;    // We keep trace of the parent node
}NodeBt_256;


NodeBt_256 *newNodeBt_256(long val) {
  NodeBt_256 *node;
  node = malloc(sizeof(NodeBt_256));
  node->vals[0] = val;
  node->count = 1;
  return node;
}


void splitNodeBt_256(NodeBt_256 **node, NodeBt_256** root){
  NodeBt_256* p = *node;
  int median = p->vals[(int) floor(MAXBt_256/2)];
  // We create a new root. 
  NodeBt_256* right_son = newNodeBt_256(median);

  // We save all the values greater than the median
  // into a its new right node
  int i = floor(MAXBt_256/2);
  int k = 0;
  // For each position (except the last one)
  for (i; i < MAXBt_256 ; i++){
    // If the value stored in i is greater, we move the value and the children
    // to the next position
    if (p->vals[i] > median){
      right_son->vals[k] = p->vals[i];
      p->vals[i] = NULL;
      right_son->C[k] = p->C[i];
      p->C[i] = (NodeBt_256*) NULL;
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
  right_son->C[k] = p->C[MAXBt_256];
  p->C[MAXBt_256] = NULL;

  // If we are the main root of the tree
  if (p->parent == NULL){
    NodeBt_256* new_root = newNodeBt_256(median);
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
  NodeBt_256* father = p->parent;
  NodeBt_256* grand = father->parent;
  int j = 0;

  while (j < father->count){
    if (father->vals[j] > median) break;
    j++;
  }

  int aux_j = j;
  // We do the same on the next values of the node.
  while (j <= father->count){
    int e = father->vals[j];
    NodeBt_256* aux = father->C[j+1];
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
  if (father->count == MAXBt_256){
    splitNodeBt_256(&father, root);
  }

  return;
}


void insert_NodeBt_256(NodeBt_256 **node, long val, NodeBt_256 **root) {
  NodeBt_256* p = *node;
  // If the root is NULL, we just create it
  if (p == NULL) {
    *node = newNodeBt_256(val);
    return;

  // If the root is not null, we proceed
  } else {
    // We must look for the position where we must insert
    int i = 0;
    while(i < p->count){
      // If the value in i is greater than val, and there is a child
      // We insert recursively in that child
      if(p->vals[i] > val && p->C[i] != NULL){
        insert_NodeBt_256(&(p->C[i]), val, root);
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
      insert_NodeBt_256(&(p->C[i]), val, root);
      return;
    }
    
    // Now that we have the right position, we move all the values to
    // the right and insert the main val into the node
    while(i <= p->count){
      int e = p->vals[i];
      p->vals[i] = val;
      val = e;
      i++;
    }
    p->count ++;

    // If the node is now full, we split it
    if(p->count == MAXBt_256){
      splitNodeBt_256(&p, root);
    }
  }
}


void insertBt_256(NodeBt_256 **node, int val){
  // To avoid loosing trace of the main root, we just give it to the 
  // insert and split functions.
  insert_NodeBt_256(node, val, node);
}


NodeBt_256* searchBt_256(NodeBt_256 **node, int val) {
  NodeBt_256 *a = *node;
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
        return searchBt_256(&(a->C[i]), val);
      // if we founded the right value, we just return
      } else if (a->vals[i] == val){
        return a;
      }
      i++;
    }
    // We look in the last child
    return searchBt_256(&(a->C[i]), val);
  }
}
