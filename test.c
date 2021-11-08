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

#define pi 0.5 //probabilidad de insercion
#define pbe 0.33 //probabilidad de busqueda exitosa
#define pbi 0.17 //probabilidad de busqueda infructuosa

long long cant_in=(long long)(n*pi); //cantidad de inserciones
long long cant_be=(long long)(n*pbe); //cantidad de busquedas exitosas
long long cant_bi=(long long)(n*pbi); //cantidad de bisquedas infructuosas

int secuencia[n]; //areglo que almacenara la secuencia

//busca un elemento en un arreglo, si lo encuentra retorna 1 sino entrega 0
int search_in_array(long long arreglo[], long long busqueda, long long ene) {
    for (int i = 0; i < ene; i++) {
        if (arreglo[i] == busqueda) return 1;
    }
    return 0;
}

//genera un random no repetido en una lista
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

void generate_params(long long ** buffer,double (*f_x)(double),
double (*f_sqrt)(double),double (*f_ln)(double)){
    long long *nodosAleatorios= (long long *)malloc(sizeof(long long)*(n/2));
    long long *paramsAleatorios = (long long *)malloc(sizeof(long long)*n);

    long long *nodosCreciente_0p1= (long long *)malloc(sizeof(long long)*(n/2));
    long long *paramsCreciente_0p1 = (long long *)malloc(sizeof(long long)*n);

    long long *nodosCreciente_0p5= (long long *)malloc(sizeof(long long)*(n/2));
    long long *paramsCreciente_0p5 = (long long *)malloc(sizeof(long long)*n);

    long long *nodosSesgado_x= (long long *)malloc(sizeof(long long)*(n/2));
    long long *paramsSesgado_x = (long long *)malloc(sizeof(long long)*n);
    float *probSesgado_x = (float *)malloc(sizeof(float)*(n/2));

    long long *nodosSesgado_sqrt= (long long *)malloc(sizeof(long long)*(n/2));
    long long *paramsSesgado_sqrt = (long long *)malloc(sizeof(long long)*n);
    float *probSesgado_sqrt = (float *)malloc(sizeof(float)*(n/2));

    long long *nodosSesgado_ln= (long long *)malloc(sizeof(long long)*(n/2));
    long long *paramsSesgado_ln = (long long *)malloc(sizeof(long long)*n);
    float *probSesgado_ln = (float *)malloc(sizeof(float)*(n/2));

    long long index = 0; //indice de operacion
    long long cant_nodos=0;
    long long max_numero = pow(2,32); //maximo numero posible
    long long random = 0; //numero obtenido al azar
    long long random_0yk = 0;
    int operacion; //operacion obtenida de la secuencia

    double k_0p1 =0; 
    double k_0p5= 0;
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


    printf("Loaging\n");
    
    fp = fopen("Values/values_0.txt","r");

    while ((read = getline(&line, &len, fp)) != -1){
        random = atol(line);
        operacion = secuencia[index];
        if (operacion==0){

            nodosAleatorios[cant_nodos]=random; 
            paramsAleatorios[index]=random;
           
            factor_k =k_0p1/max_numero;
            random_0yk =(long long) random*factor_k;
            nodosCreciente_0p1[cant_nodos]=random_0yk+cant_nodos; 
            paramsCreciente_0p1[index]=random_0yk+cant_nodos;

            
            factor_k =k_0p5/max_numero;
            random_0yk = (long long)(random*factor_k);
            nodosCreciente_0p5[cant_nodos]=random_0yk+cant_nodos; 
            paramsCreciente_0p5[index]=random_0yk+cant_nodos;
   

            nodosSesgado_x[cant_nodos]=random;
            paramsSesgado_x[index]=random;
            peso = f_x(random);
            P_x=P_x+peso;
            probSesgado_x[cant_nodos]=P_x;

            nodosSesgado_sqrt[cant_nodos]=random;
            paramsSesgado_sqrt[index]=random;
            peso = f_sqrt(random);
            P_sqrt=P_sqrt+peso;
            probSesgado_sqrt[cant_nodos]=P_sqrt;

            nodosSesgado_ln[cant_nodos]=random;
            paramsSesgado_ln[index]=random;
            peso = f_ln(random);
            P_ln=P_ln+peso;
            probSesgado_ln[cant_nodos]=P_ln;
        
            cant_nodos++;
            k_0p1 = cant_nodos*0.1;
            k_0p5 = cant_nodos*0.5;
            index++; 


   

        }
        else if (operacion==1){
          
            long long indice = (rand() % cant_nodos);
            //long long indice = atol(line);
            //indice = indice*(cant_nodos/max_numero);
            //printf("el indice pa buscar exitosamente es %lld\n",indice);
            //long long indice = 0;

            random=nodosAleatorios[indice];
            paramsAleatorios[index]=random;

            
            random=nodosCreciente_0p1[indice];
            paramsCreciente_0p1[index]=random;


            random=nodosCreciente_0p5[indice];
            paramsCreciente_0p5[index]=random;


            pesoAleatorio = (P_x/cant_nodos)*indice;
            pos = get_bigger(probSesgado_x,pesoAleatorio,cant_nodos);
            random=nodosSesgado_x[pos];
            paramsSesgado_x[index]=random;
            

            pesoAleatorio = (P_sqrt/cant_nodos)*indice;
            pos = get_bigger(probSesgado_sqrt,pesoAleatorio,cant_nodos);
            random=nodosSesgado_sqrt[pos];
            paramsSesgado_sqrt[index]=random;

            pesoAleatorio = (P_ln/cant_nodos)*indice;
            pos = get_bigger(probSesgado_ln,pesoAleatorio,cant_nodos);
            random=nodosSesgado_ln[pos];
            paramsSesgado_ln[index]=random;

            index++;
            
        }
        else{
            paramsAleatorios[index]=random;
           
            paramsCreciente_0p1[index]=random;

            paramsCreciente_0p5[index]=random;

            paramsSesgado_x[index]=random;

            paramsSesgado_sqrt[index]=random;

            paramsSesgado_ln[index]=random;
     

            index++;
        }

        if (index%10000 == 0){
            printf("[%lld,%lld]\n",index,n);
        }
    }
    fclose(fp);
    buffer[0]=paramsAleatorios;
    buffer[1]=paramsCreciente_0p1;
    buffer[2]=paramsCreciente_0p5;
    buffer[3]=paramsSesgado_x;
    buffer[4]=paramsSesgado_sqrt;
    buffer[5]=paramsSesgado_ln;

}


