#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 4

typedef struct nodo {
  int vals[MAX-1];      // Array of values
  int count;            // value counter
  struct nodo *C[MAX];  // Array of node pointers
  struct nodo *parent;  // We keep trace of the parent node
}Node;

Node *newNode(int val) {
  Node *node;
  node = malloc(sizeof(Node));
  node->vals[0] = val;
  node->count = 1;
  return node;
}


void splitNode(Node **node, int median){
  printf("spliting: %d", median);
  Node* p = *node;
  // If we are the main root of the tree
  if (p->parent == NULL){

    // We create a new root. 
    Node* new_root = newNode(median);
    Node* right_son = newNode(median);

    // We save al the values;
    int i = floor(MAX/2) - 1;
    int k = 0;
    right_son->count = 0;
    for (i; i<=MAX ; i++){
      if (p->vals[i] > median){
        right_son->vals[k] = p->vals[i];
        right_son->C[k] = p->C[i];
        p->vals[i] = 0;
        p->C[i] = (Node*) NULL;
        p->count--;
        right_son->count++;
        k++;
      }
    }

    // We change the root of the tree.
    node = &new_root;
    (*node)->C[0] = p;
    (*node)->C[1] = right_son;
    p->parent = *node;
    right_son->parent = *node;
    return;
  }
  // If we are not the root, we are in the recursive case
}


void insert(Node **node, int val) {
  Node *root = *node;
  
  // If the root is NULL, we just create it
  if (root == NULL) {
    *node = newNode(val);
    return;

  // If the root is not null, we proceed
  } else {
    // We first look for the position that the value
    // must be in the B-Tree. This position will be 
    // saved in the pointer p.
    int i = 0;
    Node* p = root;
    // While there are still values saved in the node,
    // and the pointer is not null
    while (i < p->count && p != NULL){
      // If the value is smaller, we go to the left son
      if (val < p->vals[i]){
        if (p->C[i] == NULL){
          break;
        } else{
          p = p->C[i];
          i = 0;
        }
      // If the value is the same, we founded it.
      } else if (p->vals[i] == val){
        break;
      }
      // Else, we keep looking in the node
      i++;
    }
    
    // If there is space in the parent, we insert it.
    if (p->count < MAX - 1){
      while(i <= p->count){
        int e = p->vals[i];
        p->vals[i] = val;
        val = e;
        i++;
      }
      p->count ++;
      return;
    } 
    // If not, we must split the node and go recursively
    else {
      int median = p->vals[(int) floor(MAX/2)];
      if (i == floor(MAX/2)){
        median = val;
      } else if (i < (int) floor(MAX/2)){
        median = p->vals[(int) floor(MAX/2) - 1];
      }

      splitNode(&p, median);
    }
  }
}


Node* search(Node **node, int val) {
  Node *a = *node;
  if (a == NULL){
    return NULL;
  } else{
    // We set a counter for the possible possition of the value
    // The first iteration will be with a value of 0
    int i = 0;
    while (i < a->count){
      if (val < a->vals[i]){
        return search(&(a->C[i]), val);
      } else if (a->vals[i] == val){
        return a;
      }
      i++;
    }
    return search(&(a->C[i]), val);
  }
}

void inorden(Node **node) {
  Node *a = *node;
  if (a != NULL) {
    for (int i = 0; i<a->count ; i++)
      printf("%d,", a->vals[i]);
      printf("\n");
    int i = 0;
    while (a->C[i] != NULL){
      inorden(&(a->C[i]));
      i++;
    }
  }
}

int main() {
  Node *root = NULL;

  insert(&root, 10);
  insert(&root, 100);
  insert(&root, 20);
  insert(&root, 60);

  inorden(&root);

  printf("%d", root->vals[0]);
  printf("\n");

  /* Constructing tree given in the above figure 

  insert(&root, 40);
  insert(&root, 70);

  
  printf("inserting: %d", val);
  printf("\n");

  */
  return 0;
}