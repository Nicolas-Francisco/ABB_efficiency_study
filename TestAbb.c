#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "Abb.c"

clock_t start, end;
double cpu_time_used;

#define ins 0; //insercion
#define be 1; //busqueda exitosa
#define bi 2; //busqueda incorrecta

//#define n  1000000 // 10 a la 6
#define n 100

#define pi 0.5
#define pbe 0.33
#define pbi 0.17

int cant_in=round(n*pi);
int cant_be=round(n*pbe);
int cant_bi=round(n*pbi);

int secuencia[n];

int search_in_array(long arreglo[], long busqueda, long ene) {
    for (int i = 0; i < ene; i++) {
        if (arreglo[i] == busqueda) return 1;
    }
    return 0;
}
//genera la secuencia desordena de intrucciones
void initSecuencia(){
    int secuencia_ordenado[n]; // [0,0,0,0,0,1,1,1,2,2]
    for (int i=0; i < cant_in; i++) // pongo las inserciones de 0 a cant_in
        secuencia_ordenado[i]=ins;
    for (int i=cant_in; i < cant_in+cant_be; i++) // pongo las busquedas exitosas de cant_i a cant_i+cant_be
        secuencia_ordenado[i]=be;
    for (int i=cant_in+cant_be; i < n; i++) // pongo las busquedas incorrectas de cant_in+cant_be a n
        secuencia_ordenado[i]=bi;
    
    /// desordenamos el arreglo ///
    
    int usado[n]; //[0,0,0,0,0,0,0]
    usado[0]=1;
    for (int i=1; i < n; i++){
        usado[i]=0;
    }          
    int index=1;
    for (int i=1; i < n; i++){ 
        do{
            index = (rand() % n);
        }
        while (usado[index]);
        secuencia[i] = secuencia_ordenado[index];
        usado[index]=1;
    }
    ///////////////////////////////
}

//experimento aleatorio
double aleatorio(){
    printf("--------------Experimento aleatorio-------------------------\n");
    Node *rootABB = NULL;
    long numeros[n/2];
    long cont = 0;
    long max = pow(2,32);
    long random = 0;
    start = clock();
    for (long i=0; i < n; i++){ //recorro mi secuencia
        int operation = secuencia[i];
        if (operation==0){ //insercion en los arboles
            do{
                random = (rand() % max);
            } while (search_in_array(numeros,random,cont));
            numeros[cont]=random;
            cont++;
            insert(&rootABB,random);
            printf("inserte el %lld\n",random);
        }
        
        else if (operation==1){ //busqueda exitosa
            int index = (rand() % cont);
            random=numeros[index];
            Node * node = search(&rootABB,random);
            printf("busque exitosamente el %lld\n",random);
        }
        else{ //busqueda infructuosa
            do{
                random = (rand() % max);
            } while (search_in_array(numeros,random,cont));
            Node * node = search(&rootABB,random);
            printf("busque fallidamente el %lld\n",random);
        }
    }
    end = clock();   
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; 
    return cpu_time_used;
}

double creciente(double factor){
    printf("--------------Experimento creciente con factor %f-------------------------\n",factor);
    Node *rootABB = NULL;
    long numeros[n/2];
    long  m = 0;
    long max = pow(2,32);
    long random = 0;
    double k = 0; // el k
    start = clock();
    for (long i=0; i < n; i++){ //recorro mi secuencia
        int operation = secuencia[i];
        if (operation==0){ //insercion en los arboles
            do{
                random = (rand() % k); // busco un numero entre 0 y k
                //random = m;
                random=random+m; //k+m
            } while (search_in_array(numeros,random,m));
            numeros[m]=random;
            m++;
            k=m*factor;
            printf("kkkkk es %f",k);
            insert(&rootABB,random);
            printf("inserte el %lld\n",random);
        }
        
        else if (operation==1){ //busqueda exitosa
            int index = (rand() % m);
            random=numeros[index];
            Node * node = search(&rootABB,random);
            printf("busque exitosamente el %lld\n",random);
        }
        else{ //busqueda infructuosa
            do{
                random = (rand() % max);
            } while (search_in_array(numeros,random,m));
            Node * node = search(&rootABB,random);
            printf("busque fallidamente el %lld\n",random);
        }
    }
    end = clock();   
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; 
    return cpu_time_used;
}


int main(){
    srand(time(NULL)); 
    initSecuencia();
    //printf("la cantidad de insercciones sera %d\n",cant_in);
    //printf("la cantidad de busquedas exitosas sera %d\n",cant_be);
    //printf("la cantidad de busquedas incorrectas sera %d\n",cant_bi);
    //for (int i=0; i < n; i++){
    //    printf("%d, ",secuencia[i]);
    //}
    //printf("\n");
    
    double timeA = aleatorio();
    printf("El tiempo de ejecucion es %f\n",timeA);
    double timeC_0p1 = creciente(0.1);
    //printf("El tiempo de ejecucion es %f\n",timeA);
    
}