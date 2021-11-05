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
 

void insert(Node **node,long value){
    Node *a = *node;
    if (a==NULL){
        *node=newNode(value);
    }
    else if (value > a->value) {
        if (a->right == NULL) {
            a->right = newNode(value);
        } else {
            insert(&(a->right), value);
        }
    } else {
        if (a->left == NULL) {
            a->left = newNode(value);
        } else {
            insert(&(a->left), value);
        }
    }
}

Node * search(Node ** node, long value){
    Node *a = *node;
    if (a == NULL){
        return NULL;
    }else{
        if (value == a->value){
            return a;
        }else{
            if (value < a->value){
                return search(&(a->left), value);
            }else{
                return search(&(a->right), value);
            }
        }
    }
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

/*
int main() {
    printf("Hola Mundo");
    Node *root = NULL;
    insert(&root, 28);
    insert(&root, 11);
    insert(&root, 96);
    printf("\nImprimiendo preorden\n");
    preorden(&root);
    printf("\nImprimiendo inorden\n");
    inorden(&root);
    printf("\nImprimiendo postorden\n");
    postorden(&root);
    printf("\nSe busca el node numero 11\n");
    Node * node = search(&root,11);
    inorden(&node);
    printf("\n");
}
*/