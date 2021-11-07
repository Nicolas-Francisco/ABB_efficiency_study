#include<stdio.h>
#include<stdlib.h>

typedef struct nodeAbb {
    long value;
    struct nodeAbb *left;
    struct nodeAbb *right;
}NodeAbb;

NodeAbb* newNodeAbb(long value){
    NodeAbb *node = malloc(sizeof(NodeAbb));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}
 

void insertABB(NodeAbb **node,long value){
    NodeAbb *a = *node;
    if (a==NULL){
        *node=newNodeAbb(value);
    }
    else if (value > a->value) {
        if (a->right == NULL) {
            a->right = newNodeAbb(value);
        } else {
            insertABB(&(a->right), value);
        }
    } else {
        if (a->left == NULL) {
            a->left = newNodeAbb(value);
        } else {
            insertABB(&(a->left), value);
        }
    }
}

NodeAbb * searchABB(NodeAbb ** node, long value){
    NodeAbb *a = *node;
    if (a == NULL){
        return NULL;
    }else{
        if (value == a->value){
            return a;
        }else{
            if (value < a->value){
                return searchABB(&(a->left), value);
            }else{
                return searchABB(&(a->right), value);
            }
        }
    }
}
