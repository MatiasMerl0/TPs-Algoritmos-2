#include "lista.h"
#include <stdio.h>
#define ERROR -1
#define EXITO 0
void mostrar_elemento(void* elemento){
  if(elemento)
    printf("%c ", *(char*)elemento);
}

void ListaNoExiste_Inserto_DevuelveError(lista_t* lista, int* pruebas){
  char a='a';
  printf("No existe lista, no puedo insertar: %s\n",lista_insertar(lista,&a)==ERROR?"Paso":"No paso" );
  if (lista_insertar(lista,&a)==ERROR)
    (*pruebas)++;
}

void ListaNoExiste_InsertoEnPosicion_DevuelveError(lista_t* lista, int* pruebas){
  char a='a';
  printf("No existe lista, no puedo insertar en posicion: %s\n",lista_insertar_en_posicion(lista,&a,53)==ERROR?"Paso":"No paso" );
  if (lista_insertar_en_posicion(lista,&a,53)==ERROR)
    (*pruebas)++;
}
void ListaNoExiste_Borro_DevuelveError(lista_t* lista, int* pruebas){
  printf("No existe lista, no puedo borrar: %s\n",lista_borrar(lista)==ERROR?"Paso":"No paso" );
  if(lista_borrar(lista)==ERROR)
    (*pruebas)++;
}

void ListaNoExiste_BorroEnPosicion_DevuelveError(lista_t* lista, int* pruebas){
  printf("No existe lista, no puedo borrar de posicion: %s\n",lista_borrar_de_posicion(lista,52)==ERROR?"Paso":"No paso" );
  if(lista_borrar_de_posicion(lista,52)==ERROR)
    (*pruebas)++;  
}

void ListaNoExiste_PidoElementoEnPosicion_DevuelveNull(lista_t* lista, int* pruebas){
  printf("No existe lista, no puedo pedir elemento en posicion: %s\n",lista_elemento_en_posicion(lista,10)==NULL?"Paso":"No paso" );
  if(lista_elemento_en_posicion(lista,10)==NULL)
    (*pruebas)++;
}

void ListaNoExiste_PidoUltimoElemento_DevuelveNull(lista_t* lista, int* pruebas){
  printf("No existe lista, no puedo pedir ultimo elemento: %s\n",lista_ultimo(lista)==NULL?"Paso":"No paso" );
  if(lista_ultimo(lista)==NULL)
    (*pruebas)++;
}

void ListaNoExiste_ChequeoVacia_DevuelveTrue(lista_t* lista, int* pruebas){
  printf("No existe lista, chequeo vacia y devuelve true: %s\n",lista_vacia(lista)?"Paso":"No paso" );
  if(lista_vacia(lista))
    (*pruebas)++;
}

void ListaNoExiste_ChequeoCantidad_DevuelveCero(lista_t* lista, int* pruebas){
  printf("No existe lista, chequeo cantidad y devuelve cero: %s\n",lista_elementos(lista)==0?"Paso":"No paso" );
  if(lista_elementos(lista)==0)
    (*pruebas)++;
}

void ListaNoExiste_CreoIterador_DevuelveNull(lista_t* lista, int* pruebas){
  printf("No existe lista, no puedo crear iterador: %s\n",lista_iterador_crear(lista)==NULL?"Paso":"No paso" );
  if(lista_iterador_crear(lista)==NULL)
    (*pruebas)++;
}

void ListaVacia_Borro_NoBorraYDevuelveError(lista_t* lista, int* pruebas){
  printf("Lista vacia, no puedo borrar: %s\n",lista_borrar(lista)==ERROR?"Paso":"No paso" );
  if(lista_borrar(lista)==ERROR)
    (*pruebas)++;

}

void ListaVacia_BorroEnPosicion_NoBorraYDevuelveError(lista_t* lista, int* pruebas){
  printf("Lista vacia, no puedo borrar de posicion: %s\n",lista_borrar_de_posicion(lista,12)==ERROR?"Paso":"No paso" );
  if(lista_borrar_de_posicion(lista,12)==ERROR)
    (*pruebas)++;
}

void ListaVacia_ChequeoVacia_DevuelveTrue(lista_t* lista, int* pruebas){
  printf("Lista vacia, chequeo vacia y devuelve true: %s\n",lista_vacia(lista)?"Paso":"No paso" );
  if(lista_vacia(lista))
    (*pruebas)++;
}

