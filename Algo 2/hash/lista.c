#include "lista.h"
#include <stdlib.h>
#define EXIT_ERROR -1
#define SUCCESS 0
#define VACIA 0

typedef struct nodo nodo_t;
	
struct nodo{
	void* elemento;
	nodo_t* nodo_siguiente;
};

struct lista{
	nodo_t* nodo_inicio;
	nodo_t* nodo_final;
	size_t cantidad;
};

struct lista_iterador{
	nodo_t* corriente;
	lista_t* lista;
};

lista_t* lista_crear(){
	lista_t* lista=calloc(1,sizeof(lista_t));
	if (lista==NULL)
		return NULL;
	return lista;
}

/*
**PRE:Recibe una lista (no vacia) correctamente creada y la posicion.
**POST:Devuelve un puntero al nodo en la posicion.
*/
nodo_t* lista_iterar(lista_t* lista, size_t posicion){
	nodo_t* nodo=lista->nodo_inicio;
	for(size_t i=0;i<posicion;i++){ 
		nodo=nodo->nodo_siguiente;
	}
	return nodo;
}

int lista_insertar(lista_t* lista, void* elemento){
	nodo_t* nodo=calloc(1,sizeof(nodo_t)); //calloc para que nodo_siguiente apunte a NULL
	if(lista==NULL || nodo==NULL){
		free(nodo);
		return EXIT_ERROR;
	}
	nodo->elemento=elemento;
	if(lista->cantidad==VACIA){
		lista->nodo_inicio=nodo;
		lista->nodo_final=nodo;
	}
	else{
		(lista->nodo_final)->nodo_siguiente=nodo;
		lista->nodo_final=nodo;
	}
	(lista->cantidad)++;
	return SUCCESS;
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
	nodo_t* nodo=calloc(1,sizeof(nodo_t));
	if(lista==NULL || nodo==NULL){
		free(nodo);
		return EXIT_ERROR;
	}
	nodo->elemento=elemento;
	if (posicion >= lista->cantidad){
		lista_insertar(lista,elemento);
		free(nodo); //Porque lista_insertar ya creo el nodo
		(lista->cantidad)--; //lista_insertar aumento la cantidad en la lista y no quiero que se aumente 2 veces en esta funcion.
	}
	else if(posicion==0){
		nodo->nodo_siguiente=lista->nodo_inicio;
		lista->nodo_inicio=nodo;
	}
	else{
		nodo_t* aux=lista_iterar(lista,posicion -1);
		nodo->nodo_siguiente=aux->nodo_siguiente;
		aux->nodo_siguiente=nodo;
	}
	(lista->cantidad)++;
	return SUCCESS;
}

int lista_borrar(lista_t* lista){
	if (lista==NULL || lista->cantidad==VACIA)
		return EXIT_ERROR;
	if(lista->cantidad==1){ //caso particular
		free(lista->nodo_inicio);
		lista->nodo_inicio=NULL;
		lista->nodo_final=NULL;
	}
	else{
		nodo_t* nodo=lista_iterar(lista,lista->cantidad -2);
		free(lista->nodo_final);
		nodo->nodo_siguiente=NULL;
		lista->nodo_final=nodo;
	}
	(lista->cantidad)--;
	return SUCCESS;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
	nodo_t *nodo, *aux;
	if (lista==NULL || lista->cantidad==VACIA)
		return EXIT_ERROR;
	if ((posicion >= lista->cantidad)){
		lista_borrar(lista);
		(lista->cantidad)++; //lista_borrar disminuye la cantidad en uno y no quiero que se disminuya dos veces
	}
	else if(posicion==0){ //eliminando la primer posicion
		nodo=lista->nodo_inicio;
		lista->nodo_inicio=nodo->nodo_siguiente;
		free(nodo);
	}
	else{
		nodo=lista_iterar(lista,posicion -1);
		aux=nodo->nodo_siguiente;
		nodo->nodo_siguiente=aux->nodo_siguiente;
		free(aux);
		if(nodo->nodo_siguiente==NULL) //quiere decir que se eligio la ultima posicion
			lista->nodo_final=nodo;
	}
	(lista->cantidad)--;
	return SUCCESS;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
	if((lista==NULL) || (posicion >= lista->cantidad))
		return NULL;
	nodo_t* nodo=lista_iterar(lista,posicion);
	return (nodo->elemento);
}

void* lista_ultimo(lista_t* lista){
	if((lista==NULL) || (lista->cantidad==VACIA))
		return NULL;
	return ((lista->nodo_final)->elemento);
}

bool lista_vacia(lista_t* lista){
	if (lista==NULL)
		return true;
	return(lista->cantidad==VACIA);
}

size_t lista_elementos(lista_t* lista){
	if(lista==NULL)
		return VACIA;
	return (lista->cantidad);
}

void lista_destruir(lista_t* lista){
	if(lista!=NULL){
		if(lista->cantidad!=VACIA){
			nodo_t *nodo, *aux;
			nodo=lista->nodo_inicio;
			for(int i=0;i<lista->cantidad;i++){
				aux=nodo->nodo_siguiente;
				free(nodo);
				nodo=aux;
			}
		}	
		free(lista);	
	}
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){
	lista_iterador_t* iterador=calloc(1,sizeof(lista_iterador_t));
	if (lista==NULL || iterador==NULL){
		free(iterador);
		return NULL;
	}
	iterador->lista=lista;

	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
	if(iterador==NULL)
		return false;
	if(iterador->corriente==NULL)
		return((iterador->lista)->nodo_inicio != NULL);
	return ((iterador->corriente)->nodo_siguiente != NULL);

}
 
void* lista_iterador_siguiente(lista_iterador_t* iterador){
	if(iterador==NULL)
		return NULL;
	if(iterador->corriente==NULL){
		iterador->corriente=(iterador->lista)->nodo_inicio;
		return ((iterador->corriente)->elemento);
	}
	else{
		iterador->corriente=(iterador->corriente)->nodo_siguiente;
		return ((iterador->corriente)->elemento);
	}
}

void lista_iterador_destruir(lista_iterador_t* iterador){
	free(iterador);
}

void lista_con_cada_elemento(lista_t* lista, void (*funcion)(void*)){
	if (lista==NULL || funcion==NULL || lista->cantidad==VACIA)
		return;
	nodo_t* aux=lista->nodo_inicio;
	for(int i=0;i<lista->cantidad;i++){
		funcion(aux->elemento);
		aux=aux->nodo_siguiente;
	}
}
