#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "Abb.c"
#include "Avl.c"
#include "Splay.c"
//#include "Btree.c"
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

long long cant_in=(long long)(n*pi); //cantidad de inserciones
long long cant_be=(long long)(n*pbe); //cantidad de busquedas exitosas
long long cant_bi=(long long)(n*pbi); //cantidad de bisquedas infructuosas

int secuencia[n]; //arreglo de largo 100 que cant_nodosendra las operaciones

//busca un elemento en un arreglo, si lo encuentra retorna 1 sino entrega 0
int search_in_array(long long arreglo[], long long busqueda, long long ene) {
    for (int i = 0; i < ene; i++) {
        if (arreglo[i] == busqueda) return 1;
    }
    return 0;
}

long long get_random_not_repeted(long long arreglo[],long long size,long long max_numero){
    long long random;
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

//experimento aleatorio
void aleatorio(double times[]){
    NodeAbb *rootABB = NULL; //iniacializo mi arbol
    NodeAvl *rootAVL = NULL;
    //Node *rootSPLAY = NULL;
    NodeBt_16 *rootBt_16 = NULL;
    NodeBt_256 *rootBt_256 = NULL;
    NodeBt_4096 *rootBt_4096 = NULL;
    long long *nodos= (long long *)malloc(sizeof(long long)*(n/2));
    long long *params = (long long *)malloc(sizeof(long long)*n);
    long long param;
    long long index = 0; //indice de operacion
    long long cant_nodos=0;
    long long max_numero = pow(2,32); //maximo numero posible
    long long random = 0; //numero obtenido al azar
    int operacion; //operacion obtenida de la secuencia

    printf("Loaging\n");

    for (long long i=0; i<n; i++) {
        operacion = secuencia[i];
        if (operacion==0){
            long long random = get_random_not_repeted(nodos,cant_nodos,max_numero);
            nodos[cant_nodos]=random; 
            params[index]=random; 
            cant_nodos++;
            index++; 
            printf("[%lld,%lld]\n",i+1,n);
        }
        else if (operacion==1){
            long long indice = (rand() % cant_nodos);
            random=nodos[indice];
            params[index]=random;
            index++;
            printf("[%lld,%lld]\n",i+1,n);
        }
        else{
            long long random = get_random_not_repeted(nodos,cant_nodos,max_numero);
            params[index]=random; 
            index++;
            printf("[%lld,%lld]\n",i+1,n);
        }
    }
    
    //printf(" ACA PARTE EL ABB\n");
    
    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){ //recorro mi secuencia
        operacion = secuencia[i]; //obtengo la operacion
        param = params[i];
        //printf("el paramentro es %d\n",param);
        if (operacion==0){ //la operacion es una insercion
            insertABB(&rootABB,param); //inserto el nodo en ABB
            //printf("inserte el %lld\n",param);
        }
        else if (operacion==1){ //la operacion es una busqueda exitosa
            NodeAbb * node = searchABB(&rootABB,param);//busco el nodo en mi arbol
            //printf("busque exitosamente el %lld\n",param);
        }
        else{ //La operacion es una busqueda infructuosa
            NodeAbb * node = searchABB(&rootABB,param);//busco el nodo en mi arbol
            //printf("busque fallidamente el %lld\n",param);
        }
    }
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; //obtengo el tiempo de ejecucion
    times[0]=cpu_time_used;

    //printf(" ACA PARTE EL AVL\n");

    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){ //recorro mi secuencia
        operacion = secuencia[i]; //obtengo la operacion
        param = params[i];
        //printf("el paramentro es %d\n",param);
        if (operacion==0){ //la operacion es una insercion
            insertAvl(&rootAVL,param); //inserto el nodo en ABB
            //printf("inserte el %lld\n",param);
        }
        else if (operacion==1){ //la operacion es una busqueda exitosa
            NodeAvl * node = searchAvl(&rootAVL,param);//busco el nodo en mi arbol
            //printf("busque exitosamente el %lld\n",param);
        }
        else{ //La operacion es una busqueda infructuosa
            NodeAvl * node = searchAvl(&rootAVL,param);//busco el nodo en mi arbol
            //printf("busque fallidamente el %lld\n",param);
        }
    }
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; //obtengo el tiempo de ejecucion
    times[1]=cpu_time_used;

   // printf("ACA PARTE EL SPLAY");

    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){ //recorro mi secuencia
        operacion = secuencia[i]; //obtengo la operacion
        param = params[i];
        //printf("el paramentro es %d\n",param);
        if (operacion==0){ //la operacion es una insercion
            insertSplay(param); //inserto el nodo en ABB
            //printf("inserte el %lld\n",param);
        }
        else if (operacion==1){ //la operacion es una busqueda exitosa
            NodeSplay * node = searchSplay(param);//busco el nodo en mi arbol
            //printf("busque exitosamente el %lld\n",param);
        }
        else{ //La operacion es una busqueda infructuosa
            NodeSplay * node = searchSplay(param);//busco el nodo en mi arbol
            //printf("busque fallidamente el %lld\n",param);
        }
    }
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; //obtengo el tiempo de ejecucion
    times[2]=cpu_time_used;

    //printf("ACA PARTE EL ARBOL B 16\n");

    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){ //recorro mi secuencia
        operacion = secuencia[i]; //obtengo la operacion
        param = params[i];
        //printf("el paramentro es %d\n",param);
        if (operacion==0){ //la operacion es una insercion
            insertBt_16(&rootBt_16,param); //inserto el nodo en ABB
            //printf("inserte el %lld\n",param);
        }
        else if (operacion==1){ //la operacion es una busqueda exitosa
            NodeBt_16 * node = searchBt_16(&rootBt_16,param);//busco el nodo en mi arbol
            //printf("busque exitosamente el %lld\n",param);
        }
        else{ //La operacion es una busqueda infructuosa
            NodeBt_16 * node = searchBt_16(&rootBt_16,param);//busco el nodo en mi arbol
            //printf("busque fallidamente el %lld\n",param);
        }
    }
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; //obtengo el tiempo de ejecucion
    times[3]=cpu_time_used;

    //printf("ACA PARTE EL ARBOL B 256\n");

    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){ //recorro mi secuencia
        operacion = secuencia[i]; //obtengo la operacion
        param = params[i];
        //printf("el paramentro es %d\n",param);
        if (operacion==0){ //la operacion es una insercion
            insertBt_256(&rootBt_256,param); //inserto el nodo en ABB
           // printf("inserte el %lld\n",param);
        }
        else if (operacion==1){ //la operacion es una busqueda exitosa
            NodeBt_256 * node = searchBt_256(&rootBt_256,param);//busco el nodo en mi arbol
            //printf("busque exitosamente el %lld\n",param);
        }
        else{ //La operacion es una busqueda infructuosa
            NodeBt_256 * node = searchBt_256(&rootBt_256,param);//busco el nodo en mi arbol
            //printf("busque fallidamente el %lld\n",param);
        }
    }
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; //obtengo el tiempo de ejecucion
    times[4]=cpu_time_used;

    //printf("ACA PARTE EL ARBOL B 4096\n");

    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){ //recorro mi secuencia
        operacion = secuencia[i]; //obtengo la operacion
        param = params[i];
        //printf("el paramentro es %d\n",param);
        if (operacion==0){ //la operacion es una insercion
            insertBt_4096(&rootBt_4096,param); //inserto el nodo en ABB
            //printf("inserte el %lld\n",param);
        }
        else if (operacion==1){ //la operacion es una busqueda exitosa
            NodeBt_4096 * node = searchBt_4096(&rootBt_4096,param);//busco el nodo en mi arbol
            //printf("busque exitosamente el %lld\n",param);
        }
        else{ //La operacion es una busqueda infructuosa
            NodeBt_4096 * node = searchBt_4096(&rootBt_4096,param);//busco el nodo en mi arbol
            //printf("busque fallidamente el %lld\n",param);
        }
    }
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; //obtengo el tiempo de ejecucion
    times[5]=cpu_time_used;


}

int main(){
    srand(time(NULL)); 
    initSecuencia();  
    double times[6];
    aleatorio(times);
    printf("El tiempo aleatorio de ABB es %f \n",times[0]);
    printf("El tiempo aleatorio de AVL es %f \n",times[1]);
    printf("El tiempo aleatorio de SPLAY es %f \n",times[2]);
    printf("El tiempo aleatorio de BTREE 16 es %f \n",times[3]);
    printf("El tiempo aleatorio de BTREE 256 es %f \n",times[4]);
    printf("El tiempo aleatorio de BTREE 4096 es %f \n",times[5]);


    

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