void ListaVacia_ChequeoCantidad_DevuelveCero(lista_t* lista, int* pruebas){
  printf("Lista vacia, chequeo cantidad y devuelve cero: %s\n",lista_elementos(lista)==0?"Paso":"No paso" );
  if(lista_elementos(lista)==0)
    (*pruebas)++;
}

void Inserto2000Elementos_ChequeoCantidad_Devuelve2000(lista_t* lista, int* pruebas,char *a,char *b,char *c,char *d,char *w){
  for(int i=0;i<400;i++){ //uso literales pues la funcion es para agregar especificamente 2000 elementos
    lista_insertar(lista,a);
    lista_insertar(lista,b);
    lista_insertar(lista,c);
    lista_insertar(lista,d);
    lista_insertar(lista,w);
  }
  printf("Inserto 2000 elementos, chequeo cantidad y devuelve 2000: %s\n",lista_elementos(lista)==2000?"Paso":"No paso" );
  if(lista_elementos(lista)==2000)
    (*pruebas)++;
}

void ListaConElementos_ChequeoVacia_DevuelveFalse(lista_t* lista, int* pruebas){
  printf("Lista con elementos, chequeo vacia y devuelve false: %s\n",!lista_vacia(lista)?"Paso":"No paso" );
  if(!lista_vacia(lista))
    (*pruebas)++;
}

void ListaConElementos_ChequeoQueEstenEnOrden(lista_t* lista, int* pruebas){
  bool estan_ordenados=(*(char*)lista_elemento_en_posicion(lista,0)=='a' && *(char*)lista_elemento_en_posicion(lista,1)=='b' && *(char*)lista_elemento_en_posicion(lista,2)=='c' && *(char*)lista_elemento_en_posicion(lista,3)=='d' && *(char*)lista_elemento_en_posicion(lista,4)=='w');
  printf("Lista con elementos, elementos estan en orden: %s\n",estan_ordenados?"Paso":"No paso" );
  if(estan_ordenados)
    (*pruebas)++;
}

void InsertoEnPosicion_ChequeoCantidad(lista_t* lista, int* pruebas, char* elemento){
  lista_insertar_en_posicion(lista,elemento,233);
  printf("Inserto en posicion, cantidad es 2001: %s\n",lista_elementos(lista)==2001?"Paso":"No paso" );
  if(lista_elementos(lista)==2001)
    (*pruebas)++;
}

void BuscoEnPosicion_LosElementosSonLosCorrectos(lista_t* lista, int* pruebas, char elemento){
  printf("Busco en posicion, el elemento es el correcto: %s\n",*(char*)lista_elemento_en_posicion(lista,233)==elemento?"Paso":"No paso" );
  if(*(char*)lista_elemento_en_posicion(lista,233)==elemento)
    (*pruebas)++;
}

void Borro1000Elementos_ChequeoCantidad(lista_t* lista, int* pruebas){
  for(int i=0;i<1000;i++)
    lista_borrar(lista);
  printf("Borro 1000 elementos, cantidad es 1001: %s\n",lista_elementos(lista)==1001?"Paso":"No paso" );
  if(lista_elementos(lista)==1001)
    (*pruebas)++;
}

void BorroEnPosicion_ChequeoCantidad(lista_t* lista, int* pruebas){
  lista_borrar_de_posicion(lista,233);
  printf("Borro de posicion, cantidad es 1000: %s\n",lista_elementos(lista)==1000?"Paso":"No paso" );
  if(lista_elementos(lista)==1000)
    (*pruebas)++;
}

void NoExisteIterador_IntentoChequearSiguiente_DevuelveFalse(lista_iterador_t* iterador, int* pruebas){
  printf("No existe iterador, chequeo si hay siguiente y devuelve false: %s\n",lista_iterador_tiene_siguiente(iterador)==false?"Paso":"No paso" );
  if(lista_iterador_tiene_siguiente(iterador)==false)
    (*pruebas)++;
}

void NoExisteIterador_IntentoMovermeAlSiguiente_DevuelveNull(lista_iterador_t* iterador, int* pruebas){
  printf("No existe iterador, intento moverme al siguiente y devuelve NULL: %s\n",lista_iterador_siguiente(iterador)==NULL?"Paso":"No paso" );
  if(lista_iterador_siguiente(iterador)==NULL)
    (*pruebas)++;
}

