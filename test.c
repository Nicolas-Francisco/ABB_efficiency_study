#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "Abb.c"
#include "Avl.c"
#include "Splay.c"
#include "Btree16.c"
#include "Btree256.c"
#include "Btree4096.c"


#include <string.h>


clock_t start, end;
double cpu_time_used;

#define ins 0; //codigo de insercion
#define be 1; //codigo de busqueda exitosa
#define bi 2; //codigo de busqueda infructuosa

#define n 1000000 //largo de la secuencia
#define interval 1000 //intervalo de operaciones

#define pi 0.5 //probabilidad de insercion
#define pbe 0.33 //probabilidad de busqueda exitosa
#define pbi 0.17 //probabilidad de busqueda infructuosa

long long cant_in=(long long)(n*pi); //cantidad de inserciones
long long cant_be=(long long)(n*pbe); //cantidad de busquedas exitosas
long long cant_bi=(long long)(n*pbi); //cantidad de bisquedas infructuosas

int secuencia[n]; //areglo que almacenara la secuencia

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

double p_x(double x){ // p(x)=x
    return x;
}

double p_sqrt(double x){ //p(x)=sqrt(x)
    return sqrt(x);
}

double p_ln(double x){ //p(x)=ln(x)
    return log(x);
}

//encuentra la posicion del entero mas cercano a busqueda
long long get_bigger(float arreglo[], float busqueda, long long ene ){
    for (int i = 0; i < ene; i++) {
        if (arreglo[i] >= busqueda) return i;
    }
    return -1;
}

