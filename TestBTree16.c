#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include "BTree16.c"

clock_t start, end;
double cpu_time_used;

#define ins 0;  // insertion
#define be 1;   // busqueda exitosa
#define bi 2;   // busqueda infructuosa

#define n 10        // sequence size
#define pi 0.5      // probabilidad de insercion
#define pbe 0.33    // probabilidad de busqueda exitosa
#define pbi 0.17    // probabilidad de busqueda infructuosa

long cant_in=(long)(n*pi);  // cantidad de inserciones
long cant_be=(long)(n*pbe); // cantidad de busquedas exitosas
long cant_bi=(long)(n*pbi); // cantidad de bisquedas infructuosas

// arreglo de largo 100 que cant_nodos tendra las operaciones
int secuencia[n];

// Nodos con probabilidad lineal
double p_x(double x){ 
    return x;
}

// Nodos con probabilidad raiz
double p_sqrt(double x){ 
    return sqrt(x);
}

// Nodos con probabilidad logarítmica
double p_ln(double x){ 
    return log(x);
}

// Función que busca en un arreglo, si lo encuentra retorna 1, sino 0
int search_in_array(long arreglo[], long busqueda, long ene) {
    for (int i = 0; i < ene; i++) {
        if (arreglo[i] == busqueda) return 1;
    }
    return 0;
}

// Función que encuentra la posicion del entero mas cercano a busqueda
long get_bigger(float arreglo[], float busqueda, long ene ){
    for (int i = 0; i < ene; i++) {
        if (arreglo[i] >= busqueda) return i;
    }
    return -1;
}

// Generador de la secuencia que desordena intrucciones
void initSecuencia(){
    int secuencia_ordenado[n];      // [0,0,0,0,0,1,1,1,2,2]
    // Se colocan las inserciones
    for (int i=0; i<cant_in ; i++) 
        secuencia_ordenado[i]=ins;

    // Se colocan las busquedas exitosas
    for (int i=cant_in ; i<cant_in+cant_be ; i++)
        secuencia_ordenado[i]=be;

    // Se colocan las busquedas infructuosas
    for (int i=cant_in+cant_be ; i<n ; i++) 
        secuencia_ordenado[i]=bi;
    
    // desordenamos el arreglo

    // arreglo que indica si el elemento asociado a esa possición fue desordenado
    // y colocamos el primer elemento en 1 para que no sea movido
    int usado[n];       
    usado[0]=1;         
    //inicializamos el resto con 0
    for (int i=1 ; i<n; i++){ 
        usado[i]=0;
    }          

    int index=1;

    for (int i=1 ; i<n ; i++){ 
        // Se escoge un indice al azar mientras no haya sido movido antes
        do{ 
            index = (rand() % n);
        }
        while (usado[index]);
        // El elemento se guarda en la posicion i, y
        // se marca la posicion para indicar que ya fue movido
        secuencia[i] = secuencia_ordenado[index]; 
        usado[index]=1; 
    }
}

// EXPERIMENTO ALEATORIO
double aleatorio(){
    // Se inicializa el árbol
    Node* rootBTree = NULL; 

    long nodos[n/2];                // Array donde se guardara los nodos insertados
    long cant_nodos = 0;            // Cantidad de nodos
    long max_numero = pow(2,32);    // Maximo numero posible
    long random = 0;                // Numero obtenido al azar
    int operacion;                  // Operacion obtenida de la secuencia

    start = clock();                // Temporizador

    for (long i=0; i < n; i++){   
        // Sacamos la operacion guardada aleatoriamente
        operacion = secuencia[i]; 

        // CASO DE INSERCION
        if (operacion == 0){ 
            // Se busca un numero aleatorio hasta que no esté en el arbol
            do{ 
                // Se obtiene un numero al azar entre 0 y max_numero
                random = (rand() % max_numero); 
            } while (search_in_array(nodos, random, cant_nodos));

            // Se inserta el nodo i se guarda
            nodos[cant_nodos] = random; 
            cant_nodos++; 
            insert(&rootBTree, random);
            //printf("inserte el %lld\n",random);
        }

        // CASO DE BUSQUEDA EXITOSA
        else if (operacion==1){ 
            // Se obtiene un numero aleatorio entre 0 y el máximo, y se busca
            int index = (rand() % cant_nodos);
            random=nodos[index];
            Node * node = search(&rootBTree,random);
           // printf("busque exitosamente el %lld\n",random);
        }

        // CASO DE BUSQUEDA INFRUCTUOSA
        else{ 
            // Se busca aleatoriamente hasta encontrar un valor que no está
            do{ 
                random = (rand() % max_numero); 
            } while (search_in_array(nodos,random,cant_nodos));
            // Se busca infructuosamente
            Node * node = search(&rootBTree,random);
        }
    }
    // Se apaga el temporizador y se calcula el tiempo de ejecución
    end = clock(); 
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; 
    return cpu_time_used;
}

