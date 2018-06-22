#include<iostream>
#include<cstdio>
#include<vector>
#include<stdlib.h>
#include <fstream>      // std::ifstream


using namespace std;

int main(){
  //variable tipo archivo
  ifstream archivo;
  double n;
  char linea[128];
  vector<double> v;
  // abrir el archivo en modo lectura con ios::in
  archivo.open("list.txt", ios::in);

  if(archivo.fail()){
    printf("No se pudo abrir el archivo \n");
  }

  while (!archivo.eof()) { //mientras no sea el final del archivo
/*
    archivo.getline(linea,sizeof(linea)); // obtiene la linea como texto
    if(atof(linea) != 0){ //convertir a entero
      v.push_back(atof(linea));
    }
*/
    archivo >> n;
    // printf("%lf ", (n+1));
    v.push_back(n);
    // cout<<n<<endl;
  }
  archivo.close();
  printf("size : %ld\n",v.size());
/*
  //mostrar los DATOS del ARRAY
  for(int i = 0; i < v.size(); i++){
    printf("%lf\n",v[i]+1 );
  }

*/
  return 0;
}