//funcion que genera los paramentros requeridos en cada uno de los experimentos
//apartir de un archivo txt previamente precomputado
void generate_params(long long ** buffer,char * archivo,double (*f_x)(double),
double (*f_sqrt)(double),double (*f_ln)(double)){
    long long *nodosAleatorios= (long long *)malloc(sizeof(long long)*(n/2)); // arreglo de nodos para el experimento aleatorio
    long long *paramsAleatorios = (long long *)malloc(sizeof(long long)*n); //arreglo para los paramentros del experimento aleatorio

    long long *nodosCreciente_0p1= (long long *)malloc(sizeof(long long)*(n/2));// arreglo de nodos para el experimento creciente con k=0.1m
    long long *paramsCreciente_0p1 = (long long *)malloc(sizeof(long long)*n);//arreglo para los paramentros del experimento creciente con k=0.1m

    long long *nodosCreciente_0p5= (long long *)malloc(sizeof(long long)*(n/2));// arreglo de nodos para el experimento creciente con k=0.5m
    long long *paramsCreciente_0p5 = (long long *)malloc(sizeof(long long)*n);//arreglo para los paramentros del experimento creciente con k=0.5m

    long long *nodosSesgado_x= (long long *)malloc(sizeof(long long)*(n/2));// arreglo de nodos para el experimento sesgado con p(x)=x
    long long *paramsSesgado_x = (long long *)malloc(sizeof(long long)*n);//arreglo para los paramentros del experimento sesgado con p(x)=x
    float *probSesgado_x = (float *)malloc(sizeof(float)*(n/2));//arreglo de las probabilidades para el experimento sesgado con p(x)=x

    long long *nodosSesgado_sqrt= (long long *)malloc(sizeof(long long)*(n/2));// arreglo de nodos para el experimento sesgado con p(x)=sqrt(x)
    long long *paramsSesgado_sqrt = (long long *)malloc(sizeof(long long)*n);//arreglo para los paramentros del experimento sesgado con p(x)=sqrt(x)
    float *probSesgado_sqrt = (float *)malloc(sizeof(float)*(n/2));//arreglo de las probabilidades para el experimento sesgado con p(x)=sqrt(x)

    long long *nodosSesgado_ln= (long long *)malloc(sizeof(long long)*(n/2));// arreglo de nodos para el experimento aleatorio con p(x)=ln(x)
    long long *paramsSesgado_ln = (long long *)malloc(sizeof(long long)*n);//arreglo para los paramentros del experimento sesgado con p(x)=ln(x)
    float *probSesgado_ln = (float *)malloc(sizeof(float)*(n/2));//arreglo de las probabilidades para el experimento sesgado con p(x)=ln(x)

    long long index = 0; //indice de operacion
    long long cant_nodos=0; //cantidad de nodos
    long long max_numero = pow(2,32); //maximo numero posible
    long long random = 0; //numero obtenido al azar
    long long random_0yk = 0; //random entre 0 y k
    int operacion; //operacion obtenida de la secuencia

    double k_0p1 =0; // k para experimento creciente con k = 0.1m
    double k_0p5= 0; // k para experimento creciente con k = 0.5m
    double factor_k;

    float P_x = 0; //prob total del p(x)
    float P_sqrt = 0; //prob total del p(x)
    float P_ln = 0; //prob total del p(x)

    float pesoAleatorio;

    float peso; //peso individual

    long long pos;

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;


    printf("Loading parametros\n");
    
    fp = fopen(archivo,"r");//abro el archivo que con los numeros rando precalculados

    while (((read = getline(&line, &len, fp)) != -1) && (index<n)){ //mientras aun haya un numero random en el archivo
        random = atol(line); //obtengo el numer0
        operacion = secuencia[index];
        if (operacion==0){ // si es una insercion
            
            //en el caso del el experimento aleatorio, simplemente guardo el nodo en la lista
            //y ademas guardo el mismo numero como paramentro
            nodosAleatorios[cant_nodos]=random; 
            paramsAleatorios[index]=random;
           
            //en el caso del experimento creciente con k=0.1m, necesito transformar el random a un numero entre 0 y k
            //para luego sumarle la cantidad de nodos actual y guardar este valor en la lista de nodos y paramentros
            factor_k =k_0p1/max_numero;
            random_0yk =(long long) random*factor_k;
            nodosCreciente_0p1[cant_nodos]=random_0yk+cant_nodos; 
            paramsCreciente_0p1[index]=random_0yk+cant_nodos;

            //en el caso del experimento creciente con k=0.5m, necesito transformar el random a un numero entre 0 y k
            //para luego sumarle la cantidad de nodos actual y guardar este valor en la lista de nodos y paramentros
            factor_k =k_0p5/max_numero;
            random_0yk = (long long)(random*factor_k);
            nodosCreciente_0p5[cant_nodos]=random_0yk+cant_nodos; 
            paramsCreciente_0p5[index]=random_0yk+cant_nodos;
   
            //en este caso, guardo el numero random en la lista de nodos y en la lista de paramentros pero ademas
            //obtengo el peso del random para luego calcular el peso acumulado hasta el momento para finalmente guardar este
            //valor en la lista de probs del esperimento
            nodosSesgado_x[cant_nodos]=random;
            paramsSesgado_x[index]=random;
            peso = f_x(random);
            P_x=P_x+peso;
            probSesgado_x[cant_nodos]=P_x;

            //en este caso, guardo el numero random en la lista de nodos y en la lista de paramentros pero ademas
            //obtengo el peso del random para luego calcular el peso acumulado hasta el momento para finalmente guardar este
            //valor en la lista de probs del esperimento
            nodosSesgado_sqrt[cant_nodos]=random;
            paramsSesgado_sqrt[index]=random;
            peso = f_sqrt(random);
            P_sqrt=P_sqrt+peso;
            probSesgado_sqrt[cant_nodos]=P_sqrt;

            //en este caso, guardo el numero random en la lista de nodos y en la lista de paramentros pero ademas
            //obtengo el peso del random para luego calcular el peso acumulado hasta el momento para finalmente guardar este
            //valor en la lista de probs del esperimento
            nodosSesgado_ln[cant_nodos]=random;
            paramsSesgado_ln[index]=random;
            peso = f_ln(random);
            P_ln=P_ln+peso;
            probSesgado_ln[cant_nodos]=P_ln;

            //finalmente actualizo el numero de nodos, los dos k y el index
            cant_nodos++;
            k_0p1 = cant_nodos*0.1;
            k_0p5 = cant_nodos*0.5;
            index++; 
        }

        else if (operacion==1){//si es una busqueda exitosa

            //calculo en este caso un numero random entre 0 y la cantidad de nodos para decidir que numero buscar
            long long indice = (rand() % cant_nodos);

            //con el indice obtengo el nodo que buscaremos exitosamente y lo agrego a los paramentros
            random=nodosAleatorios[indice];
            paramsAleatorios[index]=random;

            //con el indice obtengo el nodo que buscaremos exitosamente y lo agrego a los paramentros
            random=nodosCreciente_0p1[indice];
            paramsCreciente_0p1[index]=random;

            //con el indice obtengo el nodo que buscaremos exitosamente y lo agrego a los paramentros
            random=nodosCreciente_0p5[indice];
            paramsCreciente_0p5[index]=random;

            //transformamos este random a un numero entre 0 y P 
            //con este numero ahora buscamos la posicion del numero del siguiente mayor a este
            //y finalmente con esta posicion obtenemos un nodo de la lista de nodos y lo agregamos
            //a la lista de prametros del experimento
            pesoAleatorio = (P_x/cant_nodos)*indice;
            pos = get_bigger(probSesgado_x,pesoAleatorio,cant_nodos);
            random=nodosSesgado_x[pos];
            paramsSesgado_x[index]=random;
            
            //transformamos este random a un numero entre 0 y P 
            //con este numero ahora buscamos la posicion del numero del siguiente mayor a este
            //y finalmente con esta posicion obtenemos un nodo de la lista de nodos y lo agregamos
            //a la lista de prametros del experimento
            pesoAleatorio = (P_sqrt/cant_nodos)*indice;
            pos = get_bigger(probSesgado_sqrt,pesoAleatorio,cant_nodos);
            random=nodosSesgado_sqrt[pos];
            paramsSesgado_sqrt[index]=random;

            //transformamos este random a un numero entre 0 y P 
            //con este numero ahora buscamos la posicion del numero del siguiente mayor a este
            //y finalmente con esta posicion obtenemos un nodo de la lista de nodos y lo agregamos
            //a la lista de prametros del experimento
            pesoAleatorio = (P_ln/cant_nodos)*indice;
            pos = get_bigger(probSesgado_ln,pesoAleatorio,cant_nodos);
            random=nodosSesgado_ln[pos];
            paramsSesgado_ln[index]=random;

            //aumento el index
            index++;
            
        }
        else{//si es una busqueda infructuosa

            //simplemente agregamos el random precalculado en la lista de paramentros
            paramsAleatorios[index]=random;
           
            //simplemente agregamos el random precalculado en la lista de paramentros
            paramsCreciente_0p1[index]=random;

            //simplemente agregamos el random precalculado en la lista de paramentros
            paramsCreciente_0p5[index]=random;

            //simplemente agregamos el random precalculado en la lista de paramentros
            paramsSesgado_x[index]=random;

            //simplemente agregamos el random precalculado en la lista de paramentros
            paramsSesgado_sqrt[index]=random;

            //simplemente agregamos el random precalculado en la lista de paramentros
            paramsSesgado_ln[index]=random;
     
            //aumento el index
            index++;
        }
    }

    //cierro el archivo
    fclose(fp);
    
    //agrego los paramentros al buffer para luego usarlos en los respectivos experimentos
    buffer[0]=paramsAleatorios;
    buffer[1]=paramsCreciente_0p1;
    buffer[2]=paramsCreciente_0p5;
    buffer[3]=paramsSesgado_x;
    buffer[4]=paramsSesgado_sqrt;
    buffer[5]=paramsSesgado_ln;

    //libero los arreglos malloc que ya no se usaran
    free(nodosAleatorios);
    free(nodosCreciente_0p1);
    free(nodosCreciente_0p5);
    free(nodosSesgado_x);
    free(nodosSesgado_sqrt);
    free(nodosSesgado_ln);
    free(probSesgado_x);
    free(probSesgado_sqrt);
    free(probSesgado_ln);

}


