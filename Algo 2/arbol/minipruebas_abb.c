#include "abb.h"
#include <stdio.h>

typedef struct cosa{
    int clave;
    char contenido[10];
}cosa;

cosa* crear_cosa(int clave){
    cosa* c = (cosa*)malloc(sizeof(cosa));
    if(c)
	c->clave = clave;
    return c;
}

void destruir_cosa(cosa* c){
    if(c)
	free(c);
}

int comparar_cosas(void* elemento1, void* elemento2){
    if(!elemento1 || !elemento2)
	return 0;

    if(((cosa*)elemento1)->clave>((cosa*)elemento2)->clave)
	return 1;
    if(((cosa*)elemento1)->clave<((cosa*)elemento2)->clave)
	return -1;
    return 0;
}

void destructor_de_cosas(void* elemento){
    if(!elemento)
	return;
    destruir_cosa((cosa*)elemento);
}

bool mostrar_elemento(void* elemento, void* extra){
  extra=extra; //para que no se queje el compilador, gracias -Werror -Wall
  if(elemento)
    printf("%i ", ((cosa*)elemento)->clave);
  return false;
}

bool mostrar_hasta_5(void* elemento, void* extra){
  extra=extra; //para que no se queje el compilador, gracias -Werror -Wall
  if(elemento){
    printf("%i ", ((cosa*)elemento)->clave);
    if(((cosa*)elemento)->clave == 5)
      return true;
  }
  return false;
}

bool mostrar_acumulado(void* elemento, void* extra){
  if(elemento && extra){
    *(int*)extra += ((cosa*)elemento)->clave;
    printf("%i ", *(int*)extra);
  }
  return false;
}

void no_se_puede_crear_arbol_y_arbol_es_null(){
    abb_t* arbol=arbol_crear(NULL,destructor_de_cosas);
    printf("Pruebas donde no se puede crear el arbol o el arbol es null.\n");
    printf("No se puede crear arbol sin comparador: %s \n",arbol==NULL?"Paso":"No paso" );
    printf("arbol_raiz de un arbol null devuelve null: %s\n",arbol_raiz(arbol)==NULL?"Paso":"No paso" );
    printf("Arbol_vacio devuelve true: %s\n",arbol_vacio(arbol)?"Paso":"No paso" );
}

void operaciones_con_arbol_vacio(){
    abb_t* arbol=arbol_crear(comparar_cosas,destructor_de_cosas);
    printf("Pruebas con arbol vacio:\n");
    printf("Se crea arbol vacio: %s\n",arbol_vacio(arbol)?"Paso":"No paso");
    printf("Elemento de la raiz es NULL %s\n",arbol_raiz(arbol)==NULL?"Paso":"No paso" );
    cosa* c1= crear_cosa(1);
    printf("Buscar elemento en arbol vacio devuelve NULL: %s\n",arbol_buscar(arbol,c1)==NULL?"Paso":"No paso" );
    printf("No se puede eliminar elementos de un arbol vacio: %s\n",arbol_borrar(arbol,c1)==-1?"Paso":"No paso" );
    cosa* array[30];
    printf("No se puede recorrer inorden con arbol vacio: %s\n",arbol_recorrido_inorden(arbol,(void**)array,10)==0?"Paso":"No paso" );
    printf("No se puede recorrer preorden con arbol vacio: %s\n",arbol_recorrido_preorden(arbol,(void**)array,10)==0?"Paso":"No paso" );
    printf("No se puede recorrer postorden con arbol vacio: %s\n",arbol_recorrido_postorden(arbol,(void**)array,10)==0?"Paso":"No paso" );
    printf("Iterador interno no debe imprimir nada:\n");
    abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, mostrar_hasta_5, NULL);
    abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_hasta_5, NULL);
    abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_hasta_5, NULL);
    destruir_cosa(c1);

    arbol_destruir(arbol);
}

