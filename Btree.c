#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 4


typedef struct nodo {
  int vals[MAX];        // Array of values
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


void splitNode(Node **node){
  Node* p = *node;
  int median = p->vals[(int) floor(MAX/2)];

  printf("split con media %d", median);
  printf("\n");
  
  // We create a new root. 
  Node* right_son = newNode(median);

  // We save all the values;
  int i = floor(MAX/2) - 1;
  int k = 0;
  right_son->count = 0;
  for (i; i < MAX ; i++){
    if (p->vals[i] > median){
      right_son->vals[k] = p->vals[i];
      right_son->C[k] = p->C[i];
      p->vals[i] = 0;
      p->C[i] = (Node*) NULL;
      p->count--;
      right_son->count++;
      k++;
    } else if (p->vals[i] == median){
      p->vals[i] = 0;
      p->C[i] = (Node*) NULL;
      p->count--;
    }
  }

  // If we are the main root of the tree
  if (p->parent == NULL){
    Node* new_root = newNode(median);
    // We change the root of the tree.
    p->parent = new_root;
    right_son->parent = new_root;
    new_root->C[0] = p;
    new_root->C[1] = right_son;
    *node = new_root;
    return;
  }

  // If we are not the root, we are in the recursive case
  // We have to add the value into the parent
  Node* father = p->parent;
  int j = 0;
  while (father->vals[j] < median){
    j++;
  }

  // We save the value saved in the position of j, and
  // the right son of that value
  int e = father->vals[j];
  Node* aux = father->C[j+1];

  p->parent = father;           // p and right son have a new father
  right_son->parent = father;   
  father->vals[j] = median;     // the new value in j is the median
  father->C[j] = p;             // the left node is p
  father->C[j+1] = right_son;   // and the right is right_son
  father->count++;              // We increse the count of the father
  right_son = aux;              // we reload the values for the next
  median = e;                   // iterations
  j++;

  // We do the same on the next values of the node.
  while (j < father->count){
    e = father->vals[j];
    aux = father->C[j+1];
    right_son->parent = father;
    father->vals[j] = median;
    father->C[j+1] = right_son;
    right_son = aux;
    median = e;
    j++;
  }

  if (father->count == MAX){
    splitNode(&father->parent);
  }

  return;
}


void insert(Node **node, int val) {
  Node* p = *node;
  printf("insertamos %d", val);
  printf("\n");
  // If the root is NULL, we just create it
  if (p == NULL) {
    *node = newNode(val);
    return;

  // If the root is not null, we proceed
  } else {
    // We must look for the position where we must insert
    int i = 0;
    for (i; i < p->count; i++){
      if(p->vals[i] > val && p->C[i] != NULL){
        insert(&(p->C[i]), val);
        return;
      } else if (p->vals[i] > val && p->C[i] == NULL){
        break;
      }
    }
    
    // Si ya llegamos donde corresponde, entonces insertamos
    while(i <= p->count){
      int e = p->vals[i];
      p->vals[i] = val;
      val = e;
      i++;
    }
    p->count ++;

    if(p->count == MAX){
      splitNode(&p);
    }

    *node = p;
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
    printf("(");
    for (int i = 0; i<a->count ; i++)
      printf("%d,", a->vals[i]);
    printf(")");
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

  insert(&root, 15);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 5);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 2);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 1);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 4);
  printf("\n");
  inorden(&root);
  printf("\n");

  insert(&root, 3);
  printf("\n");
  inorden(&root);
  printf("\n");

  /* Constructing tree given in the above figure 

  insert(&root, 40);
  insert(&root, 70);

  
  printf("inserting: %d", val);
  printf("\n");

  
  if(a->parent != NULL){
      printf("my father : %d", a->parent->vals[0]);
      printf("\n");
    }

    
  printf("%d", root->parent->vals[0]);
  printf("\n");

  */
  return 0;
}