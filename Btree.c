#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 4

typedef struct nodo {
  int vals[MAX-1];        // Array of values
  int count;            // value counter
  struct nodo *C[MAX];  // Array of node pointers
  struct nodo *parent;  // We keep trace of the parent node
}Node;


Node *newNode(int val) {
  Node *node;
  node = malloc(sizeof(Node));
  node->vals[0] = val;
  node->count = 1;
  node->parent = NULL;
  return node;
}


void parent_insert(Node **node, int x, Node **left, Node **right, Node **root){
  Node *p = *node;

  // If theres space in the parent node
  if (p->count < MAX-1){
    
    int j = p->count-1; 
    // We move all the values to the next space
    while((p->vals)[j] >= x){
      (p->vals)[j+1] = (p->vals)[j]; 
      (p->C)[j+2] = (p->C)[j+1]; 
      j--;
      if (j < 0) {
        break;
      } 
    }

    // Then we insert the element in the position left null
    (p->vals)[j+1] = x;
    (p->C)[j+1] = *left;
    (p->C)[j+2] = *right;
    Node* left_node = *left;
    Node* right_node = *right;
    (left_node->parent) = p;
    (right_node->parent) = p;
    (p->count)++;
  }

  // If theres not space in the node, we must split the parent
  else {
    parent_split(node, x, left, right, root);
  }
}


void parent_split(Node **node, int x, Node **left, Node **right, Node **root){
  Node *p = *node;

  // We calculate the median value without inserting it
  int median = (p->vals)[MAX/2];
  int to_left = 0;
  // If the element is to the left of the median
  if (x < median){
    // We move the median to its left
    median = (p->vals)[(MAX/2)-1];
    to_left = 1;
    if (x > median){
      median = x;
      to_left = -1;
    }
  }

  // On each iteration we must create a new right node
  Node *new_right = newNode(0);
  int i = 0;
  int j = 0;

  while (i < MAX-1){
    // If the value is greater than the media, we move it to the 
    // new right node
    if ((p->vals)[i] > median){
      (new_right->vals)[j] = (p->vals)[i];
      (new_right->C)[j] = (p->C)[i];
      (p->C)[i] = NULL;
      (new_right->count)++;
      j++;
    } else if ((p->vals)[i] == median){
      (p->vals)[i] = NULL;
      (p->count)--;
    }
    i++;
  }

  // We set the correct values to the new right node
  (new_right->count)--;
  (new_right->C)[j] = (p->C)[i];

  // We insert the median to it correct position in the parent
  if (to_left > -1){
    if (to_left){
      parent_insert(&p, x, left, right, root);
    } else {
      parent_insert(&new_right, x, left, right, root);
    }
  }

  // If the new value is the median
  else{
    // We only need to insert its children
    (p->C)[new_right->count] = *left;
    (new_right->C)[0] = *right;
  }

  // If we are in the root node of the B tree, we just create
  // a new root, and place the trees as its new children
  if (p->parent == NULL){
    Node *new_parent = newNode(median);
    (new_parent->C)[0] = p;
    (new_parent->C)[1] = new_right;
    p->parent = new_parent;
    new_right->parent = new_parent;

    *root = new_parent;
  } 

  // If we are not the root, we just go up recursively
  else{
    new_right->parent = p->parent;
    parent_insert(&(p->parent), median, &p, &new_right, root);
  }
  
  return;
}

// The function insert just calls the insert to a leaf.
// We only can insert a new value into a leaf, never a intern node (this is done
// by the function parent insert)
void insert(Node **node, int x){
  leaf_insert(node, node, x);
}


void leaf_insert(Node **node, Node **root, int x) {
  Node *p = *node;

  // If the node has a chldren
  if ((p->C)[0] != NULL){
    // We look for the position that we must insert the value x
    int i = 0;
    while((p->vals)[i] < x){
      i++;
      if (i == p->count) {
        break;
      } 
    }

    // Once we reached the expected posittion, we insert the value recursively.
    leaf_insert(&(p->C)[i], root, x);
  }

  // If we are in a leaf without children
  else{
    // We must move the values that are greater than the x
    int j = p->count-1;
    while((p->vals)[j] > x){
      (p->vals)[j+1] = (p->vals)[j]; 
      j--;
      if (j < 0) {
        break;
      } 
    }

    // We insert the new value into the leaf
    (p->vals)[j+1] = x;
    (p->count)++;

    // If the node is full, we must split the leaf.
    if (p->count >= MAX){ 
      leaf_split(node, root);
    }
  }
  return;
}


void leaf_split(Node **node, Node **root){
  Node *p = *node;

  int median = (p->vals)[MAX/2];

  // As the parent_split, we always create a new right node
  Node *right = newNode(0);
  int i = 0;
  int j = 0;

  // We move the values to the right
  while (i < MAX){ 
    if ((p->vals)[i] > median){
      (right->vals)[j] = (p->vals)[i];
      (right->count)++;
      (p->vals)[i] = NULL;
      (p->count)--;
      j++;
    } 
    i++;
  }

  (right->count)--;

  // If we are the root, we create a new root.
  if (p->parent == NULL){
    Node *new_parent = newNode(median);
    (new_parent->C)[0] = p;
    (new_parent->C)[1] = right;
    p->parent = new_parent;
    right->parent = new_parent;
    *root = new_parent;
    return;
  }

  // If we are not, we must split recursively
  else{
    right->parent = p->parent;
    parent_insert(&(p->parent), median, &p, &right, root);
  }
}

Node* search(Node **root, int x){
  Node *p = *root;

  // If the node doesnt have children, we are in a leaf
  if ((p->C)[0] == NULL){
    // We look for the value in the node 
    int i = 0;
    while(i < MAX-1){ 
      // If we didn't found it, we get out and return null.
      if ((p->vals)[i] == NULL) {
        break;
      }
      // If we founded it, we return it
      if ((p->vals)[i] == x){ 
        return p;
      }
      i++;
    }
    return NULL;
  }

  // If we are not in a leaf, we must look for the children that
  // should contain the value
  else{ 
    int i = 0;
    while(i < MAX-1){
      // If the value in the position i is greater than x, we search
      // recursively on its children
      if ((p->vals)[i] >= x || (p->vals)[i] == NULL){ 
        return search(&(p->C)[i], x);            
      }
      i++;
    }
    // If the node was almost full, we look in the last children.
    return search(&(p->C)[i], x);
  }
}

void print_leaves(Node *p){
  if (p->C[0] != NULL){
    for(int i = 0; i < (p->count)+1; i++){
      print_leaves(p->C[i]);
    }
  }
  else{
    for(int i = 0; i < p->count; i++){
      printf("%d ", p->vals[i]);
    }
  }
}

void free_nodes(Node *p) {
  int i = 0;
  while(p->C[i] != NULL){
    free_nodes(p->C[i]);
  }
  free(p);
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
  Node *root = newNode(10);
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
  return 0;
}