//ejecuta una secuencia de operaciones
void ejecucion(double * times[],long long params[]){
    NodeAbb *rootABB = NULL; // inicializo arbol ABB
    NodeAvl *rootAVL = NULL; // inicializo arbol AVL
    //Node *rootSPLAY = NULL; // inicializo arbol SLAY
    NodeBt_16 *rootBt_16 = NULL; // inicializo arbol Btree 16
    NodeBt_256 *rootBt_256 = NULL; // inicializo arbol Btree 256
    NodeBt_4096 *rootBt_4096 = NULL; // inicializo arbol Btree 4096

    long long param; //parametro a usar en la operacion
    int operacion; //operacion de la secuencia

    double * timesAbb = (double *)malloc(sizeof(double)*(n/interval)); //arreglo para guardar los distintos tiempos del arbol ABB
    double * timesAvl = (double *)malloc(sizeof(double)*(n/interval)); //arreglo para guardar los distintos tiempos del arbol AVL
    double * timesSplay = (double *)malloc(sizeof(double)*(n/interval)); //arreglo para guardar los distintos tiempos del arbol SPLAY
    double * timesBtree16 = (double *)malloc(sizeof(double)*(n/interval)); //arreglo para guardar los distintos tiempos del arbol BTREE 16
    double * timesBtree256 = (double *)malloc(sizeof(double)*(n/interval)); //arreglo para guardar los distintos tiempos del arbol BTREE 256
    double * timesBtree4096 = (double *)malloc(sizeof(double)*(n/interval)); //arreglo para guardar los distintos tiempos del arbol BTREE 4096
    
    int index = 0;
    //----------- Arbol Abb ------------
    printf("Ejecutando el ABB\n");

    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){ 

        if ((i%interval==0)&&(i!=0)){
            //aca paro el timer para obtener el tiempo de ejecucion hasta el momento y lo guardo en su lista correspondiente
            //para luego volver a activarlo para que pueda calcular el siguiente intervalo
            end = clock(); 
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            timesAbb[index]=cpu_time_used;
            index++;
            start = clock();
        }

        operacion = secuencia[i]; 
        param = params[i];
        if (operacion==0){ //la operacion es una insercion
            insertABB(&rootABB,param);
            //printf("inserte el %lld\n",param);
        }
        else if (operacion==1){ //la operacion es una busqueda exitosa
            NodeAbb * node = searchABB(&rootABB,param);
            //printf("busque exitosamente el %lld\n",param);
        }
        else{ //La operacion es una busqueda infructuosa
            NodeAbb * node = searchABB(&rootABB,param);
            //printf("busque fallidamente el %lld\n",param);
        }
    }
    //obtengo el ultimo tiempo del arbol y lo guardo para finalmente guardar en el arreglo Times este arreglo
    //para su posterior uso
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    timesAbb[index]=cpu_time_used;
    times[0]=timesAbb;

    //seteo el index en 0
    index=0;


    //----------- Arbol Avl ------------
    printf("Ejecutando AVL\n");

    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){ 
        if ((i%interval==0)&&(i!=0)){
            //aca paro el timer para obtener el tiempo de ejecucion hasta el momento y lo guardo en su lista correspondiente
            //para luego volver a activarlo para que pueda calcular el siguiente intervalo
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            timesAvl[index]=cpu_time_used;
            index++;
            start = clock();
        }
        operacion = secuencia[i]; 
        param = params[i];
        if (operacion==0){ //la operacion es una insercion
            insertAvl(&rootAVL,param);
            //printf("inserte el %lld\n",param);
        }
        else if (operacion==1){ //la operacion es una busqueda exitosa
            NodeAvl * node = searchAvl(&rootAVL,param);
            //printf("busque exitosamente el %lld\n",param);
        }
        else{ //La operacion es una busqueda infructuosa
            NodeAvl * node = searchAvl(&rootAVL,param);
            //printf("busque fallidamente el %lld\n",param);
        }
    }
    //obtengo el ultimo tiempo del arbol y lo guardo para finalmente guardar en el arreglo Times este arreglo
    //para su posterior uso
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    timesAvl[index]=cpu_time_used;
    times[1]=timesAvl;

    //seteo el index en 0
    index=0;


   //----------- Arbol Splay ------------

    printf("Ejecutando Splay\n");
    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){ 
        if ((i%interval==0)&&(i!=0)){
            //aca paro el timer para obtener el tiempo de ejecucion hasta el momento y lo guardo en su lista correspondiente
            //para luego volver a activarlo para que pueda calcular el siguiente intervalo
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            timesSplay[index]=cpu_time_used;
            index++;
            start = clock();
        }
        operacion = secuencia[i]; 
        param = params[i];
        if (operacion==0){ //la operacion es una insercion
            insertSplay(param); 
        }
        else if (operacion==1){ //la operacion es una busqueda exitosa
            NodeSplay * node = searchSplay(param);
            //printf("busque exitosamente el %lld\n",param);
        }
        else{ //La operacion es una busqueda infructuosa
            NodeSplay * node = searchSplay(param);
            //printf("busque fallidamente el %lld\n",param);
        }
    }
    //obtengo el ultimo tiempo del arbol y lo guardo para finalmente guardar en el arreglo Times este arreglo
    //para su posterior uso
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    timesSplay[index]=cpu_time_used;
    times[2]=timesSplay;
    //seteo el index en 0
    index=0;

    //----------- Arbol Btree 16 ------------

    printf("Ejecutando Btree 16\n");
    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){ 
        if ((i%interval==0)&&(i!=0)){
            //aca paro el timer para obtener el tiempo de ejecucion hasta el momento y lo guardo en su lista correspondiente
            //para luego volver a activarlo para que pueda calcular el siguiente intervalo
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            timesBtree16[index]=cpu_time_used;
            index++;
            start = clock();
        }
        operacion = secuencia[i];
        param = params[i];
        if (operacion==0){ //la operacion es una insercion
            insertBt_16(&rootBt_16,param);
            //printf("inserte el %lld\n",param);
        }
        else if (operacion==1){ //la operacion es una busqueda exitosa
            NodeBt_16 * node = searchBt_16(&rootBt_16,param);
            //printf("busque exitosamente el %lld\n",param);
        }
        else{ //La operacion es una busqueda infructuosa
            NodeBt_16 * node = searchBt_16(&rootBt_16,param);
            //printf("busque fallidamente el %lld\n",param);
        }
    }
    
    //obtengo el ultimo tiempo del arbol y lo guardo para finalmente guardar en el arreglo Times este arreglo
    //para su posterior uso
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; 
    timesBtree16[index]=cpu_time_used;
    times[3]=timesBtree16;
    //seteo el index en 0
    index=0;

    //----------- Arbol Btree 256 ------------
    printf("Ejecutando Btree 256\n");

    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){ 
         if ((i%interval==0)&&(i!=0)){
            //aca paro el timer para obtener el tiempo de ejecucion hasta el momento y lo guardo en su lista correspondiente
            //para luego volver a activarlo para que pueda calcular el siguiente intervalo

            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            timesBtree256[index]=cpu_time_used;
            index++;
            start = clock();
        }
        operacion = secuencia[i]; 
        param = params[i];
        if (operacion==0){ //la operacion es una insercion
            insertBt_256(&rootBt_256,param); 
           // printf("inserte el %lld\n",param);
        }
        else if (operacion==1){ //la operacion es una busqueda exitosa
            NodeBt_256 * node = searchBt_256(&rootBt_256,param);
            //printf("busque exitosamente el %lld\n",param);
        }
        else{ //La operacion es una busqueda infructuosa
            NodeBt_256 * node = searchBt_256(&rootBt_256,param);
            //printf("busque fallidamente el %lld\n",param);
        }
    }
    //obtengo el ultimo tiempo del arbol y lo guardo para finalmente guardar en el arreglo Times este arreglo
    //para su posterior uso
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; 
    timesBtree256[index]=cpu_time_used;
    times[4]=timesBtree256;
    //seteo el index en 0
    index=0;

    //----------- Arbol Btree 4096 ------------
    printf("Ejecutando Btree 4096\n");
    

    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){
         if ((i%interval==0)&&(i!=0)){
            //aca paro el timer para obtener el tiempo de ejecucion hasta el momento y lo guardo en su lista correspondiente
            //para luego volver a activarlo para que pueda calcular el siguiente intervalo
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            timesBtree4096[index]=cpu_time_used;
            index++;
            start = clock();
        }
        operacion = secuencia[i];
        param = params[i];
        if (operacion==0){ //la operacion es una insercion
            insertBt_4096(&rootBt_4096,param);
            //printf("inserte el %lld\n",param);
        }
        else if (operacion==1){ //la operacion es una busqueda exitosa
            NodeBt_4096 * node = searchBt_4096(&rootBt_4096,param);
            //printf("busque exitosamente el %lld\n",param);
        }
        else{ //La operacion es una busqueda infructuosa
            NodeBt_4096 * node = searchBt_4096(&rootBt_4096,param);
            //printf("busque fallidamente el %lld\n",param);
        }
    }
    //obtengo el ultimo tiempo del arbol y lo guardo para finalmente guardar en el arreglo Times este arreglo
    //para su posterior uso
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; 
    timesBtree4096[index]=cpu_time_used;
    times[5]=timesBtree4096;
}

