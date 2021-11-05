#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define in 1; //insercion
#define be 2; //busqueda exitosa
#define bi 3; //busqueda incorrecta

#define n  10


#define pi 0.5
#define pbe 0.33
#define pbi 0.17

//float pi = 0.5;
//float pbe = 0.33;
//float pbi = 0.17;


int secuencia[n];

//genera la secuencia desordena de intrucciones
void initSecuencia(){
    int cant_in=round(n*pi);
    int cant_be=round(n*pbe);
    int cant_bi=round(n*pbi);
    printf("la cantidad de insercciones sera %d\n",cant_in);
    printf("la cantidad de busquedas exitosas sera %d\n",cant_be);
    printf("la cantidad de busquedas incorrectas sera %d\n",cant_bi);

    int secuencia_ordenado[n];

    for (int i=0; i < cant_in; i++) // pongo las inserciones de 0 a cant_in
        secuencia_ordenado[i]=in;

    for (int i=cant_in; i < cant_in+cant_be; i++) // pongo las busquedas exitosas de cant_i a cant_i+cant_be
        secuencia_ordenado[i]=be;

    for (int i=cant_in+cant_be; i < n; i++) // pongo las busquedas incorrectas de cant_in+cant_be a n
        secuencia_ordenado[i]=bi;
    
    int usado[n]; 

    for (int i=0; i < n; i++)
        usado[i]=0;
    int index=0;
    for (int i=0; i < n; i++){
        do{
            index = (rand() % n);
        }
        while (usado[index]);
        secuencia[i] = secuencia_ordenado[index];
        usado[index]=1;
    }
 
}



int main(){
    initSecuencia();
    
    for (int i=0; i < n; i++){
        printf(" %d,",secuencia[i]);
    }
        
        
}