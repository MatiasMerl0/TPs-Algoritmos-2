#include "pila.h"
#include <stdlib.h>
#define EXIT_ERROR -1
#define SUCCESS 0
#define REDIMENSIONAMIENTO 2
#define CRITERIO_ACHICAMIENTO 4
#define TAMANIO_PILA 10

pila_t* pila_crear(){
	pila_t* pila=malloc(sizeof(pila_t));
	if (pila==NULL){
		return NULL;
	}
	pila->tope=0;
	pila->tamanio=TAMANIO_PILA;
	pila->elementos=malloc(sizeof(void*) * (size_t)(pila->tamanio));
	if (pila->elementos==NULL){
		free(pila);
		return NULL;
	}
	return pila;
}

/*
**PRE:Recibe el puntero a un pila_t junto con el tamaño nuevo que tendra la pila.
**POST:Cambia el tamaño del vector elementos de la pila, devolviendo 0 si lo hizo con exito y -1 si falló.
*/
int redimensionar_pila(pila_t* pila, int tamanio){
	void** puntero_aux=pila->elementos;
	pila->elementos=realloc(pila->elementos,sizeof(pila_t)*(size_t)(pila->tamanio));
	if (pila->elementos==NULL){
		pila->elementos=puntero_aux;
		return EXIT_ERROR;
	}
	return SUCCESS;
}

int pila_apilar(pila_t* pila, void* elemento){
	if (pila==NULL){
		return EXIT_ERROR;
	}
	if(pila->tamanio==pila->tope){
		int redimensionar=redimensionar_pila(pila,(pila->tamanio)*REDIMENSIONAMIENTO);
		if (redimensionar==EXIT_ERROR){
			return EXIT_ERROR;
		}
		pila->tamanio*=REDIMENSIONAMIENTO;
	}
	*( (pila->elementos) + (pila->tope) )=elemento;
	(pila->tope)++;
	return SUCCESS;
}

int pila_desapilar(pila_t* pila){
	if ( (pila==NULL) || (pila->tope<=0) ){
		return EXIT_ERROR;
	}
	(pila->tope)--; 
	if (( (pila->tope)<=(pila->tamanio/CRITERIO_ACHICAMIENTO) ) && (pila->tamanio/REDIMENSIONAMIENTO>=TAMANIO_PILA)){
		int redimensionar=redimensionar_pila(pila,(pila->tamanio)/REDIMENSIONAMIENTO);
		if (redimensionar==EXIT_ERROR){
			return EXIT_ERROR;
		}
		pila->tamanio/=REDIMENSIONAMIENTO;
	}
	return SUCCESS;
}

bool pila_vacia(pila_t* pila){
	if (pila==NULL){
		return true;
	}
	return (pila->tope==0);
}

int pila_cantidad(pila_t* pila){
	if (pila==NULL){
		return 0;
	}
	return (pila->tope);
}

void* pila_tope(pila_t* pila){
	if ( (pila==NULL) || (pila->tope==0) ){
		return NULL;
	}
	return ( *(pila->elementos + (pila->tope)-1) );
}

void pila_destruir(pila_t* pila){
	if(pila!=NULL){	
		free(pila->elementos);
		free(pila);
	}
}