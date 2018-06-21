//#include<iostream>
#include<stdio.h>
#include<time.h>
#include<omp.h>

// using namespace std;

#define size_img 2000
#define size_K 5
#define num_thrds 4

#define m_size(x) {sizeof(x)/sizeof(x[0])}
#define SWAP(a,b) {int temp = a; a=b; b=temp;}
#define dbg(x) cout << #x <<"="<< x <<endl


int serial_conv2D(){

clock_t t;

// kernel 5 x 5
static int  kernel[size_K][size_K] = {{1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1}};

// int kernel[size_K][size_K] = {
//                         {1, 1, 1},
//                         {1, 1, 1},
//                         {1, 1, 1}
// };

// matriz imagen
static int img[size_img][size_img];
for(int i = 0; i < size_img; i++){
  for(int j = 0; j < size_img; j++){
    img[i][j] = 0;
  }
}

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
static int img2[size_img][size_img];

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

for (int i = 0; i < size_img; i++) {
for(int j = 0; j < size_img; j++){
  printf("%d  ",img2[i][j] );
}
printf("\n");
}
printf("tiempo en serie: %f\n", (float)t/CLOCKS_PER_SEC  );
return 0;
}

int parallel_conv2D(){
// kernel 5 x 5
static int kernel[size_K][size_K] = {{1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1},
                              {1, 1, 1, 1, 1}};

//matriz img

static int img[size_img][size_img];
for(int i = 0; i < size_img; i++){
for (int j = 0; j < size_img; j++) {
  img[i][j] = 1;
}
}

//imagene resultante
static int  img2[size_img][size_img];

// la mitad del kernel para posicionar en la matriz
int mitad = size_K / 2;

int sum,ii,jj,i,j,k,l;


# pragma omp parallel num_threads(num_thrds)
{
# pragma omp parallel for private(i,j,sum,k,l)
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
            // printf("%d ",sum );
        }

      }
    }

      img2[i][j] = sum;
  }
}
}

for (int i = 0; i < size_img; i++) {
for(int j = 0; j < size_img; j++){
  printf("%d  ",img2[i][j] );
}
printf("\n");
}

return 0;
}

int main(){

serial_conv2D();
// parallel_conv2D();

return 0;
}