// EXPERIMENTO CRECIENTE
double creciente(double factor){
    // Se inicializa el árbol
    Node* rootBTree = NULL; 

    long nodos[n/2];                // Arreglo donde se guardara los nodos insertados
    long  m = 0;                    // Cantidad de nodos
    long max_numero = pow(2,32);    // Maximo numero posible
    long random = 0;                // Numero obtenido al azar
    int operacion;                  // Operacion obtenida de la secuencia
    double k = 0;                   

    start = clock();                // Temporizador

    for (long i=0; i < n; i++){ 
        // Sacamos la operacion guardada aleatoriamente
        operacion = secuencia[i]; 

        // CASO DE INSERCION
        if (operacion==0){ 
            // Se busca aleatoriamente hasta que se encuentre un valor inexistente
            do{ 
                if (k>=1){
                    random = (rand() % (int)k);     // No al azar entre 0 y k
                    random=random+m;                // Se suma m al random
                }
                else{
                    random=0+m;
                }
            } while (search_in_array(nodos,random,m));

            // Se agrega a la lista y se inserta
            nodos[m]=random;
            m++;
            k=m*factor;
            insert(&rootBTree,random);
        }
        
        // CASO DE BUSQUEDA EXITOSA
        else if (operacion==1){
            // Se obtiene un numero aleatorio entre 0 y el máximo, y se busca
            int index = (rand() % m);
            random=nodos[index];
            Node * node = search(&rootBTree,random);
        }
        
        // CASO DE BUSQUEDA INFRUCTUOSA
        else{ 
            // Se busca aleatoriamente hasta encontrar un valor que no está
            do{ 
                random = (rand() % max_numero); 
            } while (search_in_array(nodos, random, m));
            // Se busca infructuosamente
            Node * node = search(&rootBTree, random);
        }
    }
    // Se apaga el temporizador y se calcula el tiempo de ejecución
    end = clock(); 
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; 
    return cpu_time_used;
}

// EXPERIMENTO SESGADO
double sesgada(double (*f)(double)){
    // Se inicializa el árbol
    Node* rootBTree = NULL; 

    long nodos[n/2];                // Arreglo donde se guardara los nodos insertados
    long cant_nodos = 0;            // Cantidad de nodos
    long max_numero = pow(2,32);    // Maximo numero posible
    long random = 0;                // Numero obtenido al azar
    int operacion;                  // Operacion obtenida de la secuencia
    float prob[n/2];                // Prob acumulada en cada insercion
    float P = 0;                    // Peso total P(x)
    float p_x = 0;                  // Peso individul

    start = clock();                // Temporizador

    for (long i=0; i < n; i++){
        // Sacamos la operacion guardada aleatoriamente
        operacion = secuencia[i]; 

        // CASO DE INSERCION
        if (operacion==0){
            // Se busca aleatoriamente hasta que se encuentre un valor inexistente
            do{
                random = (rand() % max_numero); 
            } while (search_in_array(nodos,random,cant_nodos));

            // Se agrega el nodo
            nodos[cant_nodos]=random;

            // Se calcula y guarda su peso
            p_x = f(random); 
            P=P+p_x; 
            prob[cant_nodos]=P; 
            cant_nodos++; 

            // Finalemente se inserta en el arbol
            insert(&rootBTree,random); 
        }
        
        // CASO BUSQUEDA EXITOSA
        else if (operacion==1){ 
            float index = (rand() % (long)P);               // Se obtiene un numero entre 0 y P
            int pos = get_bigger(prob,index,cant_nodos);    // Posicion del menor mayor que index
            random=nodos[pos];                              // Se obtiene el nodo en esa posicion
            Node * node = search(&rootBTree,random);        // Se busca el nodo
        }
        
        // CASO BUSQUEDA INFRUCTUOSA
        else{ 
            // Se busca aleatoriamente hasta que el nodo no esté en el arbol
            do{
                random = (rand() % max_numero);
            } while (search_in_array(nodos,random,cant_nodos));

            // Se busca en el arbol
            Node * node = search(&rootBTree,random);
        }
    }
    // Se apaga el temporizador y se calcula el tiempo de ejecución
    end = clock(); 
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; 
    return cpu_time_used;
}

