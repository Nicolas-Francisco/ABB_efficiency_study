#include <stdio.h>
#include <stdlib.h>

#define MAX 4

typedef struct nodo {
  int vals[MAX-1];      // Array of values
  int count;            // value counter
  struct nodo *C[MAX];   // Array of node pointers
  struct nodo *parent;  // We keep trace of the parent node
}Node;

Node *newNode(int val) {
  Node *node;
  node = malloc(sizeof(Node));
  node->vals[0] = val;
  node->count = 1;
  return node;
}

void insert(Node **node, int val) {
  Node *root = *node;

  // If the root is NULL, we just create it
  if (root == NULL) {
    Node* root = newNode(val);

  // If the root is not null, we proceed
  } else {
    // We first look for the position that the value
    // must be in the B-Tree. This position will be 
    // saved in the pointer p.
    int i = 0;
    Node* p = root;
    Node* q = NULL;
    Node* last_parent = root;
    // While there are still values saved in the node,
    // and the pointer is not null
    while (i < p->count && p != NULL){
      // If the value is smaller, we go to the left son
      if (val < p->vals[i]){
        last_parent = p;
        p = p->C[i];
        i = 0;
      // If the value is the same, we founded it.
      } else if (p->vals[i] == val){
        break;
      }
      // Else, we keep looking in the node
      i++;
    }
    
    for (int e = val; p; p = p->parent) { 
      // If the leaf node is empty, add the element
      if (p == NULL) {
        p = newNode(e);
      }

      // If number of filled keys is less than maximum
      if (p->count < MAX - 1) {
        int i;
        for (i = 0; i < p->count; i++) {
          if (p->vals[i] > e) {
            for (int j = p->count - 1; j >= i; j--)
              p->vals[j + 1] = p->vals[j];
            break;
          }
        }
        p->vals[i] = e;
        p->count = p->count + 1;
        return;
      }
          
      // If number of filled keys is equal to maximum 
      // and it's not root and there is space in the parent
      if (p->count == MAX - 1 && p->parent && p->parent->count < MAX) {
        int m;
        for (int i = 0; i < p->parent->count; i++)
          if (p->parent->C[i] == p) {
            m = i;
            break;
          }
                  
        // If right sibling is possible
        if (m + 1 <= MAX - 1) {
          // q is the right sibling
          q = p->parent->C[m + 1];
  
          if (q) {  
            // If right sibling is full
            if (q->count == MAX - 1) {
              Node* r = newNode(val);
              int z[((2 * MAX) / 3)];
              int parent1, parent2;
              int marray[2 * MAX];
              int i;
              for (i = 0; i < p->count; i++)
                marray[i] = p->vals[i];
              int fege = i;
              marray[i] = e;
              marray[i + 1] = p->parent->vals[m];
              for (int j = i + 2; j < ((i + 2) + (q->count)); j++)
                marray[j] = q->vals[j - (i + 2)];

              // Put first (2*N-2)/3 elements into keys of p
              for (int i = 0; i < (2 * MAX - 2) / 3; i++)
                  p->vals[i] = marray[i];
              parent1 = marray[(2 * MAX - 2) / 3];

              // Put next (2*N-1)/3 elements into keys of q
              for (int j = ((2 * MAX - 2) / 3) + 1; j < (4 * MAX) / 3; j++)
                  q->vals[j - ((2 * MAX - 2) / 3 + 1)] = marray[j];
              parent2 = marray[(4 * MAX) / 3];

              // Put last (2*N)/3 elements into keys of r
              for (int f = ((4 * MAX) / 3 + 1); f < 2 * MAX; f++)
                r->vals[f - ((4 * MAX) / 3 + 1)] = marray[f];

              // Because m=0 and m=1 are children of the same key,
              // a special case is made for them
              if (m == 0 || m == 1) {
                p->parent->vals[0] = parent1;
                p->parent->vals[1] = parent2;
                p->parent->C[0] = p;
                p->parent->C[1] = q;
                p->parent->C[2] = r;
                return;
              } else {
                p->parent->vals[m - 1] = parent1;
                p->parent->vals[m] = parent2;
                p->parent->C[m - 1] = p;
                p->parent->C[m] = q;
                p->parent->C[m + 1] = r;
                return;
              }
            }
          } else {
            int put;
            if (m == 0 || m == 1)
              put = p->parent->vals[0];
            else
              put = p->parent->vals[m - 1];
            for (int j = (q->count) - 1; j >= 1; j--)
              q->vals[j + 1] = q->vals[j];
            q->vals[0] = put;
            p->parent->vals[m == 0 ? m : m - 1] = p->vals[p->count - 1];
          }
        }
      }
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
    for (int i = 0; i<a->count ; i++)
      inorden(&(a->C[i]));
  }
}

int main() {
  Node *root = NULL;
  /* Constructing tree given in the above figure */
  insert(&root, 10);
  insert(&root, 100);
  insert(&root, 20);
  insert(&root, 80);
  insert(&root, 40);
  insert(&root, 70);

  search(&root, 10);
  inorden(&root);
  return 0;
}