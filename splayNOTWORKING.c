#include<stdio.h>
#include<stdlib.h>

typedef struct node{

    int value;
    struct node* left;
    struct node* right;
    struct node* parent;
    
}Node;

Node* root;

void rightRotate(Node* p){
    Node* left = p->left;
    Node* right = p->right;
    Node* papa = p->parent;

    if(papa){
        if(papa->right == p){
            papa->right = left;
        }
        else{
            papa->left = left;
        }
    }
    if(right){
        right->parent=p;
    }
    left->parent=papa;
    left->right=p;

    p->parent=left;
    p->left=right;

}

void leftRotate(Node* p){
    Node* left = p->right;
    Node* right = p->left;
    Node* papa = p->parent;

    if(papa){
        if(papa->right == p){
            papa->right = left;
        }
        else{
            papa->left = left;
        }
    }
    if(right){
        right->parent=p;
    }
    left->parent=papa;
    left->left=p;

    p->parent=left;
    p->right=right;

}

void splay(Node* T){
    while(1){
        Node *p = T->parent;
        if(!p) break;
        Node *pp = p->parent;
        if(!pp){
            if(p->left==T){
                rightRotate(p);
            }
            else{
                leftRotate(p);
            }
            break;
        }
        if(pp->left==p){
            if(p->left==T){
                rightRotate(pp);
                rightRotate(p);
            }
            else{
                rightRotate(p);
                leftRotate(pp);
            }
        }
        else{
            if(p->left==T){
                rightRotate(p);
                leftRotate(pp);
            }
            else{
                leftRotate(pp);
                leftRotate(p);
            }
        }
    }
    root=T;
}

void insert(int val){
    if(!root){
        root = malloc(sizeof(Node));
        root->left=NULL;
        root->right=NULL;
        root->parent=NULL;
        root->value=val;
    }
    Node *p = root;
    while(1)
    {
        if(p->value==val)break;
        if(val < (p->value)){
            if(p->left){
                p=p->left;
            }
            else{
                p->left= malloc(sizeof(Node));
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
                p->right=malloc(sizeof(Node));
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

Node* search(int val){
    if(!root) return NULL;
    Node* p = root;
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

void inorder(Node* r){
    if(!r)return;
    inorder(r->left);
    printf("v: %d", r->value);
    if(r->left) printf("l: %d", r->left->value);
    if(r->right) printf("l: %d", r->right->value);
    puts("");
    inorder(r->right);
}


int main()
{
  /* Constructing tree given in the above figure */
  insert(10);
  insert(100);
  insert(20);
  insert(80);
  insert(120);
  
  insert(70);
  
  /* printf("\nImprimiendo\n");
  inorder(root);
  printf("\nBuscando el 1:\n");
  printf("%i", search(1)->value);
  printf("\nImprimiendo\n");
  inorder(root); */

  return 0;
}