#include "lista.h"
#include "cola.h"
#include <stdlib.h>
#define EXITO 0
#define ERROR -1

struct lista{
		cola_t* cola;
};

struct lista_iterador{
	cola_t* cola;
	int tope; //el tope me dice cuando dejar de iterar
};

/*
 * Crea la lista reservando la memoria necesaria.
 * Devuelve un puntero a la lista creada o NULL en caso de error.
 */
lista_t* lista_crear(){
	lista_t* lista=malloc(sizeof(lista_t));
	if(!lista)	
		return NULL;
	lista->cola=cola_crear(); 
	if(!lista->cola){
		free(lista);
		return NULL;
	}
	return lista;
}

/*
 * Inserta un elemento al final de la lista.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 */
int lista_insertar(lista_t* lista, void* elemento){
	if(!lista)
		return ERROR;
	int estado=EXITO;
	estado=cola_encolar(lista->cola,elemento);
	return estado;
}

/*
 * Recibe una posicion n, desencola y encola n elementos para que el primer 
 * elemento de la cola sea la posicion buscada. La cola queda desordenada, por lo que se debe llamar 
 * nuevamente a la funcion con el numero n-cantidad_cola para que la cola quede ordenada.
*/
int obtener_posicion(cola_t* cola,size_t posicion){
	int estado=EXITO;
	for(size_t i=0;i<posicion;i++){
		void* elemento=cola_primero(cola);
		estado=cola_desencolar(cola);
		estado=cola_encolar(cola,elemento);
	}
	return estado;
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
	if(!lista)
		return ERROR;
	int estado=EXITO;
	if(posicion>(size_t)cola_cantidad(lista->cola))
		return lista_insertar(lista,elemento);
	size_t posiciones_a_corregir= (size_t)cola_cantidad(lista->cola) - posicion;
	estado=obtener_posicion(lista->cola,posicion);
	estado=cola_encolar(lista->cola,elemento);
	estado=obtener_posicion(lista->cola,posiciones_a_corregir); //para dejar la cola ordenada.
	return estado;
}

/*
 * Quita de la lista el elemento que se encuentra en la ultima posición.
 * Devuelve 0 si pudo eliminar o -1 si no pudo.
 */
int lista_borrar(lista_t* lista){
	if(lista_vacia(lista))
		return ERROR;
	else return lista_borrar_de_posicion(lista,(size_t)cola_cantidad(lista->cola)-1); //Es un caso particular de insertar en posicion.
}

/*
 * Quita de la lista el elemento que se encuentra en la posición
 * indicada, donde 0 es el primer elemento.  
 * En caso de no existir esa posición se intentará borrar el último
 * elemento.  
 * Devuelve 0 si pudo eliminar o -1 si no pudo.
 */
int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
	if(lista_vacia(lista))
		return ERROR;
	int estado=EXITO;
	if(posicion>(size_t)cola_cantidad(lista->cola))
		lista_borrar(lista);
	size_t posiciones_a_corregir=(size_t)cola_cantidad(lista->cola) - posicion;
	estado=obtener_posicion(lista->cola,posicion);
	estado=cola_desencolar(lista->cola);
	estado=obtener_posicion(lista->cola,posiciones_a_corregir-1); //pongo -1 porque borre un elemento.
	return estado;
}

/*
 * Devuelve el elemento en la posicion indicada, donde 0 es el primer
 * elemento.
 *
 * Si no existe dicha posicion devuelve NULL.
 */
void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
	if(lista_vacia(lista) || posicion>(size_t)cola_cantidad(lista->cola))
		return NULL;
	size_t posiciones_a_corregir=(size_t)cola_cantidad(lista->cola) - posicion;
	int estado=obtener_posicion(lista->cola,posicion);
	void* elemento=cola_primero(lista->cola);
	estado=obtener_posicion(lista->cola,posiciones_a_corregir);
	if(estado==ERROR)
		return NULL;
	return elemento;
}

/* 
 * Devuelve el último elemento de la lista o NULL si la lista se
 * encuentra vacía.
 */
void* lista_ultimo(lista_t* lista){
	if(lista_vacia(lista))
		return NULL;
	else return lista_elemento_en_posicion(lista,(size_t)cola_cantidad(lista->cola)-1); //Es un caso particular de elemento en posicion.
}

/* 
 * Devuelve true si la lista está vacía o false en caso contrario.
 */
bool lista_vacia(lista_t* lista){
	if(!lista)
		return true;
	else return cola_vacia(lista->cola);
}

/*
 * Devuelve la cantidad de elementos almacenados en la lista.
 */
size_t lista_elementos(lista_t* lista){
	if(!lista)
		return 0;
	else return (size_t)cola_cantidad(lista->cola);
}

/*
 * Libera la memoria reservada por la lista.
 */
void lista_destruir(lista_t* lista){
	if(!lista)
		return;
	cola_destruir(lista->cola);
	free(lista);
}

/*
 * Crea un iterador para una lista. El iterador creado es válido desde
 * el momento de su creación hasta que no haya mas elementos por
 * recorrer o se modifique la lista iterada (agregando o quitando
 * elementos de la lista).
 *
 * Devuelve el puntero al iterador creado o NULL en caso de error.
 */
lista_iterador_t* lista_iterador_crear(lista_t* lista){
	if(!lista)
		return NULL;
	lista_iterador_t* iterador=malloc(sizeof(lista_iterador_t));
	if(!iterador)
		return NULL;
	iterador->cola=lista->cola;
	iterador->tope=cola_cantidad(lista->cola);
	return iterador;
}

/*
 * Devuelve true si hay mas elementos sobre los cuales iterar o false
 * si no hay mas.
 */
bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
	if(!iterador)
		return false;
	return (iterador->tope>0);
}

/*
 * Avanza el iterador al proximo elemento y lo devuelve.
 * En caso de error devuelve NULL.
 */
void* lista_iterador_siguiente(lista_iterador_t* iterador){
	if(!iterador || (iterador->tope) <= 0)
		return NULL;
	void* elemento=cola_primero(iterador->cola);
	cola_desencolar(iterador->cola);
	cola_encolar(iterador->cola,elemento);
	(iterador->tope)--;
	return elemento;
}

/*
 * Libera la memoria reservada por el iterador.
 */
void lista_iterador_destruir(lista_iterador_t* iterador){
	free(iterador);
}

/*
 * Iterador interno. Recorre la lista e invoca la funcion con cada
 * elemento de la misma.
 */
void lista_con_cada_elemento(lista_t* lista, void (*funcion)(void*)){
	if(lista_vacia(lista))
		return;
	for(int i=0;i<cola_cantidad(lista->cola);i++){
		void* elemento=cola_primero(lista->cola);
		funcion(elemento);
		cola_desencolar(lista->cola);
		cola_encolar(lista->cola,elemento);
	}
}