int main(){
    srand(time(NULL)); 
    initSecuencia();   

    printf("--------------------------- Experimento aleatorio ---------------------------\n");
    FILE* ficheroAleatorioBTree16;
    ficheroAleatorioBTree16 = fopen("Datos/aleatorioBTree16.txt", "w");
    for (int i = 0; i < 100; i++){
        double timeA = aleatorio();
        fprintf(ficheroAleatorioBTree16, "%f",timeA);
        if(i<99){
            fprintf(ficheroAleatorioBTree16, ",");
        }        
    }
    fclose(ficheroAleatorioBTree16);
    printf("\n");

    printf("------------------- Experimento creciente con factor 0.1 -------------------\n");
    FILE* ficheroCreciente_0p1_BTree16;
    ficheroCreciente_0p1_BTree16 = fopen("Datos/creciente_0p1_BTree16.txt", "w");
    for (int i = 0; i < 100; i++){
        double timeC_0p1 = creciente(0.1);
        fprintf(ficheroCreciente_0p1_BTree16, "%f",timeC_0p1);
        if(i<99){
            fprintf(ficheroCreciente_0p1_BTree16, ",");
        }        
    }
    fclose(ficheroCreciente_0p1_BTree16);
    printf("\n");

    printf("------------------- Experimento creciente con factor 0.5 -------------------\n");
    FILE* ficheroCreciente_0p5_BTree16;
    ficheroCreciente_0p5_BTree16 = fopen("Datos/creciente_0p5_BTree16.txt", "w");
    for (int i = 0; i < 100; i++){
        double timeC_0p5 = creciente(0.5);
        fprintf(ficheroCreciente_0p5_BTree16, "%f",timeC_0p5);
        if(i<99){
            fprintf(ficheroCreciente_0p5_BTree16, ",");
        }        
    }
    fclose(ficheroCreciente_0p5_BTree16);
    printf("\n");

    printf("------------------------------ Sesgado p(x)=x ------------------------------\n");
    FILE* ficheroSesgado_x_BTree16;
    ficheroSesgado_x_BTree16 = fopen("Datos/sesgado_x_BTree16", "w");
    for (int i = 0; i < 100; i++){
        double time_s_x = sesgada(&p_x);
        fprintf(ficheroSesgado_x_BTree16, "%f",time_s_x);
        if(i<99){
            fprintf(ficheroSesgado_x_BTree16, ",");
        }        
    }
    fclose(ficheroSesgado_x_BTree16);
    printf("\n");

    printf("--------------------------- Sesgado p(x)=sqrt(x) ---------------------------\n");
    
    FILE* ficheroSesgado_sqrt_BTree16;
    ficheroSesgado_sqrt_BTree16 = fopen("Datos/sesgado_sqrt_BTree16", "w");
    for (int i = 0; i < 100; i++){
       double time_s_sqrt = sesgada(&p_sqrt);
        fprintf(ficheroSesgado_sqrt_BTree16, "%f",time_s_sqrt);
        if(i<99){
            fprintf(ficheroSesgado_sqrt_BTree16, ",");
        }        
    }
    fclose(ficheroSesgado_sqrt_BTree16);
    printf("\n");

    printf("---------------------------- Sesgado p(x)=ln(x) ----------------------------\n");
    
    FILE* ficheroSesgado_ln_BTree16;
    ficheroSesgado_ln_BTree16 = fopen("Datos/sesgado_ln_BTree16", "w");
    for (int i = 0; i < 100; i++){
       double time_s_ln = sesgada(&p_ln);
        fprintf(ficheroSesgado_ln_BTree16, "%f",time_s_ln);
        if(i<99){
            fprintf(ficheroSesgado_ln_BTree16, ",");
        }        
    }
    fclose(ficheroSesgado_ln_BTree16);
    printf("\n");
}