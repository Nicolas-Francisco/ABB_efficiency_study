// C program to insert a node in AVL tree
#include<stdio.h>
#include<stdlib.h>

typedef struct node{
    int value;
    struct node *left;
    struct node *right;
    int height;
}Node;
 
int height(Node * node){
    if (node == NULL)
        return 0;
    return node->height;
}
 
int max(int a, int b){
    return (a > b)? a : b;
}
 
Node* newNode(int value){
    Node *node = malloc(sizeof(Node));
    node->value   = value;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;
    return node;
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
Node *rightRotate(Node **node)
{
    Node *y = *node;
    Node *x = y->left;
    Node *T2 = x->right;
 
    // Perform rotation
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
 
    // Return new root
    *node = x;
}
 
// A utility function to left rotate subtree rooted with x
// See the diagram given above.
void leftRotate(Node **node)
{
    Node *x = *node;
    Node *y = x->right;
    Node *T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    //  Update heights
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
 
    // Return new root
    *node = y;
}
 
// Get Balance factor of node N
int getBalance(Node *node)
{
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}
 
// Recursive function to insert a value in the subtree rooted
// with node and returns the new root of the subtree.
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
    else // Equal keys are not allowed in BST
        return;

    /* 2. Update height of this ancestor node */
    a->height = 1 + max(height(a->left),
                           height(a->right));
    

    //* 3. Get the balance factor of this ancestor
         // node to check whether this node became
        // unbalanced */
    int balance = getBalance(a);

    //printf("\nla altura es %d \n",balance);

    // Left Left Case
    if (balance > 1 && value < a->left->value){
        rightRotate(node);
        return;
    }

     // Left Left Case
    if (balance > 1 && value < a->left->value){
        rightRotate(node);
                return;
    }
    // Right Right Case
    if (balance < -1 && value > a->right->value){
        leftRotate(node);
                return;
    }
 
    // Left Right Case
    if (balance > 1 && value > a->left->value){
        leftRotate(&(a->left));
        rightRotate(node);
                return;
    }
 
    // Right Left Case
    if (balance < -1 && value < a->right->value)
    {
        rightRotate(&(a->right));
        leftRotate(node);
    }
    return;
}
 
// A utility function to print preorder traversal
// of the tree.
// The function also prints height of every node
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
  insert(&root, 40);
  insert(&root, 70);
  printf("\nImprimiendo\n");
  preorden(&root);

  return 0;
}