int main(){

    srand(time(NULL));
    char * mode = "w";

    //creo los 36 archivos necesarios para cada uno de los experimentos
    
    FILE *aleatorioAbb = fopen("Resultados/aleatorioAbb.txt", mode); 
    FILE *aleatorioAvl = fopen("Resultados/aleatorioAvl.txt", mode); 
    FILE *aleatorioSplay = fopen("Resultados/aleatorioSplay.txt", mode); 
    FILE *aleatorioBtree16 = fopen("Resultados/aleatorioBtree16.txt", mode); 
    FILE *aleatorioBtree256 = fopen("Resultados/aleatorioBtree256.txt", mode); 
    FILE *aleatorioBtree4096 = fopen("Resultados/aleatorioBtree4096.txt", mode); 

    FILE *creciente0p1Abb = fopen("Resultados/creciente0p1Abb.txt", mode); 
    FILE *creciente0p1Avl = fopen("Resultados/creciente0p1Avl.txt", mode); 
    FILE *creciente0p1Splay = fopen("Resultados/creciente0p1Splay.txt", mode); 
    FILE *creciente0p1Btree16 = fopen("Resultados/creciente0p1Btree16.txt", mode); 
    FILE *creciente0p1Btree256 = fopen("Resultados/creciente0p1Btree256.txt", mode); 
    FILE *creciente0p1Btree4096 = fopen("Resultados/creciente0p1Btree4096.txt", mode);

    FILE *creciente0p5Abb = fopen("Resultados/creciente0p5Abb.txt", mode); 
    FILE *creciente0p5Avl = fopen("Resultados/creciente0p5Avl.txt", mode); 
    FILE *creciente0p5Splay = fopen("Resultados/creciente0p5Splay.txt", mode); 
    FILE *creciente0p5Btree16 = fopen("Resultados/creciente0p5Btree16.txt", mode); 
    FILE *creciente0p5Btree256 = fopen("Resultados/creciente0p5Btree256.txt", mode); 
    FILE *creciente0p5Btree4096 = fopen("Resultados/creciente0p5Btree4096.txt", mode);

    FILE *sesgadoxAbb = fopen("Resultados/sesgadoxAbb.txt", mode); 
    FILE *sesgadoxAvl = fopen("Resultados/sesgadoxAvl.txt", mode); 
    FILE *sesgadoxSplay = fopen("Resultados/sesgadoxSplay.txt", mode); 
    FILE *sesgadoxBtree16 = fopen("Resultados/sesgadoxBtree16.txt", mode); 
    FILE *sesgadoxBtree256 = fopen("Resultados/sesgadoxBtree256.txt", mode); 
    FILE *sesgadoxBtree4096 = fopen("Resultados/sesgadoxBtree4096.txt", mode);

    FILE *sesgadosqrtAbb = fopen("Resultados/sesgadosqrtAbb.txt", mode); 
    FILE *sesgadosqrtAvl = fopen("Resultados/sesgadosqrtAvl.txt", mode); 
    FILE *sesgadosqrtSplay = fopen("Resultados/sesgadosqrtSplay.txt", mode); 
    FILE *sesgadosqrtBtree16 = fopen("Resultados/sesgadosqrtBtree16.txt", mode); 
    FILE *sesgadosqrtBtree256 = fopen("Resultados/sesgadosqrtBtree256.txt", mode); 
    FILE *sesgadosqrtBtree4096 = fopen("Resultados/sesgadosqrtBtree4096.txt", mode);

    FILE *sesgadolnAbb = fopen("Resultados/sesgadolnAbb.txt", mode); 
    FILE *sesgadolnAvl = fopen("Resultados/sesgadolnAvl.txt", mode); 
    FILE *sesgadolnSplay = fopen("Resultados/sesgadolnSplay.txt", mode); 
    FILE *sesgadolnBtree16 = fopen("Resultados/sesgadolnBtree16.txt",mode); 
    FILE *sesgadolnBtree256 = fopen("Resultados/sesgadolnBtree256.txt", mode); 
    FILE *sesgadolnBtree4096 = fopen("Resultados/sesgadolnBtree4096.txt", mode);

    //arreglo con los archivos precomputados
    char *arreglo_archivos[10] = {"Values/values_0.txt", 
        "Values/values_1.txt", "Values/values_2.txt", "Values/values_3.txt", 
        "Values/values_4.txt", "Values/values_5.txt", "Values/values_6.txt", 
        "Values/values_7.txt", "Values/values_8.txt", "Values/values_9.txt"};


    long long * params[6];
    double * times[6];
    int cantIntervalos =  n/interval;
    double * timesAbb;
    double * timesAvl;
    double * timesSplay;
    double * timesBtree16;
    double * timesBtree256;
    double * timesBtree4096;

    printf("-------------------iteracion unica-------------------------\n");

    //calculo la secuencia
    initSecuencia();
    //precalculo los paramentros
    generate_params(params,arreglo_archivos[0],&p_x,&p_sqrt,&p_ln);
        

    printf("--------------------Aleatorio-----------------------\n");
    //ejecuto el experimento aleatorio para posteriormente guardar los tiempos obtenidos por cada arbol en su archivo respectivo
    ejecucion(times,params[0]); 

    timesAbb = times[0];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(aleatorioAbb,"%f",timesAbb[i]);
        fprintf(aleatorioAbb,"%s","\n");
    }

    timesAvl = times[1];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(aleatorioAvl,"%f",timesAvl[i]);
        fprintf(aleatorioAvl,"%s","\n");
    }

    timesSplay = times[2];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(aleatorioSplay,"%f",timesSplay[i]);
        fprintf(aleatorioSplay,"%s","\n");
    }

    timesBtree16 = times[3];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(aleatorioBtree16,"%f",timesBtree16[i]);
        fprintf(aleatorioBtree16,"%s","\n");
    }

    timesBtree256 = times[4];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(aleatorioBtree256,"%f",timesBtree256[i]);
        fprintf(aleatorioBtree256,"%s","\n");
    }

    timesBtree4096 = times[5];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(aleatorioBtree4096,"%f",timesBtree4096[i]);
        fprintf(aleatorioBtree4096,"%s","\n");
    }

    //libero los arreglos generados
    for(int i=0;i<6;i++){
        free(times[i]);
    }

    printf("Se guardaron los datos del aleatorio\n");
    printf("------------------Creciente 0.1-------------------------\n");
    //ejecuto el experimento creciente de 0.1 para posteriormente guardar los tiempos obtenidos por cada arbol en su archivo respectivo
    ejecucion(times,params[1]);

    timesAbb = times[0];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(creciente0p1Abb,"%f",timesAbb[i]);
        fprintf(creciente0p1Abb,"%s","\n");
    }

    timesAvl = times[1];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(creciente0p1Avl,"%f",timesAvl[i]);
        fprintf(creciente0p1Avl,"%s","\n");
    }

    timesSplay = times[2];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(creciente0p1Splay,"%f",timesSplay[i]);
        fprintf(creciente0p1Splay,"%s","\n");
    }

    timesBtree16 = times[3];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(creciente0p1Btree16,"%f",timesBtree16[i]);
        fprintf(creciente0p1Btree16,"%s","\n");
    }

    timesBtree256 = times[4];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(creciente0p1Btree256,"%f",timesBtree256[i]);
        fprintf(creciente0p1Btree256,"%s","\n");
    }

    timesBtree4096 = times[5];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(creciente0p1Btree4096,"%f",timesBtree4096[i]);
        fprintf(creciente0p1Btree4096,"%s","\n");
    }
    //libero los arreglos generados
    for(int i=0;i<6;i++){
        free(times[i]);
    }

    printf("Se guardaron los datos del crecientes 0.1\n");

    printf("----------------Creciente 0.5------------------------\n");
    //ejecuto el experimento creciente de 0.5 para posteriormente guardar los tiempos obtenidos por cada arbol en su archivo respectivo
    ejecucion(times,params[2]);

    timesAbb = times[0];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(creciente0p5Abb,"%f",timesAbb[i]);
        fprintf(creciente0p5Abb,"%s","\n");
    }

    timesAvl = times[1];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(creciente0p5Avl,"%f",timesAvl[i]);
        fprintf(creciente0p5Avl,"%s","\n");
    }

    timesSplay = times[2];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(creciente0p5Splay,"%f",timesSplay[i]);
        fprintf(creciente0p5Splay,"%s","\n");
    }

    timesBtree16 = times[3];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(creciente0p5Btree16,"%f",timesBtree16[i]);
        fprintf(creciente0p5Btree16,"%s","\n");
    }

    timesBtree256 = times[4];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(creciente0p5Btree256,"%f",timesBtree256[i]);
        fprintf(creciente0p5Btree256,"%s","\n");
    }

    timesBtree4096 = times[5];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(creciente0p5Btree4096,"%f",timesBtree4096[i]);
        fprintf(creciente0p5Btree4096,"%s","\n");
    }
    //libero los arreglos generados
    for(int i=0;i<6;i++){
        free(times[i]);
    }

    printf("Se guardaron los datos del crecientes 0.5\n");
    printf("--------------sesgado x-------------------------\n");
    //ejecuto el experimento sesgado p(x)=x para posteriormente guardar los tiempos obtenidos por cada arbol en su archivo respectivo
    ejecucion(times,params[3]); 

    timesAbb = times[0];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadoxAbb,"%f",timesAbb[i]);
        fprintf(sesgadoxAbb,"%s","\n");
    }

    timesAvl = times[1];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadoxAvl,"%f",timesAvl[i]);
        fprintf(sesgadoxAvl,"%s","\n");
    }

    timesSplay = times[2];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadoxSplay,"%f",timesSplay[i]);
        fprintf(sesgadoxSplay,"%s","\n");
    }

    timesBtree16 = times[3];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadoxBtree16,"%f",timesBtree16[i]);
        fprintf(sesgadoxBtree16,"%s","\n");
    }

    timesBtree256 = times[4];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadoxBtree256,"%f",timesBtree256[i]);
        fprintf(sesgadoxBtree256,"%s","\n");
    }

    timesBtree4096 = times[5];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadoxBtree4096,"%f",timesBtree4096[i]);
        fprintf(sesgadoxBtree4096,"%s","\n");
    }
    //libero los arreglos generados
    for(int i=0;i<6;i++){
        free(times[i]);
    }

    printf("Se guardaron los sesgados x\n");

    printf("----------------sesgado sqrt---------------------------\n");
    //ejecuto el experimento sesgado p(x)=sqrt(x) para posteriormente guardar los tiempos obtenidos por cada arbol en su archivo respectivo
    ejecucion(times,params[4]);

        timesAbb = times[0];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadosqrtAbb,"%f",timesAbb[i]);
        fprintf(sesgadosqrtAbb,"%s","\n");
    }

    timesAvl = times[1];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadosqrtAvl,"%f",timesAvl[i]);
        fprintf(sesgadosqrtAvl,"%s","\n");
    }

    timesSplay = times[2];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadosqrtSplay,"%f",timesSplay[i]);
        fprintf(sesgadosqrtSplay,"%s","\n");
    }

    timesBtree16 = times[3];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadosqrtBtree16,"%f",timesBtree16[i]);
        fprintf(sesgadosqrtBtree16,"%s","\n");
    }

    timesBtree256 = times[4];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadosqrtBtree256,"%f",timesBtree256[i]);
        fprintf(sesgadosqrtBtree256,"%s","\n");
    }

    timesBtree4096 = times[5];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadosqrtBtree4096,"%f",timesBtree4096[i]);
        fprintf(sesgadosqrtBtree4096,"%s","\n");
    }
    //libero los arreglos generados
    for(int i=0;i<6;i++){
        free(times[i]);
    }

    printf("Se guardaron los sesgados sqrt\n");

    printf("---------------sesgado ln-------------------------\n");
    //ejecuto el experimento sesgado p(x)=ln(x) para posteriormente guardar los tiempos obtenidos por cada arbol en su archivo respectivo
    ejecucion(times,params[5]);

        timesAbb = times[0];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadolnAbb,"%f",timesAbb[i]);
        fprintf(sesgadolnAbb,"%s","\n");
    }

    timesAvl = times[1];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadolnAvl,"%f",timesAvl[i]);
        fprintf(sesgadolnAvl,"%s","\n");
    }

    timesSplay = times[2];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadolnSplay,"%f",timesSplay[i]);
        fprintf(sesgadolnSplay,"%s","\n");
    }

    timesBtree16 = times[3];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadolnBtree16,"%f",timesBtree16[i]);
        fprintf(sesgadolnBtree16,"%s","\n");
    }

    timesBtree256 = times[4];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadolnBtree256,"%f",timesBtree256[i]);
        fprintf(sesgadolnBtree256,"%s","\n");
    }

    timesBtree4096 = times[5];
    for(int i=0;i<cantIntervalos;i++){
        fprintf(sesgadolnBtree4096,"%f",timesBtree4096[i]);
        fprintf(sesgadolnBtree4096,"%s","\n");
    }
    //libero los arreglos generados
    for(int i=0;i<6;i++){
        free(times[i]);
    }

    printf("Se guardaron los sesgados ln\n");

    for(int i=0;i<6;i++){
        free(params[i]);
    }

    printf("se terminaron :)\n");

    //cierro todos los archivos
    fclose(aleatorioAbb);
    fclose(aleatorioAvl);
    fclose(aleatorioSplay);
    fclose(aleatorioBtree16);
    fclose(aleatorioBtree256);
    fclose(aleatorioBtree4096);

    fclose(creciente0p1Abb);
    fclose(creciente0p1Avl);
    fclose(creciente0p1Splay);
    fclose(creciente0p1Btree16);
    fclose(creciente0p1Btree256);
    fclose(creciente0p1Btree4096);

    fclose(creciente0p5Abb);
    fclose(creciente0p5Avl);
    fclose(creciente0p5Splay);
    fclose(creciente0p5Btree16);
    fclose(creciente0p5Btree256);
    fclose(creciente0p5Btree4096);

    fclose(sesgadoxAbb);
    fclose(sesgadoxAvl);
    fclose(sesgadoxSplay);
    fclose(sesgadoxBtree16);
    fclose(sesgadoxBtree256);
    fclose(sesgadoxBtree4096);

    fclose(sesgadosqrtAbb);
    fclose(sesgadosqrtAvl);
    fclose(sesgadosqrtSplay);
    fclose(sesgadosqrtBtree16);
    fclose(sesgadosqrtBtree256);
    fclose(sesgadosqrtBtree4096);
    
    fclose(sesgadolnAbb);
    fclose(sesgadolnAvl);
    fclose(sesgadolnSplay);
    fclose(sesgadolnBtree16);
    fclose(sesgadolnBtree256);
    fclose(sesgadolnBtree4096);

}