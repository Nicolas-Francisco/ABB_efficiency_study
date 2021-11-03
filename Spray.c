#include<stdio.h>
#include<stdlib.h>



typedef struct node {
    int value;
    struct node *left;
    struct node *right;
}Node;

Node* newNode(int value){
    Node *node = malloc(sizeof(Node));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void *rightRotate(Node **node)
{
    Node *y = *node;
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    *node = x;
}
 
void leftRotate(Node **node)
{
    Node *x = *node;
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    *node = y;
}
void splay(Node **node, int value){
    Node *a=*node;
    if (a==NULL || a->value == value){
        return;
    }
    if (a->value > value){ //si deberia estar en el nodo izq
        if(a->left == NULL){ // si es vaci
            return;
        }
        if(a->left->value > value){
           splay(&(a->left->left), value);
           rightRotate(node);
        }
        else if (a->left->value < value){
            splay(&(a->left->right),value);
            if(a->left->right != NULL){
                leftRotate(&(a->left));
            }
        }
        if (a->left == NULL){
            return;
        }
        else{
            rightRotate(node);
            return;
        }

    }
    else{
        if(a->right ==NULL ){
            return;
        }
        if (a->right->value > value){
            splay(&(a->right->left),value);
            if(a->right->left != NULL){
                rightRotate(&(a->right));
            }
        }
        else if (a->right->value < value){
            splay(&(a->right->right),value);
            leftRotate(node);
        }
        
        if (a->right == NULL){
            return;
        }
        else{
            leftRotate(node);
        }
    }
}

void insert(Node** node, int value)
{
    Node *a = *node;
    if (a == NULL){
        *node=newNode(value);
        return;
    }
    if (value < a->value)
        insert(&(a->left), value);
    else if (value > a->value)
        insert(&(a->right), value);
    else 
        return;

    splay(node,value);
}



////////// Debuging //////////////////
void preorden(Node **node) {
    Node *a = *node;
    if (a != NULL) {
        printf("%d,", a->value);
        preorden(&(a->left));
        preorden(&(a->right));
    }
}
void inorden(Node **node) {
    Node *a = *node;
    if (a != NULL) {
        inorden(&(a->left));
        printf("%d,", a->value);
        inorden(&(a->right));
    }
}
void postorden(Node **node) {
    Node *a = *node;
    if (a != NULL) {
        postorden(&(a->left));
        postorden(&(a->right));
        printf("%d,", a->value);
    }
}
//////////////////////////////////////
 
/* Driver program to test above function*/
int main()
{
  Node *root = NULL;
  /* Constructing tree given in the above figure */
  insert(&root, 10);
  insert(&root, 100);
  insert(&root, 20);
  insert(&root, 80);
  insert(&root, 120);
  //insert(&root, 70);
  printf("\nImprimiendo\n");
  preorden(&root);

  return 0;
}