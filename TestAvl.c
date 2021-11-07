#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "Avl.c"

clock_t start, end;
double cpu_time_used;

#define ins 0; //codigo de insercion
#define be 1; //codigo de busqueda exitosa
#define bi 2; //codigo de busqueda infructuosa

#define n 10 //largo de la secuencia

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
    Node *rootAVL = NULL; //iniacializo mi arbol
    long nodos[n/2]; //arreglo donde se guardara los nodos insertados
    long cant_nodos = 0; //cantidad de nodos
    long max_numero = pow(2,32); //maximo numero posible
    long random = 0; //numero obtenido al azar
    int operacion; //operacion obtenida de la secuencia

    start = clock(); //inicio el temporizador

    for (long i=0; i < n; i++){ //recorro mi secuencia
        operacion = secuencia[i]; //obtengo la operacion

        if (operacion==0){ //la operacion es una insercion

            do{ //busco un random hasta que no este en el arreglo de nodos
                random = (rand() % max_numero); //obtengo un numero al azar entre 0 y max_numero
            } while (search_in_array(nodos,random,cant_nodos));
            nodos[cant_nodos]=random; //agrego el nodo
            cant_nodos++; //aumento la cantidad
            insert(&rootAVL,random); //inserto el nodo en el arbol
            //printf("inserte el %lld\n",random);
        }
        else if (operacion==1){ //la operacion es una busqueda exitosa

            int index = (rand() % cant_nodos);//obtengo un numero random entre 0 y la cantidad de nodos
            random=nodos[index]; //obtengo el numero del nodo
            Node * node = search(&rootAVL,random);//busco el nodo en mi arbol
           // printf("busque exitosamente el %lld\n",random);
        }
        else{ //La operacion es una busqueda infructuosa

            do{ //busco un random hasta que no este en el arreglo de nodos
                random = (rand() % max_numero); //obtengo un numero al azar entre 0 y max_numero
            } while (search_in_array(nodos,random,cant_nodos));
            Node * node = search(&rootAVL,random);//busco el nodo en mi arbol
           // printf("busque fallidamente el %lld\n",random);
        }
    }
    end = clock(); //termino temporizador
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; //obtengo el tiempo de ejecucion
    return cpu_time_used;
}

//experimento creciente
double creciente(double factor){
    Node *rootAVL = NULL; //iniacializo mi arbol
    long nodos[n/2];//arreglo donde se guardara los nodos insertados
    long  m = 0; //cantidad de nodos
    long max_numero = pow(2,32);//maximo numero posible
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
            insert(&rootAVL,random);//inserto el nodo en el arbol
        }
        
        else if (operacion==1){ //la operacion es una busqueda exitosa

            int index = (rand() % m);//obtengo un numero random entre 0 y la cantidad de nodos
            random=nodos[index];//obtengo el numero del nodo
            Node * node = search(&rootAVL,random);//busco el nodo en mi arbol
            //printf("busque exitosamente el %lld\n",random);
        }
        else{ //La operacion es una busqueda infructuosa
            do{ //busco un random hasta que no este en el arreglo de nodos
                random = (rand() % max_numero);
            } while (search_in_array(nodos,random,m)); //obtengo un numero al azar entre 0 y max_numero
            Node * node = search(&rootAVL,random);//busco el nodo en mi arbol
            //printf("busque fallidamente el %lld\n",random);
        }
    }
    end = clock();//termino temporizador   
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;//obtengo el tiempo de ejecucion 
    return cpu_time_used;
}

double sesgada(double (*f)(double)){
    
    Node *rootAVL = NULL;//iniacializo mi arbol
    long nodos[n/2]; //arreglo donde se guardara los nodos insertados
    long cant_nodos = 0;//cantidad de nodos
    long max_numero = pow(2,32); //maximo numero posible
    long random = 0; //numero obtenido al azar
    int operacion; //operacion obtenida de la secuencia
    float prob[n/2]; //aca se guarda la prob acumulada en cada insercion
    float P = 0; // peso total de los p(x)
    float p_x = 0; //peso individula

    start = clock(); //inicio el temporizador

    for (long i=0; i < n; i++){ //recorro mi secuencia
        operacion = secuencia[i]; //obtengo la operacion

        if (operacion==0){ //la operacion es una insercion

            do{//busco un random hasta que no este en el arreglo de nodos
                random = (rand() % max_numero);//obtengo un numero al azar entre 0 y max_numero
            } while (search_in_array(nodos,random,cant_nodos));
            nodos[cant_nodos]=random; //agrego el nodo

            p_x = f(random); //sbtenemos el peso del nodo
            P=P+p_x; // sumamos p_x al peso global
            prob[cant_nodos]=P; //guargo la probabilidad aculada del momento

            cant_nodos++; //aumento la cantidad
            insert(&rootAVL,random); //inserto el nodo en el arbol
            //printf("inserte el %lld\n",random);
        }
        
        else if (operacion==1){ //la operacion es una busqueda exitosa

            float index = (rand() % (long)P);//obtengo un numero random entre 0 y P
            int pos = get_bigger(prob,index,cant_nodos);//obtengo la posicion del siguiente numero mas grande de prob
            random=nodos[pos]; //obtengo el nodo en esa posicion
            Node * node = search(&rootAVL,random);//busco el nodo en mi arbol
            //printf("busque exitosamente el %lld\n",random); 
        }
        else{ //La operacion es una busqueda infructuosa
            do{//busco un random hasta que no este en el arreglo de nodos
                random = (rand() % max_numero);//obtengo un numero al azar entre 0 y max_numero
            } while (search_in_array(nodos,random,cant_nodos));
            Node * node = search(&rootAVL,random);//busco el nodo en mi arbol
            //printf("busque fallidamente el %lld\n",random);
        }
    }
    end = clock();//termino temporizador   
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; //obtengo el tiempo de ejecucion
    return cpu_time_used;
}

