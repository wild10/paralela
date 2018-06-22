#include<iostream>
#include<cstdio>
#include<ctime>
#include<omp.h>

using namespace std;

#define size_img 2000
#define size_K 5
#define num_thrds 4

#define m_size(x) {sizeof(x)/sizeof(x[0])}
#define SWAP(a,b) {int temp = a; a=b; b=temp;}
#define dbg(x) cout << #x <<"="<< x <<endl


int serial_conv2D(){

  clock_t t;

  // kernel 5 x 5
  float kernel[size_K][size_K] = {{1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1}};

  //reservando memoria
  float **img = new float*[size_img]; //creando filas
  for(int i = 0; i < size_img; i++) //creando columnas
    img[i] = new float[size_img];

 //asignando
    for(int i = 0; i < size_img; i++){
      for(int j = 0; j < size_img; j++){
        img[i][j] = 0;
      }
    }

  // img[1][1] = 1;
/*
  // matriz imagen
  float img[size_img][size_img];
  for(int i = 0; i < size_img; i++){
    for(int j = 0; j < size_img; j++){
      img[i][j] = 0;
    }
  }
*/

  // img[0][0] = 1;
  /*
printf("Mostrar img \n");

  for (int i = 0; i < size_img; i++) {
    for(int j = 0; j < size_img; j++){
      printf("%d ",img[i][j] );
    }
    printf("\n");
  }
*/

  //imagene resultante
  // float img2[size_img][size_img];
  float **img2 = new float*[size_img];
  for(int i = 0; i < size_img; i++)
    img2[i] = new float[size_img];

  // la mitad del kernel para posicionar en la matriz
  int mitad = size_K / 2;

  // int sum,kk,ll,ii,jj;
  int sum, ii,jj;

//start time
  t = clock();
  for(int i = 0; i < size_img; i++){
    for(int j = 0; j < size_img; j++){

      //acumular
      sum = 0;
      for( int k = 0 ; k < size_K ; k++){
        //kk = size_K - 1 - k; //indice fila keernel al revez

        for (int l = 0; l < size_K; l++) {
          //ll = size_K - 1 -l;   // indice col kernel al revez

          // indices aux img para controlar limites
          ii = i + (k - mitad);
          jj = j + (l - mitad);

          //control de limites de la señal
          if( ii >= 0 && ii < size_K && jj >= 0 && jj < size_K)
          {
              sum += img[ii][jj] * kernel[k][l];
              // printf("%d ",sum );
          }

        }
      }
      img2[i][j] = sum;
    }
  }
  //end time
  t = clock() - t;


printf("Mostrar img 2\n");

//mostrar datos
//
// for (int i = 0; i < size_img; i++) {
//   for(int j = 0; j < size_img; j++){
//     printf("%f  ",img2[i][j] );
//   }
//   printf("\n");
// }


  printf("tiempo en serie: %f\n", (float)t/CLOCKS_PER_SEC  );
  return 0;
}

int parallel_conv2D(){
  // kernel 5 x 5
 int kernel[size_K][size_K] = {{1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1},
                                {1, 1, 1, 1, 1}};

//matriz img
//allocanting images
int **img = new int*[size_img]; // creando filas
for(int i = 0; i < size_img; i++) //creando columnas
    img[i] = new int [size_img];

//asignando valores a img
for(int i = 0; i < size_img; i++){
  for(int j = 0; j < size_img; j++){
    img[i][j] = 1;
  }

}

/*
static int img[size_img][size_img];
for(int i = 0; i < size_img; i++){
  for (int j = 0; j < size_img; j++) {
    img[i][j] = 1;
  }
}

*/
//imagene resultante
// static int  img2[size_img][size_img];
//reserva de memoria para img2
int **img2 = new int*[size_img]; //creando finlas
for(int i = 0; i < size_img; i++)
  img2[i] = new int[size_img];

// la mitad del kernel para posicionar en la matriz
int mitad = size_K / 2;

int sum,ii,jj,i,j,k,l;

double t= omp_get_wtime();
// Sleep(1000);
// double t2 = omp_get_wtime();
# pragma omp parallel for  num_threads(num_thrds)  private(i,j,k,l,ii,jj,sum)
// {
// # pragma omp parallel for private(i,j,sum,k,l)
  for( i = 0; i < size_img; i++){
    for( j = 0; j < size_img; j++){

      //acumular
      sum = 0;
      for(  k = 0 ; k < size_K ; k++){
        //kk = size_K - 1 - k; //indice fila keernel al revez

        for ( l = 0; l < size_K; l++) {
          //ll = size_K - 1 -l;   // indice col kernel al revez
          // indices aux img para controlar limites
          ii = i + (k - mitad);
          jj = j + (l - mitad);

          //control de limites de la señal
          if( ii >= 0 && ii < size_K && jj >= 0 && jj < size_K)
          {
              sum += img[ii][jj] * kernel[k][l];

          }

        }
      }
        img2[i][j] = sum;
    }
  }
   t = omp_get_wtime()-t;

// }

for (int i = 0; i < size_img; i++) {
  for(int j = 0; j < size_img; j++){
    printf("%d  ",img2[i][j] );
  }
  printf("\n");
}

  printf("tiempo parallel: %lf\n", t);
  return 0;
}

int main(){

  // serial_conv2D();
  parallel_conv2D();

  return 0;
}
