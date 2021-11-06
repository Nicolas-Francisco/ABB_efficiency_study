#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 4


typedef struct nodo {
  int vals[MAX];        // Array of values
  int count;            // value counter
  struct nodo *C[MAX+1];  // Array of node pointers
  struct nodo *parent;  // We keep trace of the parent node
}Node;


Node *newNode(int val) {
  Node *node;
  node = malloc(sizeof(Node));
  node->vals[0] = val;
  node->count = 1;
  return node;
}


Node* splitNode(Node **node){
  Node* p = *node;
  int median = p->vals[(int) floor(MAX/2)];
  // We create a new root. 
  Node* right_son = newNode(median);

  printf("split with %d", median);
  printf("\n");

  if(p->parent != NULL){
    printf("my father : %d ", p->parent->vals[0]);
    printf("\n");
  }

  // We save all the values;
  int i = floor(MAX/2);
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
    Node* my_new_right = right_son;
    Node* my_new_left = p;
    // We change the root of the tree.
    my_new_left->parent = new_root;
    my_new_right->parent = new_root;
    new_root->C[0] = my_new_left;
    new_root->C[1] = my_new_right;
    printf("new father: %d", new_root->vals[0]);
    return new_root;
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

  if (father->count == MAX){
    father = splitNode(&father);
  }

  return p;
}


void insert(Node **node, int val) {
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
      if(p->vals[i] > val && p->C[i] != NULL){
        insert(&(p->C[i]), val);
        return;
      } else if (p->vals[i] > val && p->C[i] == NULL){
        break;
      }
      i++;
    }

    if (p->vals[i] < val && p->C[i] != NULL){
      insert(&(p->C[i]), val);
      return;
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
      p = splitNode(&p);
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
    for (int i = 0; i<a->count ; i++)
      printf("%d ", a->vals[i]);
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