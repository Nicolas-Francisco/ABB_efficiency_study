#include<stdio.h>
#include<stdlib.h>

typedef struct nodeSplay{

    int value;
    struct nodeSplay* left;
    struct nodeSplay* right;
    struct nodeSplay* parent;
    
}NodeSplay;

NodeSplay* rootSplay;

void rightRotateSplay(NodeSplay* p){
    NodeSplay* L = p->left;
    NodeSplay* R = L->right;
    NodeSplay* papa = p->parent;

    if(papa){
        if(papa->right == p){
            papa->right = L;
        }
        else{
            papa->left = L;
        }
    }
    if(R){
        R->parent=p;
    }
    L->parent=papa;
    L->right=p;

    p->parent=L;
    p->left=R;

}

void leftRotateSplay(NodeSplay* p){
    NodeSplay* L = p->right;
    NodeSplay* R = L->left;
    NodeSplay* papa = p->parent;

    if(papa){
        if(papa->right == p){
            papa->right = L;
        }
        else{
            papa->left = L;
        }
    }
    if(R){
        R->parent=p;
    }
    L->parent=papa;
    L->left=p;

    p->parent=L;
    p->right=R;

}

void splay(NodeSplay* T){
    while(1){
        NodeSplay *p = T->parent;
        if(!p) break;
        NodeSplay *pp = p->parent;
        if(!pp){
            if(p->left==T){
                rightRotateSplay(p);
            }
            else{
                leftRotateSplay(p);
            }
            break;
        }
        if(pp->left==p){
            if(p->left==T){
                rightRotateSplay(pp);
                rightRotateSplay(p);
            }
            else{
                leftRotateSplay(p);
                rightRotateSplay(pp);
            }
        }
        else{
            if(p->left==T){
                rightRotateSplay(p);
                leftRotateSplay(pp);
            }
            else{
                leftRotateSplay(pp);
                leftRotateSplay(p);
            }
        }
    }
    rootSplay=T;
}

void insertSplay(int val){
    if(!rootSplay){
        rootSplay = malloc(sizeof(rootSplay));
        rootSplay->left=NULL;
        rootSplay->right=NULL;
        rootSplay->parent=NULL;
        rootSplay->value=val;
    }
    NodeSplay *p = rootSplay;
    while(1)
    {
        if(p->value==val)break;
        if(val < (p->value)){
            if(p->left){
                p=p->left;
            }
            else{
                p->left= malloc(sizeof(rootSplay));
                p->left->parent=p;
                p->left->right=NULL;
                p->left->left=NULL;
                p->left->value=val;
                p=p->left;
                break;
            }
        }
        else{
            if(p->right)p=p->right;
            else{
                p->right=malloc(sizeof(rootSplay));
                p->right->parent=p;
                p->right->right=NULL;
                p->right->left=NULL;
                p->right->value=val;
                p=p->right;
                break;
            }
        }
    }
    splay(p);
}

NodeSplay* searchSplay(int val){
    if(!rootSplay) return NULL;
    NodeSplay* p = rootSplay;
    while(p){
        if(p->value==val)break;
        if(val<(p->value)){
            if(p->left) p=p->left;
            else break;
        }
        else{
            if(p->right)p=p->right;
            else break;
        }
    }
    splay(p);
    if(p->value==val) return p;
    else return NULL;
}

