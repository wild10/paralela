#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <time.h>

#define SMALL    32  // Arrays size <= SMALL switches to insertion sort

void merge(float a[], int size, float temp[]);
void insertion_sort(float a[], int size);
void mergesort_serial(float a[], int size, float temp[]);
void mergesort_parallel_omp(float a[], int size, float temp[], int threads);
void run_omp(float a[], int size, float temp[], int threads);
void write(float a[], int size);
int main(int argc, char* argv[]);



int main(int argc, char* argv[]) {
    puts("-OpenMP Recursive Mergesort-\t");
    // check de ejecucion
    if ( argc != 3 ) /* argc debe tener 3 apropiada! */
    {
        printf( "ingrese: %s array-size numero-de-threads\n", argv[0]);
        return 1;
    }
	// Get args
    int size = atoi(argv[1]); // Array size
    int threads = atoi(argv[2]); // numero de threads consola
	 // Check nested parallelism availability
	  omp_set_nested(1);
    if (omp_get_nested() !=1 )
    {
		puts("Advertencia: el paralelismo anidado es deseado pero no está disponible");
    }
	// Check procesador y threads
    int processors = omp_get_num_procs(); // Available processors
    // printf("Array size = %d\nProcesos = %d\nProcesadores = %d\n",
		// size, threads, processors);

    if (threads > processors)
    {
        printf( "Advertencia: %d threads solicitados, ejecutara run_omp on %d procesadores disponibles\n",
			threads, processors);
		omp_set_num_threads(threads);
    }
    int max_threads = omp_get_max_threads(); // Max threds disponibles
    if (threads > max_threads) //threads solicitados tienen mas d un maximo disponible
    {
        printf( "Error: no puede usar %d threads, solo %d threads disponibles\n",
		threads, max_threads);
		return 1;
	  }

    // Array allocation
    float* a    = malloc(sizeof(float)*size);
    float* temp = malloc(sizeof(float)*size);
    if (a == NULL || temp == NULL)
    {
		printf( "Error: no puedo reservar memoria para el array %d\n", size);
		return 1;
    }

    FILE *archivo; // declarar tipo file para leer
    float n,dato;
    int k = 0;
    archivo = fopen("list.txt","rt"); //abrir fichero
    dato = fscanf(archivo,"%f",&n); // escanear los datos desde el archivo

    while( dato != EOF){
      a[k] = n;
      dato = fscanf(archivo,"%f",&n); // escanear los datos desde el archivo
      k++;
    }
    fclose(archivo);



    // int i;
    // Random array

    int i;
    // srand(time(NULL));
    // for(i=0; i<size; i++) {
    //     a[i] = rand() % size;
    // }


    //escribir arreglo
    // write(a, size);

	  // Sort
    float start = omp_get_wtime();
  	run_omp(a, size, temp, threads);
    float end = omp_get_wtime( );
    printf("Start = %.2f\nEnd = %.2f\nTiempo paralelo = %.2f\n",
		start, end, end - start);
    //escribir arreglo ordenado
     // write(a, size);
	// Result check
    for(i=1; i<size; i++) {
        if (!((float)a[i-1] <= (float)a[i])) {
            printf("Implementacion error: a[%d]=%lf > a[%d]=%lf\n", i-1, a[i-1], i, a[i]);
            return 1;
        }
    }
    puts("-exitoso-");
    float wtick = omp_get_wtick( );
    printf("Wtick = %.8f\n1/Wtick = %.8f\n",
		wtick, 1.0 / wtick);
    return 0;
}

// Driver
void run_omp(float a[], int size, float temp[], int threads) {
    // Enable nested parallelism, if available
    omp_set_nested(1);
    // Parallel mergesort
    mergesort_parallel_omp(a, size, temp, threads);
}

// OpenMP merge sort with given number of threads
void mergesort_parallel_omp(float a[], int size, float temp[], int threads) {
    if ( threads == 1) {
//        printf("Thread %d begins serial merge sort\n", omp_get_thread_num());
    	mergesort_serial(a, size, temp);
    } else if (threads > 1) {
       #pragma omp parallel sections num_threads(2)
       {
//		        printf("Thread %d begins recursive section\n", omp_get_thread_num());
			#pragma omp section
		        { //printf("Thread %d begins recursive call\n", omp_get_thread_num());
			mergesort_parallel_omp(a, size/2, temp, threads/2);}
			#pragma omp section
		        { //printf("Thread %d begins recursive call\n", omp_get_thread_num());
			mergesort_parallel_omp(a + size/2, size - size/2, temp + size/2, threads - threads/2);}
			// The above use of temp + size/2 is an essential change from the serial version
       }
	   // Thread allocation is implementation dependent
       // Some threads can execute multiple sections while others are idle
       // Merge the two sorted sub-arrays through temp
       merge(a, size, temp);
    } else {
       printf("Error: %d threads\n", threads);
       return;
    }
}

void mergesort_serial(float a[], int size, float temp[]) {
    // Switch to insertion sort for small arrays
     if (size < SMALL) {
       insertion_sort(a, size);
       return;
    }
    mergesort_serial(a, size/2, temp);
    mergesort_serial(a + size/2, size - size/2, temp);
    // The above call will not work properly in an OpenMP program
    // Merge the two sorted subarrays into a temp array
    merge(a, size, temp);
}

void merge(float a[], int size, float temp[]) {
    int i1 = 0;
    int i2 = size/2;
    int tempi = 0;
    while (i1 < size/2 && i2 < size) {
        if (a[i1] < a[i2]) {
            temp[tempi] = a[i1];
            i1++;
        } else {
            temp[tempi] = a[i2];
            i2++;
        }
        tempi++;
    }
    while (i1 < size/2) {
        temp[tempi] = a[i1];
        i1++;
        tempi++;
    }
    while (i2 < size) {
        temp[tempi] = a[i2];
        i2++;
        tempi++;
    }
    // Copy sorted temp array into main array, a
    memcpy(a, temp, size*sizeof(int));
}

void insertion_sort(float a[], int size) {
    int i;
    for (i=0; i < size; i++) {
    int j, v = a[i];
        for (j = i - 1; j >= 0; j--) {
            if (a[j] <= v) break;
            a[j + 1] = a[j];
        }
        a[j + 1] = v;
    }
}

void write(float a[], int size){
  int  i;
  for(i = 0; i < size; i++){
    printf("%.15f \n ",a[i]);
  }
  printf("\n");
}
