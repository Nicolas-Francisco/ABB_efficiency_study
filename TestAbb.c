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
#define n 10

#define pi 0.5
#define pbe 0.33
#define pbi 0.17

int cant_in=round(n*pi);
int cant_be=round(n*pbe);
int cant_bi=round(n*pbi);

int secuencia[n];

double p_x(double x){
    return x;
}
double p_sqrt(double x){
    return sqrt(x);
}
double p_ln(double x){
    return log(x);
}

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
                //printf("k es %f \n",k);
                //printf("el k usado pal random es %d \n",(int)k);
                if (k>=1){
                    //printf("k distinto a 0\n");
                    random = (rand() % (int)k); // busco un numero entre 0 y k
                    random=random+m; //k+m
                }
                else{
                    //printf("k es 0\n");
                    random=0+m;
                }
                
            } while (search_in_array(numeros,random,m));
            numeros[m]=random;
            m++;
            k=m*factor;
            
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

double sesgada(double (*f)(double)){
    printf("--------------Sesgado-------------------------\n");
    Node *rootABB = NULL;
    long numeros[n/2]; //aca se guarda los numeros insertados
    long ene = (long)n;
    long size = pow(2,32)*(ene/2)-(((ene/2)*(ene/2 + 1))/2);
    printf("el tam max es %lld\n",size);
    //long numeros_prob[size]; // lista con los numeros probables
    long pos = 0;  //lugar donde insertar en numeros_prob
    long * numeros_prob = (long*)malloc(sizeof(long)*n);

    long cont = 0;   // cantidad de nodos en el arbol
    long max = pow(2,32); // numero maximo posible
    long random = 0; //numero random
    start = clock(); //inicio del timer
    long P = 0; // peso total de los  p(x)
    long p_x = 0;
    numeros_prob[2] = 177;
    for (int i=0; i < n; i++){
        numeros_prob[i]=0;
        printf("a %lld\n",i);
    }



    /*
    for (long i=0; i < n; i++){ //recorro mi secuencia
        int operation = secuencia[i];
        if (operation==0){ //insercion en los arboles
            printf("aca inserto\n");
            
            do{
                random = (rand() % max);
            } while (search_in_array(numeros,random,cont));
            numeros[cont]=random;
            cont++;
            insert(&rootABB,random);
            
            p_x = f(random); //Obtenemos el peso
            P=P+p_x; // sumamos al peso global
            long hasta =pos+(long)p_x ;
            printf("hasta es %lld\n",hasta);
            //for (long i=0; i <= 10; i++){
               // printf("acaaaa");
                // numeros_prob[i]=0;
            //}
            pos=pos+(long)p_x;
            printf("pos es %lld\n",pos);

            printf("inserte el %lld\n",random);
        }
        
        else if (operation==1){ //busqueda exitosa
            printf("aca se busqueda exitosa\n");
        }
        else{ //busqueda infructuosa
            printf("aca se busqueda infructuosa\n");
        }
    }
    end = clock();   
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; 
    return cpu_time_used;
    */
   return 1;
}

int main(){
    srand(time(NULL)); 
    initSecuencia();   
    /* 
    double timeA = aleatorio();
    printf("El tiempo de ejecucion es %f\n",timeA);
    double timeC_0p1 = creciente(0.1);
    printf("El tiempo de ejecucion es %f\n",timeC_0p1);
    double timeC_0p5 = creciente(0.5);
    printf("El tiempo de ejecucion es %f\n",timeC_0p5);
    */
    double time_s = sesgada(&p_x);
    printf("El tiempo de ejecucion es %f\n",time_s);
}