// ejecuta las operaciones en los arboles
void ejecucion(double times[],long long params[]){
    NodeAbb *rootABB = NULL; // inicializo arbol ABB
    NodeAvl *rootAVL = NULL; // inicializo arbol AVL
    //Node *rootSPLAY = NULL; // inicializo arbol SLAY
    NodeBt_16 *rootBt_16 = NULL; // inicializo arbol Btree 16
    NodeBt_256 *rootBt_256 = NULL; // inicializo arbol Btree 256
    NodeBt_4096 *rootBt_4096 = NULL; // inicializo arbol Btree 4096

    long long param; //parametro a usar en la operacion
    int operacion; //operacion de la secuencia
    
    //----------- Arbol Abb ------------
    printf("ABB\n");

    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){ 
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
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    times[0]=cpu_time_used;

    //----------- Arbol Avl ------------
    printf("AVL\n");

    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){ 
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
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    times[1]=cpu_time_used;

   //----------- Arbol Splay ------------

    printf("Splay\n");
    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){ 
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
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    times[2]=cpu_time_used;

    //----------- Arbol Btree 16 ------------

    printf("Btree 16\n");
    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){ 
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
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; 
    times[3]=cpu_time_used;

    //----------- Arbol Btree 256 ------------
    printf("Btree 256\n");

    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){ 
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
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; 
    times[4]=cpu_time_used;

    //----------- Arbol Btree 4096 ------------
    printf("Btree 4096\n");
    

    start = clock(); //inicio el temporizador
    for (long long i=0; i < n; i++){
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
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; 
    times[5]=cpu_time_used;
}

int main(){
    srand(time(NULL)); 
    initSecuencia();
    long long *params[6];
    generate_params(params,&p_x,&p_sqrt,&p_ln);
    double times[6];

    printf("----------------------------------------------------\n");
    ejecucion(times,params[0]); // se ejecuta el aleatorio
    printf("El tiempo aleatorio de ABB es %f \n",times[0]);
    printf("El tiempo aleatorio de AVL es %f \n",times[1]);
    printf("El tiempo aleatorio de SPLAY es %f \n",times[2]);
    printf("El tiempo aleatorio de BTREE 16 es %f \n",times[3]);
    printf("El tiempo aleatorio de BTREE 256 es %f \n",times[4]);
    printf("El tiempo aleatorio de BTREE 4096 es %f \n",times[5]);

    
    printf("----------------------------------------------------\n");
    ejecucion(times,params[1]); // se ejecura el creciente de 0.1;
    printf("El tiempo creciente 0.1 de ABB es %f \n",times[0]);
    printf("El tiempo creciente 0.1 de AVL es %f \n",times[1]);
    printf("El tiempo creciente 0.1 de SPLAY es %f \n",times[2]);
    printf("El tiempo creciente 0.1 de BTREE 16 es %f \n",times[3]);
    printf("El tiempo creciente 0.1 de BTREE 256 es %f \n",times[4]);
    printf("El tiempo creciente 0.1 de BTREE 4096 es %f \n",times[5]);

    
    printf("----------------------------------------------------\n");
    ejecucion(times,params[2]); // se ejecura el creciente de 0.5;
    printf("El tiempo creciente 0.5 de ABB es %f \n",times[0]);
    printf("El tiempo creciente 0.5 de AVL es %f \n",times[1]);
    printf("El tiempo creciente 0.5 de SPLAY es %f \n",times[2]);
    printf("El tiempo creciente 0.5 de BTREE 16 es %f \n",times[3]);
    printf("El tiempo creciente 0.5 de BTREE 256 es %f \n",times[4]);
    printf("El tiempo creciente 0.5 de BTREE 4096 es %f \n",times[5]);
    printf("----------------------------------------------------\n");

    
   
    ejecucion(times,params[3]); // se ejecura el csesgado p(x);

    
    printf("El tiempo sesgado p(x) de ABB es %f \n",times[0]);
    printf("El tiempo sesgado p(x) de AVL es %f \n",times[1]);
    printf("El tiempo sesgado p(x) de SPLAY es %f \n",times[2]);
    printf("El tiempo sesgado p(x) de BTREE 16 es %f \n",times[3]);
    printf("El tiempo sesgado p(x) de BTREE 256 es %f \n",times[4]);
    printf("El tiempo sesgado p(x) de BTREE 4096 es %f \n",times[5]);

    printf("----------------------------------------------------\n");
    ejecucion(times,params[4]); // se ejecura el sesgado sqrt(x);
    printf("El tiempo sesgado sqrt(x) de ABB es %f \n",times[0]);
    printf("El tiempo sesgado sqrt(x) de AVL es %f \n",times[1]);
    printf("El tiempo sesgado sqrt(x) de SPLAY es %f \n",times[2]);
    printf("El tiempo sesgado sqrt(x) de BTREE 16 es %f \n",times[3]);
    printf("El tiempo sesgado sqrt(x) de BTREE 256 es %f \n",times[4]);
    printf("El tiempo sesgado sqrt(x) de BTREE 4096 es %f \n",times[5]);
    printf("----------------------------------------------------\n");
 
    
    ejecucion(times,params[5]); // se ejecura el sesgado ln(x);
    printf("El tiempo sesgado ln(x) de ABB es %f \n",times[0]);
    printf("El tiempo sesgado ln(x) de AVL es %f \n",times[1]);
    printf("El tiempo sesgado ln(x) de SPLAY es %f \n",times[2]);
    printf("El tiempo sesgado ln(x) de BTREE 16 es %f \n",times[3]);
    printf("El tiempo sesgado ln(x) de BTREE 256 es %f \n",times[4]);
    printf("El tiempo sesgado ln(x) de BTREE 4096 es %f \n",times[5]);
    


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