void ExisteIterador_IntentoChequearSiguiente_DevuelveTrue(lista_iterador_t* iterador, int* pruebas){
  printf("Existe iterador, cheque si hay siguiente y devuelve true: %s\n",lista_iterador_tiene_siguiente(iterador)?"Paso":"No paso" );
  if(lista_iterador_tiene_siguiente(iterador))
    (*pruebas)++;
}

void ExisteIterador_IntentoMovermeAlSiguiente_DevuelveElementosCorrectos(lista_iterador_t* iterador,int* pruebas){
  bool paso=(*(char*)lista_iterador_siguiente(iterador)=='a');
  printf("Existe iterador, intento moverme al siguiente y devuelve el elemento en primera posicion: %s\n",paso?"Paso":"No paso" );
  if(paso)
    (*pruebas)++;
}

int main(){

  int pruebas=0;
  char a='a', b='b', c='c', d='d', w='w', f='f';
  //si la lista no existe
  lista_t* lista=NULL;
  ListaNoExiste_Inserto_DevuelveError(lista,&pruebas);
  ListaNoExiste_InsertoEnPosicion_DevuelveError(lista,&pruebas);
  ListaNoExiste_Borro_DevuelveError(lista,&pruebas);
  ListaNoExiste_BorroEnPosicion_DevuelveError(lista,&pruebas);
  ListaNoExiste_PidoElementoEnPosicion_DevuelveNull(lista,&pruebas);
  ListaNoExiste_PidoUltimoElemento_DevuelveNull(lista,&pruebas);
  ListaNoExiste_ChequeoVacia_DevuelveTrue(lista,&pruebas);
  ListaNoExiste_ChequeoCantidad_DevuelveCero(lista,&pruebas);
  ListaNoExiste_CreoIterador_DevuelveNull(lista,&pruebas);
  //si la lista esta vacia
  lista=lista_crear();
  ListaVacia_Borro_NoBorraYDevuelveError(lista,&pruebas);
  ListaVacia_BorroEnPosicion_NoBorraYDevuelveError(lista,&pruebas);
  ListaVacia_ChequeoVacia_DevuelveTrue(lista,&pruebas);
  ListaVacia_ChequeoCantidad_DevuelveCero(lista,&pruebas);
  //insertando elementos
  Inserto2000Elementos_ChequeoCantidad_Devuelve2000(lista,&pruebas,&a,&b,&c,&d,&w);
  ListaConElementos_ChequeoVacia_DevuelveFalse(lista,&pruebas);
  ListaConElementos_ChequeoQueEstenEnOrden(lista,&pruebas);
  //insertando en posicion
  InsertoEnPosicion_ChequeoCantidad(lista,&pruebas,&f);
  BuscoEnPosicion_LosElementosSonLosCorrectos(lista,&pruebas,f);
  //borrando elementos
  Borro1000Elementos_ChequeoCantidad(lista,&pruebas);
  //borrando en posicion
  BorroEnPosicion_ChequeoCantidad(lista,&pruebas);
  //cuando iterador no existe
  lista_iterador_t* iterador=NULL;
  printf("Pruebas de iterador.\n");
  NoExisteIterador_IntentoChequearSiguiente_DevuelveFalse(iterador,&pruebas);
  NoExisteIterador_IntentoMovermeAlSiguiente_DevuelveNull(iterador,&pruebas);
  //cuando iterador existe
  iterador=lista_iterador_crear(lista);
  ExisteIterador_IntentoChequearSiguiente_DevuelveTrue(iterador,&pruebas);
  ExisteIterador_IntentoMovermeAlSiguiente_DevuelveElementosCorrectos(iterador,&pruebas);

  printf("Pruebas pasadas %i/24\n",pruebas );
  printf("Destruyo la lista y creo una nueva, enlistando: a b c d w\n");
  lista_iterador_destruir(iterador);
  lista_destruir(lista);
  lista=lista_crear();
  lista_insertar(lista,&a);
  lista_insertar(lista,&b);
  lista_insertar(lista,&c);
  lista_insertar(lista,&d);
  lista_insertar(lista,&w);

  printf("Imprimo la lista usando el iterador interno: ");
  lista_con_cada_elemento(lista, mostrar_elemento);
  printf("\n");

  lista_destruir(lista);
  return 0;
}