void operaciones_insertando_elementos(){
    abb_t* arbol=arbol_crear(comparar_cosas,destructor_de_cosas);
    printf("Pruebas insertando elementos:\n");
    cosa* c1= crear_cosa(1);
    cosa* c2= crear_cosa(2);
    cosa* c3= crear_cosa(3);
    cosa* c4= crear_cosa(4);
    cosa* c5= crear_cosa(5);
    cosa* c6= crear_cosa(6);
    cosa* c7= crear_cosa(7);
    cosa* elementos[30];
    
    arbol_insertar(arbol, c4);
    arbol_insertar(arbol, c2);
    arbol_insertar(arbol, c6);
    arbol_insertar(arbol, c1);
    arbol_insertar(arbol, c3);
    arbol_insertar(arbol, c5);
    arbol_insertar(arbol, c7);

    printf("Inserto 7 elementos, el arbol no esta vacio: %s\n",arbol_vacio(arbol)?"No paso":"Paso" );
    printf("La raiz es 4: %s\n",((cosa*)arbol_raiz(arbol))->clave==4?"Paso":"No paso" );
    printf("No se puede hacer recorrido inorden con un array de longitud cero: %s\n",arbol_recorrido_inorden(arbol, (void**)elementos,0)==0?"Paso":"No paso" );
    printf("No se puede hacer recorrido preorden con un array de longitud cero: %s\n",arbol_recorrido_preorden(arbol, (void**)elementos,0)==0?"Paso":"No paso" );
    printf("No se puede hacer recorrido postorden con un array de longitud cero: %s\n",arbol_recorrido_postorden(arbol, (void**)elementos,0)==0?"Paso":"No paso" );
    
    printf("Recorrido inorden devuelve 7: %s\n",arbol_recorrido_inorden(arbol, (void**)elementos, 10)==7?"Paso":"No paso" );
    printf("Recorrido inorden tiene que dar: 1 2 3 4 5 6 7\n");
    printf("Recorrido da: \n");
    int cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 10);
    for(int i=0;i<cantidad;i++)
    printf("%i ", elementos[i]->clave);
    printf("\n");
   
    printf("Recorrido postorden devuelve 7: %s\n",arbol_recorrido_postorden(arbol, (void**)elementos, 10)==7?"Paso":"No paso" );
    printf("Recorrido postorden tiene que dar: 1 3 2 5 7 6 4\n");
    printf("Recorrido postorden da:\n");
    cantidad = arbol_recorrido_postorden(arbol, (void**)elementos, 10);
    for(int i=0;i<cantidad;i++)
    printf("%i ", elementos[i]->clave);
    printf("\n");
    
    printf("Recorrido preorden devuelve 7: %s\n",arbol_recorrido_preorden(arbol, (void**)elementos, 10)==7?"Paso":"No paso" );
    printf("Recorrido preorden tiene que dar: 4 2 1 3 6 5 7 \n");
    printf("Recorrido preorden da:\n");
    cantidad = arbol_recorrido_preorden(arbol, (void**)elementos, 10);
    for(int i=0;i<cantidad;i++)
    printf("%i ", elementos[i]->clave);
    printf("\n");
    printf("\n");

    printf("Inserto mas valores (y algunos repetidos)\n" );
    arbol_insertar(arbol, crear_cosa(0));
    arbol_insertar(arbol, crear_cosa(0));
    arbol_insertar(arbol, crear_cosa(20));
    arbol_insertar(arbol, crear_cosa(56));
    arbol_insertar(arbol, crear_cosa(37));
    arbol_insertar(arbol, crear_cosa(27));
    arbol_insertar(arbol, crear_cosa(20));
    arbol_insertar(arbol, crear_cosa(80));
    arbol_insertar(arbol, crear_cosa(100));
    arbol_insertar(arbol, crear_cosa(30));

    printf("Recorrido inorden tiene que dar:0 0 1 2 3 4 5 6 7 20 20 27 30 37 56 80 100\n");
    printf("Recorrido inorden da:");
    cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 40);
    for(int i=0;i<cantidad;i++)
    printf("%i ", elementos[i]->clave);
    printf("\n");

    printf("Recorrido preorden tiene que dar:4 2 1 0 0 3 6 5 7 20 56 37 27 20 30 80 100\n");
    printf("Recorrido preorden da:");
    cantidad = arbol_recorrido_preorden(arbol, (void**)elementos, 40);
    for(int i=0;i<cantidad;i++)
    printf("%i ", elementos[i]->clave);
    printf("\n");

    printf("Recorrido postorden tiene que dar:0 0 1 3 2 5 20 30 27 37 100 80 56 20 7 6 4\n");
    printf("Recorrido postorden da:");
    cantidad = arbol_recorrido_postorden(arbol, (void**)elementos, 40);
    for(int i=0;i<cantidad;i++)
    printf("%i ", elementos[i]->clave);
    printf("\n");

    printf("\nRecorrido inorden hasta encontrar el 5: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_hasta_5, NULL);
    printf("\n");

    printf("Recorrido preorden hasta encontrar el 5: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_hasta_5, NULL);
    printf("\n");

    printf("Recorrido postorden hasta encontrar el 5: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, mostrar_hasta_5, NULL);
    printf("\n\n");


    int acumulador=0;
    printf("Recorrido inorden acumulando los valores tiene que dar:0 0 1 3 6 10 15 21 28 48 68 95 125 162 218 298 398\n" );
    printf("Recorrido inorden acumulando los valores da: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_acumulado, &acumulador);
    printf("\n\n");

    acumulador=0;
    printf("Recorrido preorden acumulando los valores tiene que dar:4 6 7 7 7 10 16 21 28 48 104 141 168 188 218 298 398\n");
    printf("Recorrido preorden acumulando los valores da: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_acumulado, &acumulador);
    printf("\n\n");

    acumulador=0;
    printf("Recorrido postorden acumulando los valores tiene que dar:0 0 1 4 6 11 31 61 88 125 225 305 361 381 388 394 398\n");
    printf("Recorrido postorden acumulando los valores da: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, mostrar_acumulado, &acumulador);
    printf("\n\n");

    arbol_destruir(arbol);
}

void pruebas_borrando_elementos(){
    abb_t* arbol=arbol_crear(comparar_cosas,destructor_de_cosas);
    cosa* auxiliar = crear_cosa(0);
    cosa* elementos[30];
    printf("Pruebas borrando elementos:\n");

    arbol_insertar(arbol, crear_cosa(0));
    arbol_insertar(arbol, crear_cosa(3));
    arbol_insertar(arbol, crear_cosa(2));
    arbol_insertar(arbol, crear_cosa(6));
    arbol_insertar(arbol, crear_cosa(37));
    arbol_insertar(arbol, crear_cosa(17));
    arbol_insertar(arbol, crear_cosa(25));
    arbol_insertar(arbol, crear_cosa(1));
    arbol_insertar(arbol, crear_cosa(100));
    arbol_insertar(arbol, crear_cosa(7));
    arbol_insertar(arbol, crear_cosa(5));
    arbol_insertar(arbol, crear_cosa(19));
    arbol_insertar(arbol, crear_cosa(9));
    arbol_insertar(arbol, crear_cosa(39));

    printf("Recorrido inorden:");
    int cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 20);
    for(int i=0;i<cantidad;i++)
    printf("%i ", elementos[i]->clave);
    printf("\n");

    printf("Borro el nodo: %s\n",arbol_borrar(arbol,auxiliar)==0?"Paso":"No paso" );
    printf("Busco el nodo borrado y no esta: %s\n",((cosa*)arbol_buscar(arbol, auxiliar))==NULL?"Paso":"No paso");

    auxiliar->clave = 100;
    printf("Borro nodo hoja (100): %s\n", (arbol_borrar(arbol, auxiliar))==0?"Paso":"No paso");

    auxiliar->clave = 2;
    printf("Borro nodo con un hijo (2): %s\n", (arbol_borrar(arbol, auxiliar))==0?"Paso":"No paso");

    auxiliar->clave = 6;
    printf("Borro nodo con dos hijos (6): %s\n", (arbol_borrar(arbol, auxiliar))==0?"Paso":"No paso");

    auxiliar->clave = 3;
    printf("Borro la raiz (3): %s\n",(arbol_borrar(arbol, auxiliar))==0?"Paso":"No paso");

    auxiliar->clave = 100000;
    printf("No puede borrar un elemento que no existe en el arbol: %s\n",(arbol_borrar(arbol, auxiliar))==-1?"Paso":"No paso");


    printf("\nRecorrido inorden debe dar:1 5 7 9 17 19 25 37 39\n");
    printf("Recorrido inorden da:");
    cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 20);
    for(int i=0;i<cantidad;i++)
    printf("%i ", elementos[i]->clave);
    printf("\n\n");

    printf("Recorrido preorden debe dar:1 5 37 17 7 9 25 19 39 \n");
    printf("Recorrido inorden da:");
    cantidad = arbol_recorrido_preorden(arbol, (void**)elementos, 20);
    for(int i=0;i<cantidad;i++)
    printf("%i ", elementos[i]->clave);
    printf("\n\n");

    printf("Recorrido postorden debe dar:9 7 19 25 17 39 37 5 1 \n");
    printf("Recorrido postorden da:");
    cantidad = arbol_recorrido_postorden(arbol, (void**)elementos, 20);
    for(int i=0;i<cantidad;i++)
    printf("%i ", elementos[i]->clave);
    printf("\n\n");

    int acumulador=0;
    printf("Recorrido inorden acumulando los valores tiene que dar:1 6 13 22 39 58 83 120 159\n" );
    printf("Recorrido inorden acumulando los valores da: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_acumulado, &acumulador);
    printf("\n\n");

    acumulador=0;
    printf("Recorrido preorden acumulando los valores tiene que dar:1 6 43 60 67 76 101 120 159\n");
    printf("Recorrido preorden acumulando los valores da: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_acumulado, &acumulador);
    printf("\n\n");

    acumulador=0;
    printf("Recorrido postorden acumulando los valores tiene que dar:9 16 35 60 77 116 153 158 159\n");
    printf("Recorrido postorden acumulando los valores da: ");    
    abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, mostrar_acumulado, &acumulador);
    printf("\n\n");

    destruir_cosa(auxiliar);
    arbol_destruir(arbol);

}

int main(){
  
    no_se_puede_crear_arbol_y_arbol_es_null();
    printf("\n");
    operaciones_con_arbol_vacio();
    printf("\n");
    operaciones_insertando_elementos();
    printf("\n");
    pruebas_borrando_elementos();

    return 0;
}
