#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 16

typedef struct nodo {
  int vals[MAX];          // Array of values
  int count;              // value counter
  struct nodo *C[MAX+1];  // Array of node pointers
  struct nodo *parent;    // We keep trace of the parent node
}Node;


Node *newNode(int val) {
  Node *node;
  node = malloc(sizeof(Node));
  node->vals[0] = val;
  node->count = 1;
  return node;
}


void splitNode(Node **node, Node** root){
  Node* p = *node;
  int median = p->vals[(int) floor(MAX/2)];
  // We create a new root. 
  Node* right_son = newNode(median);

  // We save all the values greater than the median
  // into a its new right node
  int i = floor(MAX/2);
  int k = 0;
  // For each position (except the last one)
  for (i; i < MAX ; i++){
    // If the value stored in i is greater, we move the value and the children
    // to the next position
    if (p->vals[i] > median){
      right_son->vals[k] = p->vals[i];
      p->vals[i] = NULL;
      right_son->C[k] = p->C[i];
      p->C[i] = (Node*) NULL;
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
  right_son->C[k] = p->C[MAX];
  p->C[MAX] = NULL;

  // If we are the main root of the tree
  if (p->parent == NULL){
    Node* new_root = newNode(median);
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
  Node* father = p->parent;
  Node* grand = father->parent;
  int j = 0;

  while (j < father->count){
    if (father->vals[j] > median) break;
    j++;
  }

  int aux_j = j;
  // We do the same on the next values of the node.
  while (j <= father->count){
    int e = father->vals[j];
    Node* aux = father->C[j+1];
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
  if (father->count == MAX){
    splitNode(&father, root);
  }

  return;
}


void insert_Node(Node **node, int val, Node **root) {
  Node* p = *node;
  // If the root is NULL, we just create it
  if (p == NULL) {
    *node = newNode(val);
    return;

  // If the root is not null, we proceed
  } else {
    // We must look for the position where we must insert
    int i = 0;
    while(i < p->count){
      // If the value in i is greater than val, and there is a child
      // We insert recursively in that child
      if(p->vals[i] > val && p->C[i] != NULL){
        insert_Node(&(p->C[i]), val, root);
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
      insert_Node(&(p->C[i]), val, root);
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
    if(p->count == MAX){
      splitNode(&p, root);
    }
  }
}


void insertBT(Node **node, int val){
  // To avoid loosing trace of the main root, we just give it to the 
  // insert and split functions.
  insert_Node(node, val, node);
}


Node* searchBT(Node **node, int val) {
  Node *a = *node;
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
        return search(&(a->C[i]), val);
      // if we founded the right value, we just return
      } else if (a->vals[i] == val){
        return a;
      }
      i++;
    }
    // We look in the last child
    return search(&(a->C[i]), val);
  }
}


void inorden(Node **node) {
  Node *a = *node;
  if (a != NULL) {
    printf("[");
    for (int i = 0; i<a->count ; i++)
      printf("%d ", a->vals[i]);
    printf("]");
    printf("\n");
    int i = 0;
    printf("(");
    while (a->C[i] != NULL){
      inorden(&(a->C[i]));
      i++;
    }
    printf(")");
  }
}


int main() {
  // We test the Btree using a series of insertions.
  Node *root = NULL;

  insert(&root, 10);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 60);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 20);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 50);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 65);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 70);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 80);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 90);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 100);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 110);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 120);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 130);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 140);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 85);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 95);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 98);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 97);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 99);
  printf("\n");
  inorden(&root);
  printf("\n");
  return 0;
}