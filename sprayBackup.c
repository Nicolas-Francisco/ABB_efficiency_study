#include<stdio.h>
#include<stdlib.h>

typedef struct node{
	struct node *l;
	struct node *r;
	struct node *p;
	int v;
}Node;

Node *root;

void rightRotate(Node *P)
{
	Node *T=P->l;
	Node *B=T->r;
	Node *D=P->p;
	if(D)
	{
		if(D->r==P) D->r=T;
		else D->l=T;
	}
	if(B)
		B->p=P;
	T->p=D;
	T->r=P;
	
	P->p=T;
	P->l=B;
}
void leftRotate(Node *P)
{
	Node *T=P->r;
	Node *B=T->l;
	Node *D=P->p;
	if(D)
	{
		if(D->r==P) D->r=T;
		else D->l=T;
	}
	if(B)
		B->p=P;
	T->p=D;
	T->l=P;
	
	P->p=T;
	P->r=B;
}

void Splay(Node *T)
{
	while(1)
	{
		Node *p=T->p;
		if(!p) break;
		Node *pp=p->p;
		if(!pp)//Zig
		{
			if(p->l==T)
				rightRotate(p);
			else
				leftRotate(p);
			break;
		}
		if(pp->l==p)
		{
			if(p->l==T)
			{//ZigZig
				rightRotate(pp);
				rightRotate(p);
			}
			else
			{//ZigZag
				leftRotate(p);
				rightRotate(pp);
			}
		}
		else
		{
			if(p->l==T)
			{//ZigZag
				rightRotate(p);
				leftRotate(pp);
			}
			else
			{//ZigZig
				leftRotate(pp);
				leftRotate(p);
			}
		}
	}
	root=T;
}
void insert(int v)
{
	if(!root)
	{
		root=(Node *)malloc(sizeof(Node));
		root->l=NULL;
		root->r=NULL;
		root->p=NULL;
		root->v=v;
		return;
	}
	Node *P=root;
	while(1)
	{
		if(P->v==v) break; // not multiset
		if(v < (P->v) )
		{
			if(P->l) P=P->l;
			else
			{
				P->l=(Node *)malloc(sizeof(Node));
				P->l->p=P;
				P->l->r=NULL;
				P->l->l=NULL;
				P->l->v=v;
				P=P->l;
				break;
			}
		}
		else
		{
			if(P->r) P=P->r;
			else
			{
				P->r=(Node *)malloc(sizeof(Node));
				P->r->p=P;
				P->r->r=NULL;
				P->r->l=NULL;
				P->r->v=v;
				P=P->r;
				break;
			}
		}
	}
	Splay(P);
}

void preorder(Node *R)
{
	if(!R) return;
	printf("v: %d ",R->v);
	preorder(R->l);
	preorder(R->r);
}

Node* search(int v)
{
	if(!root) return NULL;
	Node *P=root;
	while(P)
	{
		if(P->v==v)
			break;
		if(v<(P->v))
		{
			if(P->l)
				P=P->l;
			else
				break;
		}
		else
		{
			if(P->r)
				P=P->r;
			else
				break;
		}
	}
	Splay(P);
	if(P->v==v) return P;
	else {
		return NULL;
	}
}

/* int main()
{
	
  insert(10);
  insert(100);
  insert(20);
  insert(80);
  insert(120);
  insert(70);
  preorder(root);

  printf("END OF FIRST EX \n");
  int value100 = search(100)->v;
  printf("FOUND %i \n", value100);
  Node* value0 = search(130);
  preorder(root);
  printf("FOUND %i \n", value0->v);
  preorder(root);
} */