int main(){
    srand(time(NULL)); 
    initSecuencia();   

    printf("--------------Experimento aleatorio--------------------------\n");
    FILE* ficheroAleatorioAvl;
    ficheroAleatorioAvl = fopen("Datos/aleatorioAvl.txt", "w");
    for (int i = 0; i < 100; i++){
        double timeA = aleatorio();
        fprintf(ficheroAleatorioAvl, "%f",timeA);
        if(i<99){
            fprintf(ficheroAleatorioAvl, ",");
        }        
    }
    fclose(ficheroAleatorioAvl);
    printf("\n");

    printf("--------------Experimento creciente con factor 0.1------------\n");
    FILE* ficheroCreciente_0p1_Avl;
    ficheroCreciente_0p1_Avl = fopen("Datos/creciente_0p1_Avl.txt", "w");
    for (int i = 0; i < 100; i++){
        double timeC_0p1 = creciente(0.1);
        fprintf(ficheroCreciente_0p1_Avl, "%f",timeC_0p1);
        if(i<99){
            fprintf(ficheroCreciente_0p1_Avl, ",");
        }        
    }
    fclose(ficheroCreciente_0p1_Avl);
    printf("\n");

    printf("--------------Experimento creciente con factor 0.5------------\n");
    FILE* ficheroCreciente_0p5_Avl;
    ficheroCreciente_0p5_Avl = fopen("Datos/creciente_0p5_Avl.txt", "w");
    for (int i = 0; i < 100; i++){
        double timeC_0p5 = creciente(0.5);
        fprintf(ficheroCreciente_0p5_Avl, "%f",timeC_0p5);
        if(i<99){
            fprintf(ficheroCreciente_0p5_Avl, ",");
        }        
    }
    fclose(ficheroCreciente_0p5_Avl);
    printf("\n");

    printf("--------------------Sesgado p(x)=x----------------------------\n");
    FILE* ficheroSesgado_x_Avl;
    ficheroSesgado_x_Avl = fopen("Datos/sesgado_x_Avl.txt", "w");
    for (int i = 0; i < 100; i++){
        double time_s_x = sesgada(&p_x);
        fprintf(ficheroSesgado_x_Avl, "%f",time_s_x);
        if(i<99){
            fprintf(ficheroSesgado_x_Avl, ",");
        }        
    }
    fclose(ficheroSesgado_x_Avl);
    printf("\n");

    printf("--------------------Sesgado p(x)=sqrt(x)------------------------\n");
    
    FILE* ficheroSesgado_sqrt_Avl;
    ficheroSesgado_sqrt_Avl = fopen("Datos/sesgado_sqrt_Avl.txt", "w");
    for (int i = 0; i < 100; i++){
       double time_s_sqrt = sesgada(&p_sqrt);
        fprintf(ficheroSesgado_sqrt_Avl, "%f",time_s_sqrt);
        if(i<99){
            fprintf(ficheroSesgado_sqrt_Avl, ",");
        }        
    }
    fclose(ficheroSesgado_sqrt_Avl);
    printf("\n");

    printf("--------------------Sesgado p(x)=ln(x)---------------------------\n");
    
    FILE* ficheroSesgado_ln_Avl;
    ficheroSesgado_ln_Avl = fopen("Datos/sesgado_ln_Avl.txt", "w");
    for (int i = 0; i < 100; i++){
       double time_s_ln = sesgada(&p_ln);
        fprintf(ficheroSesgado_ln_Avl, "%f",time_s_ln);
        if(i<99){
            fprintf(ficheroSesgado_ln_Avl, ",");
        }        
    }
    fclose(ficheroSesgado_ln_Avl);
    printf("\n");


}