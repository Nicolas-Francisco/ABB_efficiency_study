#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "Abb.c"

clock_t start, end;
double cpu_time_used;

#define ins 0; //codigo de insercion
#define be 1; //codigo de busqueda exitosa
#define bi 2; //codigo de busqueda infructuosa

#define n 1000000 //largo de la secuencia

#define pi 0.5 //probabilidad de insercion
#define pbe 0.33 //probabilidad de busqueda exitosa
#define pbi 0.17 //probabilidad de busqueda infructuosa

long cant_in=(long)(n*pi); //cantidad de inserciones
long cant_be=(long)(n*pbe); //cantidad de busquedas exitosas
long cant_bi=(long)(n*pbi); //cantidad de bisquedas infructuosas

int secuencia[n]; //arreglo de largo 100 que cant_nodosendra las operaciones

//busca un elemento en un arreglo, si lo encuentra retorna 1 sino entrega 0
int search_in_array(long arreglo[], long busqueda, long ene) {
    for (int i = 0; i < ene; i++) {
        if (arreglo[i] == busqueda) return 1;
    }
    return 0;
}

long get_random_not_repeted(long arreglo[],long size,long max_numero){
    long random;
    do{
        random = (rand() % max_numero);
    } while (search_in_array(arreglo,random,size));
    return random;
}


//genera la secuencia desordena de intrucciones
void initSecuencia(){
    int secuencia_ordenado[n]; // [0,0,0,0,0,1,1,1,2,2]
    for (int i=0; i < cant_in; i++) //se coloca las inserciones
        secuencia_ordenado[i]=ins;
    for (int i=cant_in; i < cant_in+cant_be; i++) //se coloca las busquedas exitosas
        secuencia_ordenado[i]=be;
    for (int i=cant_in+cant_be; i < n; i++) //se coloca las busquedas infructuosas
        secuencia_ordenado[i]=bi;
    
    /// desordenamos el arreglo ///
    int usado[n]; //arreglo que indica si el elemento asociado a esa pos fue desordenado
    usado[0]=1; //colocamos el primer elemento en 1 para que no sea movido
    for (int i=1; i < n; i++){ //inicializamos el resto con 0
        usado[i]=0;
    }          
    int index=1;
    for (int i=1; i < n; i++){ 
        do{ //vamos escogiendo un indice al azar mientras no haya sido movido antes
            index = (rand() % n);
        }
        while (usado[index]);
        secuencia[i] = secuencia_ordenado[index]; //guardo el elemento en la pos i
        usado[index]=1; //marco la posicion para indicar que ya fue movido
    }
}

//experimento creciente
double creciente(double factor){
    NodeAbb *rootABB = NULL; //iniacializo mi arbol
    NodeAvl *rootAVL = NULL;
    //Node *rootSPLAY = NULL;
    NodeBt_16 *rootBt_16 = NULL;
    NodeBt_256 *rootBt_256 = NULL;
    NodeBt_4096 *rootBt_4096 = NULL;
    long *nodos= (long *)malloc(sizeof(long)*(n/2));
    long *params = (long *)malloc(sizeof(long)*n);
    long param;
    long index = 0; //indice de operacion
    long m=0;
    long max_numero = pow(2,32); //maximo numero posible
    long random = 0; //numero obtenido al azar
    int operacion; //operacion obtenida de la secuencia
    double k = 0; // el k



    start = clock(); //inicio el temporizador
    for (long i=0; i < n; i++){ //recorro mi secuencia
        operacion = secuencia[i]; //obtengo la operacion

        if (operacion==0){ //la operacion es una insercion

            do{ //busco un random hasta que no este en el arreglo de nodos
                if (k>=1){
                    random = (rand() % (int)k); // busco un numero al azar entre 0 y k
                    random=random+m; //sumo m al random
                }
                else{
                    random=0+m;
                }
            } while (search_in_array(nodos,random,m));
            nodos[m]=random;//agrego el nodo
            m++;//aumento la cantidad
            k=m*factor;//redefino el k
            insert(&rootABB,random);//inserto el nodo en el arbol
        }
        
        else if (operacion==1){ //la operacion es una busqueda exitosa

            int index = (rand() % m);//obtengo un numero random entre 0 y la cantidad de nodos
            random=nodos[index];//obtengo el numero del nodo
            Node * node = search(&rootABB,random);//busco el nodo en mi arbol
            //printf("busque exitosamente el %lld\n",random);
        }
        else{ //La operacion es una busqueda infructuosa
            do{ //busco un random hasta que no este en el arreglo de nodos
                random = (rand() % max_numero);
            } while (search_in_array(nodos,random,m)); //obtengo un numero al azar entre 0 y max_numero
            Node * node = search(&rootABB,random);//busco el nodo en mi arbol
            //printf("busque fallidamente el %lld\n",random);
        }
    }
    end = clock();//termino temporizador   
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;//obtengo el tiempo de ejecucion 
    return cpu_time_used;
}