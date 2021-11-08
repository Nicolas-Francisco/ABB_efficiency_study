// C program to insert a node in AVL tree
#include<stdio.h>
#include<stdlib.h>

typedef struct nodeAvl{
    long long value;
    struct nodeAvl *left;
    struct nodeAvl *right;
    int height;
}NodeAvl;
 
long long height(NodeAvl * node){
    if (node == NULL)
        return 0;
    return node->height;
}
 
long long max(long long a, long long b){
    return (a > b)? a : b;
}
 
NodeAvl* newNodeAvl(long long value){
    NodeAvl *node = malloc(sizeof(NodeAvl));
    node->value   = value;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;
    return node;
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
void *rightRotateAvl(NodeAvl **node)
{
    NodeAvl *y = *node;
    NodeAvl *x = y->left;
    NodeAvl *T2 = x->right;
 
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
void leftRotateAvl(NodeAvl **node)
{
    NodeAvl *x = *node;
    NodeAvl *y = x->right;
    NodeAvl *T2 = y->left;
 
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
int getBalance(NodeAvl *node)
{
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}
 
// Recursive function to insert a value in the subtree rooted
// with node and returns the new root of the subtree.
void insertAvl(NodeAvl** node, long long value)
{
    NodeAvl *a = *node;
    if (a == NULL){
        *node=newNodeAvl(value);
        return;
    }
    if (value < a->value)
        insertAvl(&(a->left), value);
    else if (value > a->value)
        insertAvl(&(a->right), value);
    else // Equal keys are not allowed in BST
        return;

    /* 2. Update height of this ancestor node */
    a->height = 1 + max(height(a->left),
                           height(a->right));
    

    //* 3. Get the balance factor of this ancestor
         // node to check whether this node became
        // unbalanced */
    int balance = getBalance(a);

     // Left Left Case
    if (balance > 1 && value < a->left->value){
        rightRotateAvl(node);
        return;
    }
    // Right Right Case
    if (balance < -1 && value > a->right->value){
        leftRotateAvl(node);
        return;
    }
 
    // Left Right Case
    if (balance > 1 && value > a->left->value){
        leftRotateAvl(&(a->left));
        rightRotateAvl(node);
        return;
    }
 
    // Right Left Case
    if (balance < -1 && value < a->right->value)
    {
        rightRotateAvl(&(a->right));
        leftRotateAvl(node);
    }
    return;
}


NodeAvl * searchAvl(NodeAvl ** node, long long value){
    NodeAvl *a = *node;
    if (a == NULL){
        return NULL;
    }else{
        if (value == a->value){
            return a;
        }else{
            if (value < a->value){
                return searchAvl(&(a->left), value);
            }else{
                return searchAvl(&(a->right), value);
            }
        }
    }
}
 
