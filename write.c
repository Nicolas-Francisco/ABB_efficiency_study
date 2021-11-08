#include<stdio.h>

int main() {
    /*
     * Podemos poner la ruta completa o el nombre, si ponemos
     * solo el nombre entonces se refiere al directorio en donde
     * se ejecuta el programa
     *
     */
    char *nombreArchivo = "Resultados/aleatorioAbb.txt";
    char *modo = "a";// w es para sobrescribir, a+ es para añadir al existente
    FILE *archivo = fopen(nombreArchivo, modo);
    
    for(int i=0;i<10;i++){
        fprintf(archivo, "%d",i);
        fprintf(archivo,"%s","\n");
    }
    fclose(archivo);
    return 0;
}