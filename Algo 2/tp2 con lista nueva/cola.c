#include "cola.h"
#include <stdlib.h>
#include <stdbool.h>
#define EXIT_ERROR -1
#define SUCCESS 0

cola_t* cola_crear(){
	cola_t* cola=calloc(1,sizeof(cola_t));
		if(cola==NULL){
			return NULL;
		}
	return cola;
}

int cola_encolar(cola_t* cola, void* elemento){
	if (cola==NULL){
		return EXIT_ERROR;
	}
	
	nodo_t* nodo=malloc(sizeof(nodo_t));
	if (nodo==NULL){
		return EXIT_ERROR;
	}
	nodo->elemento=elemento;
	if(cola->cantidad==0){ //En caso que la cola este vacia
		cola->nodo_inicio=nodo;
		cola->nodo_fin=nodo;
	}
	else{
		(cola->nodo_fin)->siguiente=nodo;
		cola->nodo_fin=nodo;
	}
	(cola->nodo_fin)->siguiente=NULL; //Quiero que el ultimo elemento de la cola apunte a NULL
	(cola->cantidad)++;
	return SUCCESS;
}

int cola_desencolar(cola_t* cola){
	if(cola==NULL || cola->cantidad==0){
		return EXIT_ERROR;
	}
	nodo_t* auxiliar=(cola->nodo_inicio)->siguiente; //Si solo hubiera 1 elemento, auxiliar estara apuntando a NULL
	free(cola->nodo_inicio);
	cola->nodo_inicio=auxiliar;
	(cola->cantidad)--;
	if(cola->cantidad==0){
		cola->nodo_fin=NULL; //Si luego de desencolar ya no hay elementos, fin apunta a NULL
	}
	return SUCCESS;
}

bool cola_vacia(cola_t* cola){
    if(cola==NULL){
		return true;
	}
	else{
		return(cola->cantidad==0);
	}
}

int cola_cantidad(cola_t* cola){
	if (cola==NULL){
		return 0;
	}
	else{
		return (cola->cantidad);
	}
}

void* cola_primero(cola_t* cola){
	if (cola==NULL || cola->cantidad==0){
		return NULL;
	}
	return ((cola->nodo_inicio)->elemento);
}

void cola_destruir(cola_t* cola){
	if (cola!=NULL){
		nodo_t* auxiliar=cola->nodo_inicio;
		while(auxiliar!=NULL){ //Porque el ultimo elemento en la cola apunta a null
			auxiliar=(cola->nodo_inicio)->siguiente;
			free(cola->nodo_inicio);
			cola->nodo_inicio=auxiliar;
		}
		free(cola);
	}
}
