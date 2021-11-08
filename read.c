#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(void)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    long int value;

    fp = fopen("Values/values_0.txt", "r");
    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
        //printf("%s", line);
        value = atol(line);
        value = value +1;
        //printf("%li\n", value);
        printf("caca %lld\n",value);
    }

    fclose(fp);
}