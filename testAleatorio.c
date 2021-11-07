#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "Abb.c"
#include "Avl.c"
#include "Btree.c"
#include "Btree16.c"
#include "Btree256.c"
#include "Btree4096.c"

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

double p_x(double x){ // p(x)=x
    return x;
}

double p_sqrt(double x){ //p(x)=sqrt(x)
    return sqrt(x);
}

double p_ln(double x){ //p(x)=ln(x)
    return log(x);
}

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

//encuentra la posicion del entero mas cercano a busqueda
long get_bigger(float arreglo[], float busqueda, long ene ){
    for (int i = 0; i < ene; i++) {
        if (arreglo[i] >= busqueda) return i;
    }
    return -1;
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

//experimento aleatorio
double aleatorio(){
    Node *rootABB = NULL; //iniacializo mi arbol
    Node *rootAVL = NULL;
    Node *rootBT = NULL;
    Node *rootBT16 = NULL;
    Node *rootBT256 = NULL;
    Node *rootBT4096 = NULL;
    //Node *rootSPLAY = NULL;

    //long nodos[n/2]; //arreglo donde se guardara los nodos insertados
    //long params[n];
    long *nodos= (long *)malloc(sizeof(long)*(n/2));
    long *params = (long *)malloc(sizeof(long)*n);
    long param;
    long index = 0; //indice de operacion
    long cant_nodos=0;
    long max_numero = pow(2,32); //maximo numero posible
    long random = 0; //numero obtenido al azar
    int operacion; //operacion obtenida de la secuencia

    for (long i=0; i<n; i++) {
        operacion = secuencia[i];
        if (operacion==0){
            long random = get_random_not_repeted(nodos,cant_nodos,max_numero);
            nodos[cant_nodos]=random; 
            params[index]=random; 
            cant_nodos++;
            index++; 
        }
        else if (operacion==1){
            long indice = (rand() % cant_nodos);
            random=nodos[indice];
            params[index]=random;
            index++;
        }
        else{
            long random = get_random_not_repeted(nodos,cant_nodos,max_numero);
            params[index]=random; 
            index++;
        }
    }

    start = clock(); //inicio el temporizador

    for (long i=0; i < n; i++){ //recorro mi secuencia
        operacion = secuencia[i]; //obtengo la operacion
        param = params[i];
        //printf("el paramentro es %d\n",param);
        if (operacion==0){ //la operacion es una insercion
            insertABB(&rootABB,param); //inserto el nodo en ABB
            insertABB(&rootAVL,param); //inserto el nodo en AVL
            insertABB(&rootBT,param); //inserto el nodo en BT
            insertABB(&rootBT16,param); //inserto el nodo en BT
            insertABB(&rootBT256,param); //inserto el nodo en BT
            insertBT(&rootBT4096,param); //inserto el nodo en BT
            insertSPLAY(param); //inserto el nodo en splay

            printf("inserte el %lld\n",param);
        }
        else if (operacion==1){ //la operacion es una busqueda exitosa
            Node * node = search(&rootABB,param);//busco el nodo en mi arbol
            Node * node = search(&rootABB,param);//busco el nodo en mi arbol
            Node * node = search(&rootABB,param);//busco el nodo en mi arbol
            Node * node = search(&rootABB,param);//busco el nodo en mi arbol
            Node * node = search(&rootABB,param);//busco el nodo en mi arbol
            Node * node = search(&rootABB,param);//busco el nodo en mi arbol
            Node * node = search(&rootABB,param);//busco el nodo en mi arbol
            printf("busque exitosamente el %lld\n",param);
        }
        else{ //La operacion es una busqueda infructuosa
            Node * node = search(&rootABB,param);//busco el nodo en mi arbol
            printf("busque fallidamente el %lld\n",param);
        }
    }
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; //obtengo el tiempo de ejecucion
    return cpu_time_used;
}

int main(){
    srand(time(NULL)); 
    initSecuencia();  
    double timeA = aleatorio(); 
    printf("aaaa %f\n",timeA);

    /*

    printf("--------------Experimento aleatorio--------------------------\n");
    FILE* ficheroAleatorioAbb;
    ficheroAleatorioAbb = fopen("Datos/aleatorioAbb.txt", "w");
    for (int i = 0; i < 100; i++){
        double timeA = aleatorio();
        fprintf(ficheroAleatorioAbb, "%f",timeA);
        if(i<99){
            fprintf(ficheroAleatorioAbb, ",");
        }        
    }
    fclose(ficheroAleatorioAbb);
    printf("\n");

    printf("--------------Experimento creciente con factor 0.1------------\n");
    FILE* ficheroCreciente_0p1_Abb;
    ficheroCreciente_0p1_Abb = fopen("Datos/creciente_0p1_Abb.txt", "w");
    for (int i = 0; i < 100; i++){
        double timeC_0p1 = creciente(0.1);
        fprintf(ficheroCreciente_0p1_Abb, "%f",timeC_0p1);
        if(i<99){
            fprintf(ficheroCreciente_0p1_Abb, ",");
        }        
    }
    fclose(ficheroCreciente_0p1_Abb);
    printf("\n");

    printf("--------------Experimento creciente con factor 0.5------------\n");
    FILE* ficheroCreciente_0p5_Abb;
    ficheroCreciente_0p5_Abb = fopen("Datos/creciente_0p5_Abb.txt", "w");
    for (int i = 0; i < 100; i++){
        double timeC_0p5 = creciente(0.5);
        fprintf(ficheroCreciente_0p5_Abb, "%f",timeC_0p5);
        if(i<99){
            fprintf(ficheroCreciente_0p5_Abb, ",");
        }        
    }
    fclose(ficheroCreciente_0p5_Abb);
    printf("\n");

    printf("--------------------Sesgado p(x)=x----------------------------\n");
    FILE* ficheroSesgado_x_Abb;
    ficheroSesgado_x_Abb = fopen("Datos/sesgado_x_Abb.txt", "w");
    for (int i = 0; i < 100; i++){
        double time_s_x = sesgada(&p_x);
        fprintf(ficheroSesgado_x_Abb, "%f",time_s_x);
        if(i<99){
            fprintf(ficheroSesgado_x_Abb, ",");
        }        
    }
    fclose(ficheroSesgado_x_Abb);
    printf("\n");

    printf("--------------------Sesgado p(x)=sqrt(x)------------------------\n");
    
    FILE* ficheroSesgado_sqrt_Abb;
    ficheroSesgado_sqrt_Abb = fopen("Datos/sesgado_sqrt_Abb.txt", "w");
    for (int i = 0; i < 100; i++){
       double time_s_sqrt = sesgada(&p_sqrt);
        fprintf(ficheroSesgado_sqrt_Abb, "%f",time_s_sqrt);
        if(i<99){
            fprintf(ficheroSesgado_sqrt_Abb, ",");
        }        
    }
    fclose(ficheroSesgado_sqrt_Abb);
    printf("\n");

    printf("--------------------Sesgado p(x)=ln(x)---------------------------\n");
    
    FILE* ficheroSesgado_ln_Abb;
    ficheroSesgado_ln_Abb = fopen("Datos/sesgado_ln_Abb.txt", "w");
    for (int i = 0; i < 100; i++){
       double time_s_ln = sesgada(&p_ln);
        fprintf(ficheroSesgado_ln_Abb, "%f",time_s_ln);
        if(i<99){
            fprintf(ficheroSesgado_ln_Abb, ",");
        }        
    }
    fclose(ficheroSesgado_ln_Abb);
    printf("\n");
*/

}