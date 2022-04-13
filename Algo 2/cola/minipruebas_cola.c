#include "cola.h"
#include <stdio.h>
#define ERROR -1
#define EXITO 0

void ColaNoExiste_IntentoEncolar_DevuelveError(cola_t* cola,int elemento, int* pruebas_exitosas){
  printf("Cola no existe, no puede encolar: %s\n",cola_encolar(cola,&elemento)==ERROR?"Paso":"No paso" );
  if (cola_encolar(cola,&elemento)==ERROR){
    (*pruebas_exitosas)++;
  }
}

void ColaNoExiste_IntentoDesencolar_DevuelveError(cola_t* cola,int* pruebas_exitosas){
  printf("Cola no existe, no puede desencolar: %s\n",cola_desencolar(cola)==ERROR?"Paso":"No paso" );
  if(cola_desencolar(cola)==ERROR){
    (*pruebas_exitosas)++;
  }
}

void ColaNoExiste_ChequeoVacia_DevuelveTrue(cola_t* cola,int* pruebas_exitosas){
  printf("Cola no existe, cola_vacia devuelve true: %s\n",cola_vacia(cola)?"Paso":"No paso" );
  if(cola_vacia(cola)){
    (*pruebas_exitosas)++;
  }
}

void ColaNoExiste_ChequeoCantidad_DevuelveCero(cola_t* cola,int* pruebas_exitosas){
  printf("Cola no existe, cola_cantidad devuelve cero: %s\n",cola_cantidad(cola)==0?"Paso":"No paso" );
  if(cola_cantidad(cola)==0){
    (*pruebas_exitosas)++;
  }
}

void ColaNoExiste_PidoElemento_DevuelveNULL(cola_t* cola,int* pruebas_exitosas){
  printf("Cola no existe, pido elemento y devuelve NULL: %s\n",cola_primero(cola)==NULL?"Paso":"No paso" );
  if(cola_primero(cola)==NULL){
    (*pruebas_exitosas)++;
  }
}

void ColaEstaVacia_ChequeoVacia_DevuelveTrue(cola_t* cola,int* pruebas_exitosas){
  printf("Cola esta vacia, cola_vacia devuelve true: %s\n",cola_vacia(cola)?"Paso":"No paso" );
  if(cola_vacia(cola)){
    (*pruebas_exitosas)++;
  }
}

void ColaEstaVacia_ChequeoCantidad_DevuelveCero(cola_t* cola,int* pruebas_exitosas){
  printf("Cola esta vacia, cola_cantidad devuelve cero: %s\n",cola_cantidad(cola)==0?"Paso":"No paso" );
  if(cola_cantidad(cola)==0){
    (*pruebas_exitosas)++;
  }
}

void ColaEstaVacia_PidoElemento_DevuelveNULL(cola_t* cola,int* pruebas_exitosas){
  printf("Cola esta vacia, pido elemento y devuelve NULL: %s\n",cola_primero(cola)==NULL?"Paso":"No paso" );
  if(cola_primero(cola)==NULL){
    (*pruebas_exitosas)++;
  }
}

void Encolo1000Elementos_ChequeoCantidad_Devuelve1000(cola_t* cola,int* a,int* b,int* c,int* d,int* pruebas_exitosas){
  for(int i=0;i<250;i++){ //uso literales en esta funcion pues, como dice el nombre de la funcion, es especificamente para  añadir 1000 elementos
    cola_encolar(cola,a); 
    cola_encolar(cola,b);
    cola_encolar(cola,c);
    cola_encolar(cola,d);
  }
  printf("Encolo 1000 elementos, cola_cantidad devuelve 1000: %s\n",cola_cantidad(cola)==1000?"Paso":"No paso" );
  if (cola_cantidad(cola)==1000){
    (*pruebas_exitosas)++;
  }
}

void ColaTiene1000Elementos_ChequeoVacia_DevuelveFalse(cola_t* cola,int* pruebas_exitosas){
  printf("Cola tiene 1000 elementos, cola_vacia devuelve false: %s\n",cola_vacia(cola)?"Paso":"No paso" );
  if(!cola_vacia(cola)){
    (*pruebas_exitosas)++;
  }
}

/*
**PRE:Recibe el estado de la prueba, y un elemento a comparar.
**post:Compara el primer elemento de la cola con el elemento recibido. Luego desencola un elemento.
*/
bool comparacion(int elemento, cola_t* cola){
  bool paso=true;
  if(*(int*)cola_primero(cola)!=elemento){
      paso=false;
    }
    printf("%i\n",*(int*)cola_primero(cola) );
    cola_desencolar(cola);
    return paso;
}

void ColaTieneElementos_DesencoloEImprimo_DebenCoincidirLosElementos(cola_t* cola,int a, int b,int c, int d,int* pruebas_exitosas){
  //desencolo los primeros 4 elementos y deben coincidir
  printf("Salida esperada: \n%i\n%i\n%i\n%i\n",a,b,c,d);
  printf("Salida obtenida:\n");
  if(comparacion(a,cola) && comparacion(b,cola) && comparacion(c,cola) && comparacion (d,cola)){
    (*pruebas_exitosas)++;
  }
}

void ColaTieneElementos_Desencolo2000_QuedanCamposEnOrden(cola_t* cola,int* pruebas_exitosas){
  bool paso=false;
  for (int i=0;i<2000;i++){
    cola_desencolar(cola);
  }
  if(cola_vacia(cola) && cola_cantidad(cola)==0 && cola_primero(cola)==NULL){
    (*pruebas_exitosas)++;
    paso=true;
  }
  printf("Desencolo muchos elementos, todo queda en orden: %s\n",paso?"Paso":"No paso" );
}

int main(){
  int pruebas_exitosas=0;
  int a=1, b=2, c=3, d=4;
  //Pruebas cuando no existe la cola
  cola_t* cola=NULL; //Al fallar el crear se pone en NULL
  ColaNoExiste_IntentoEncolar_DevuelveError(cola,a,&pruebas_exitosas);
  ColaNoExiste_IntentoDesencolar_DevuelveError(cola,&pruebas_exitosas);
  ColaNoExiste_ChequeoVacia_DevuelveTrue(cola,&pruebas_exitosas);
  ColaNoExiste_ChequeoCantidad_DevuelveCero(cola,&pruebas_exitosas);
  ColaNoExiste_PidoElemento_DevuelveNULL(cola,&pruebas_exitosas);

  //Pruebas con cola vacia
  cola=cola_crear();
  ColaEstaVacia_ChequeoVacia_DevuelveTrue(cola,&pruebas_exitosas);
  ColaEstaVacia_ChequeoCantidad_DevuelveCero(cola,&pruebas_exitosas);
  ColaEstaVacia_PidoElemento_DevuelveNULL(cola,&pruebas_exitosas);

  //Pruebas encolando elementos
  Encolo1000Elementos_ChequeoCantidad_Devuelve1000(cola,&a,&b,&c,&d,&pruebas_exitosas);
  ColaTiene1000Elementos_ChequeoVacia_DevuelveFalse(cola,&pruebas_exitosas);

  //Pruebas desencolando elementos
  ColaTieneElementos_DesencoloEImprimo_DebenCoincidirLosElementos(cola,a,b,c,d,&pruebas_exitosas); //Me fijo si los elementos estan en el orden correcto
  ColaTieneElementos_Desencolo2000_QuedanCamposEnOrden(cola,&pruebas_exitosas);
  
  printf("%i Pruebas pasadas de 12\n",pruebas_exitosas );
  cola_destruir(cola);

  return 0;
}