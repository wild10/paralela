  #include<stdio.h>
  #include<stdlib.h>
  // #include "vector.h"

  #define size 96232132

  int main(){

    //declarar tipo file para leer file
    FILE *archivo;

    // double  array[size];
    // Array allocation
    double* array  = malloc(sizeof(double)*size);
    // vector <double> array;
    double i,n;

    // abrir fichero direccion
    archivo = fopen("list.txt","rt");

    i = fscanf(archivo,"%lf",&n); //escanear los datos del archivo
    int k = 0;
    while(i != EOF ){ //si no es el final
      // printf("a = %lf \n", n);
       array[k] = n; //asignadar datos a nuestro archivo
      // array.push_back(n);
      i = fscanf(archivo, "%lf", &n); // seguir escaneando
      k++;
    }

    printf("ESCRIBIR LOS DATOS DEL ARCHIVO EN EN ARRAY \n");
    printf("%lf\n",array[size-1] );
/*
    for(int j = 0; j < size; j++){
      printf("%d :%lf\n",j,array[j] );
    }
*/
    fclose(archivo);
/*
    int leng = sizeof(array)/sizeof(array[0]);
    printf("leng: %d \n",leng);
*/
   return 0;
  }
