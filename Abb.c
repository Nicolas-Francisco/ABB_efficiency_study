#include<stdio.h>
#include<stdlib.h>

typedef struct nodo {
    int info;
    struct nodo *izq;
    struct nodo *der;
}Nodo;

typedef Nodo *Arbol;

void insertar(Nodo **nodo,int info)
{
    Nodo *a = *nodo;
    if (a==NULL){
        Nodo *p = malloc(sizeof(Nodo));
        p->info = info;
        p->izq = NULL;
        p->der = NULL;
        *nodo=p;
    }
    else if (info > a->info) {
        if (a->der == NULL) {
            Nodo *p = malloc(sizeof(Nodo));
            p->info = info;
            p->izq = NULL;
            p->der = NULL;
            a->der = p;
        } else {
            insertar(&(a->der), info);
        }
    } else {
        if (a->izq == NULL) {
            Nodo *p = malloc(sizeof(Nodo));
            p->info = info;
            p->izq = NULL;
            p->der = NULL;
            a->izq = p;
        } else {
            insertar(&(a->izq), info);
        }
    }
}

void preorden(Nodo **nodo) {
    Nodo *a = *nodo;
    if (a != NULL) {
        printf("%d,", a->info);
        preorden(&(a->izq));
        preorden(&(a->der));
    }
}


void inorden(Nodo **nodo) {
    Nodo *a = *nodo;
    if (a != NULL) {
        inorden(&(a->izq));
        printf("%d,", a->info);
        inorden(&(a->der));
    }
}


void postorden(Nodo **nodo) {
    Nodo *a = *nodo;
    if (a != NULL) {
        postorden(&(a->izq));
        postorden(&(a->der));
        printf("%d,", a->info);
    }
}

Nodo * buscar (Nodo ** nodo, int info){
    Nodo *a = *nodo;
    if (a == NULL){
        return NULL;
    }else{
        if (info == a->info){
            return a;
        }else{
            if (info < a->info){
                return buscar (&(a->izq), info);
            }else{
                return buscar (&(a->der), info);
            }
        }
    }
}

int main() {
    printf("Hola Mundo");
    Nodo *raiz = NULL;
    insertar(&raiz, 28);
    insertar(&raiz, 11);
    insertar(&raiz, 96);
    printf("\nImprimiendo preorden\n");
    preorden(&raiz);
    printf("\nImprimiendo inorden\n");
    inorden(&raiz);
    printf("\nImprimiendo postorden\n");
    postorden(&raiz);
    printf("\nSe busca el nodo numero 11\n");
    Nodo * nodo = buscar(&raiz,11);
    inorden(&nodo);
    printf("\n");
}
