#include "hash_iterador.h"
#include "lista.h"
#include <stdlib.h>
#include <string.h>
#define ERROR -1
#define EXITO 0
#define NO_ESTA -2
#define CRITERIO_REHASH 3

extern char* strdup(const char*);

struct hash{
	lista_t** tabla_hash;
	lista_t* claves;
	size_t capacidad;
	size_t cantidad;
	hash_destruir_dato_t destructor;
};

struct hash_iter{
	lista_iterador_t* iterador_lista;
};

//Estructura creada para insertar en el hash.
typedef struct nodo_hash{
	const char* clave;
	void* elemento;
}nodo_hash_t;

/*
 * Inicializa las listas en el campo tabla_hash de la estructura hash.
 * En caso de error devuelve -1, sino devuelve 0.
*/
int crear_listas(hash_t* hash){
	bool fallo=false;

	for(int i=0;i < (hash->capacidad);i++){ //creo las listas
		hash->tabla_hash[i]=lista_crear();
		if(hash->tabla_hash[i]==NULL)
			fallo=true;
	}
	if(fallo){
		for(int i=0;i < (hash->capacidad);i++)
			lista_destruir(hash->tabla_hash[i]);
		free(hash->tabla_hash); 
		free(hash);
		return ERROR;
	}
	return EXITO;
}

/*
 * Crea el hash reservando la memoria necesaria para el.
 * Destruir_elemento es un destructor que se utilizará para liberar
 * los elementos que se eliminen del hash.
 * Capacidad indica la capacidad minima inicial con la que se crea el hash.
 * Devuelve un puntero al hash creado o NULL en caso de no poder crearlo.
 */
hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad){
	hash_t* hash=calloc(1,sizeof(hash_t));

	if(!hash || capacidad==0){ //no tiene sentido un hash sin capacidad.
		free(hash);
		return NULL;
	}
	hash->capacidad=capacidad;
	hash->destructor=destruir_elemento;
	hash->tabla_hash=calloc(1, sizeof(lista_t*) * capacidad);
	if(!(hash->tabla_hash))
		return NULL;
	if(crear_listas(hash)==ERROR)
		return NULL;
	hash->claves=lista_crear();
	if(hash->claves==NULL)
		return NULL;
	return hash;
}

/*
 *Funcion hash, castea como unsigned short int cada caracter de la clave y lo suma,
 *luego a ese numero se le aplica modulo de la capacidad del hash.
*/
size_t funcion_hash(hash_t* hash, const char* clave){
	size_t valor_str,valor_hash;
	valor_str=0;
	for(int i=0;clave[i]!='\0';i++)
		valor_str += (unsigned short)clave[i];
	valor_hash = (valor_str % hash->capacidad);
	return valor_hash;

}

/*
 *Recibe una clave, le aplica la funcion hash y recorre en la lista correspondiente 
 *en busca de la clave, si la encuentra devuelve la posicion, en caso contrario devuelve -2 si no esta 
 * o -1 si hay un error de memoria. 
*/
int buscar_posicion(lista_t* lista, const char* clave){
	nodo_hash_t* nodo;
	int contador=0,posicion=NO_ESTA;
	lista_iterador_t* iterador=lista_iterador_crear(lista);
	if(!iterador)
		return ERROR; //aca se generaria un problema de falta de memoria que saltaria mas adelante en otras funciones.
	while(lista_iterador_tiene_siguiente(iterador) && (posicion == NO_ESTA) ){
		nodo=(nodo_hash_t*)lista_iterador_siguiente(iterador);
		if( strcmp(nodo->clave,clave)==0 ) //chequeo si coinciden claves
			posicion=contador;
		contador++;
	}
	lista_iterador_destruir(iterador);
	return posicion;
}

/*
 *Funcion que actualiza el valor de un elemento que ya estaba en el hash.
 *Borra las estructuras en memoria dinamica, reemplazando por las nuevas.
*/
int actualizar_elemento(lista_t* lista, const char* clave, void* nuevo_elemento, int posicion, hash_destruir_dato_t destructor){
	nodo_hash_t* nuevo_nodo=calloc(1,sizeof(nodo_hash_t));
	if(!nuevo_nodo)
		return ERROR;
	nuevo_nodo->clave=clave;
	nuevo_nodo->elemento=nuevo_elemento;

	nodo_hash_t* nodo_viejo=(nodo_hash_t*)lista_elemento_en_posicion(lista, (unsigned int)posicion);
	if(destructor!=NULL)
		destructor(nodo_viejo->elemento);
	free(nodo_viejo);
	if(lista_borrar_de_posicion(lista,(unsigned int)posicion)==ERROR){
		free(nuevo_nodo);
		return ERROR;
	}
	if(lista_insertar_en_posicion(lista,nuevo_nodo,(unsigned int)posicion)==ERROR){
		free(nuevo_nodo);
		return ERROR;
	}
	return EXITO;
}

/*
 *Funcion auxiliar de buscar_siguiente_primo. Chequea si un numero es primo.
*/
bool es_primo(size_t numero){
	bool tiene_divisores=false;
	size_t i=2; //No tiene sentido arrancar con 0 o 1. La capacidad es como minimo 1 y antes de entrar a esta funcion se aumenta el valor a 2.
	while(!tiene_divisores && i<numero){
		if( (numero%i) == 0)
			tiene_divisores=true;
		i++;
	}
	return(!tiene_divisores);
}

/*
 *Funcion auxiliar del rehash. Recibe un numero (la capacidad actual del hash) y devuelve el siguiente numero primo
 *para que sea la nueva capacidad del hash
*/
size_t buscar_siguiente_primo(size_t capacidad){
	bool encontre_primo=false;
	size_t nueva_capacidad=capacidad;
	while(!encontre_primo){
		nueva_capacidad++;
		if(es_primo(nueva_capacidad))
			encontre_primo=true;
	}
	return nueva_capacidad;
}

/*
 *Funcion auxiliar del rehash. 
 *Recibe el hash y una lista auxiliar. Pasa todos los elementos del hash a la lista auxiliar.
*/
void migrar_elementos(hash_t* hash, lista_t* lista_aux){
	for(int i=0;i < (hash->capacidad);i++){
		lista_iterador_t* iterador=lista_iterador_crear(hash->tabla_hash[i]);
		while(lista_iterador_tiene_siguiente(iterador)){
			void* elemento=lista_iterador_siguiente(iterador);
			lista_insertar(lista_aux,elemento);
		}
		lista_iterador_destruir(iterador);
		lista_destruir(hash->tabla_hash[i]); //destruyo la lista,todos los nodo_hash_t estan en la lista auxiliar asi que no se pierde memoria.
	}
}

/*
 *Funcion auxiliar del rehash.
 *Recibe el hash (vaciado) y la lista auxiliar con los elementos del hash cargados.
 *Itera la lista, insertando los en el hash.
*/
void reinsertar_elementos(hash_t* hash, lista_t* lista_aux){
	lista_iterador_t* iterador=lista_iterador_crear(lista_aux);
	while(lista_iterador_tiene_siguiente(iterador)){
		nodo_hash_t* nodo = (nodo_hash_t*)lista_iterador_siguiente(iterador);
		hash_insertar(hash,nodo->clave,nodo->elemento);
		free((char*)nodo->clave);
		free(nodo); //hash_insertar esta creando un nodo_hash_t extra, por eso tengo que hacer free de este.
	}
	lista_iterador_destruir(iterador);
}

/*
 *Funcion de rehasheo. Se activa cuando la cantidad de elementos del hash
 *supera el triple del campo capacidad del hash. La nueva capacidad es el siguiente numero primo.
*/
int rehash(hash_t* hash){
	lista_t* lista_aux=lista_crear();
	migrar_elementos(hash,lista_aux);
	size_t nueva_capacidad=buscar_siguiente_primo(hash->capacidad);
	lista_t** tabla_aux=hash->tabla_hash; //el auxiliar para el realloc
	hash->cantidad=0; //voy a reinsertar los elementos, asi que la cantidad tiene que volver a 0.
	lista_destruir(hash->claves);
	hash->claves = lista_crear(); //creo y destruyo la lista de claves para que no aparezcan claves repetidas
	hash->tabla_hash=realloc(hash->tabla_hash,sizeof(lista_t*) * nueva_capacidad);
	if(hash->tabla_hash == NULL){
		hash->tabla_hash=tabla_aux;
		crear_listas(hash);
		reinsertar_elementos(hash,lista_aux);
		lista_destruir(lista_aux);
		return ERROR;
	}
	hash->capacidad=nueva_capacidad;
	crear_listas(hash);
	reinsertar_elementos(hash,lista_aux); //esta operacion se haria con la tabla de hash realocada con mayor longitud.
	lista_destruir(lista_aux);
	return EXITO;
}

/*
 * Inserta un elemento reservando la memoria necesaria para el mismo.
 * Devuelve 0 si pudo guardarlo o -1 si no pudo.
 */
int hash_insertar(hash_t* hash, const char* clave, void* elemento){
	nodo_hash_t* nodo=calloc(1,sizeof(nodo_hash_t));
	if(!hash || !nodo || !clave){ 
		free(nodo);
		return ERROR;	
	}
	nodo->clave=strdup(clave);
	nodo->elemento=elemento;

	size_t valor_hash=funcion_hash(hash,clave);
	int posicion=buscar_posicion(hash->tabla_hash[valor_hash],clave);
	if(posicion==ERROR)
		return ERROR;
	else if(posicion==NO_ESTA){
		if(lista_insertar(hash->tabla_hash[valor_hash],nodo)==ERROR)
			return ERROR;
		(hash->cantidad)++;
		if(lista_insertar(hash->claves,(char*)nodo->clave)==ERROR) //casteo a (char*) para que el compilador no se queje por ser const char*
			return ERROR;
	}
	else{
		free((char*)nodo->clave); //alocado con strdup.
		free(nodo); //porque no lo necesito para actualizar el elemento
		actualizar_elemento(hash->tabla_hash[valor_hash],clave,elemento,posicion,hash->destructor);
	}
	if( (CRITERIO_REHASH * hash->capacidad) < hash->cantidad){
		if(rehash(hash)==ERROR)
			return ERROR;
	}
	return EXITO;
}

/*
 *Funcion auxiliar de hash quitar. Al eliminar un elemento del hash tambien
 *se tiene que eliminar su clave del campo "claves" del hash. Esta funcion itera es lista y elimina la clave.
*/
int borrar_clave(lista_t* claves, const char* clave_a_eliminar){
	lista_iterador_t* iterador=lista_iterador_crear(claves);
	bool encontre=false;
	int posicion_de_clave=-1;
	while(lista_iterador_tiene_siguiente(iterador) && !encontre){
		const char* clave=(const char*)lista_iterador_siguiente(iterador);
		if(strcmp(clave,clave_a_eliminar)==0){
			free((char*)clave);
			encontre=true;
		}
		posicion_de_clave++;
	}
	if(encontre){
		if(lista_borrar_de_posicion(claves,(size_t)posicion_de_clave)==ERROR)
			return ERROR;
	}

	lista_iterador_destruir(iterador);
	return EXITO;
}

/*
 * Quita un elemento del hash e invoca la funcion destructora
 * pasandole dicho elemento.
 * Devuelve 0 si pudo eliminar el elemento o -1 si no pudo.
 */
int hash_quitar(hash_t* hash, const char* clave){
	if(!hash || !clave)
		return ERROR;
	size_t valor_hash=funcion_hash(hash,clave);
	int posicion=buscar_posicion(hash->tabla_hash[valor_hash],clave);
	if(posicion==ERROR || posicion==NO_ESTA)
		return ERROR;
	nodo_hash_t* nodo_a_destruir=(nodo_hash_t*)lista_elemento_en_posicion(hash->tabla_hash[valor_hash],(unsigned int)posicion);
	if( (hash->destructor) != NULL)
		hash->destructor(nodo_a_destruir->elemento);
	free(nodo_a_destruir);
	(hash->cantidad)--;
	if(lista_borrar_de_posicion(hash->tabla_hash[valor_hash],(unsigned int)posicion)== ERROR)
		return ERROR;
	if(borrar_clave(hash->claves,clave)==ERROR)
		return ERROR;
	return EXITO;
}

/*
 * Devuelve un elemento del hash con la clave dada o NULL si dicho
 * elemento no existe.
 */
void* hash_obtener(hash_t* hash, const char* clave){
	if(!hash || !clave)
		return NULL;
	size_t valor_hash=funcion_hash(hash,clave);
	int posicion=buscar_posicion(hash->tabla_hash[valor_hash],clave);
	if(posicion==ERROR || posicion==NO_ESTA)
		return NULL;
	nodo_hash_t* nodo=(nodo_hash_t*)lista_elemento_en_posicion(hash->tabla_hash[valor_hash],(unsigned int)posicion);
	return (nodo->elemento);
}

/*
 * Devuelve true si el hash contiene un elemento almacenado con la
 * clave dada o false en caso contrario.
 */
bool hash_contiene(hash_t* hash, const char* clave){
	if(!hash || !clave)
		return false;
	size_t valor_hash=funcion_hash(hash,clave);
	int posicion=buscar_posicion(hash->tabla_hash[valor_hash],clave);
	if(posicion==ERROR || posicion==NO_ESTA)
		return false;
	return true;
}

/*
 * Devuelve la cantidad de elementos almacenados en el hash.
 */
size_t hash_cantidad(hash_t* hash){
	if(!hash)
		return 0;
	return (hash->cantidad);
}

/*
 *Funcion auxiliar de hash_destruir. Itera una lista aplicando el destructor a todos los elementos.
*/
void  vaciar_lista(lista_t* lista,hash_destruir_dato_t destructor){
	nodo_hash_t* nodo;
	lista_iterador_t* iterador=lista_iterador_crear(lista);
	if(!iterador)
		return;
	while(lista_iterador_tiene_siguiente(iterador)){
		nodo=(nodo_hash_t*)lista_iterador_siguiente(iterador);
		if(destructor != NULL)
			destructor(nodo->elemento);
		free(nodo);
	}
	lista_iterador_destruir(iterador);
}

/*
 * Funcion auxiliar de hash_destruir, libera las claves almacenadas con strdup en la lista "claves"
*/
void liberar_claves(lista_t* claves){
	lista_iterador_t* iterador=lista_iterador_crear(claves);
	void* nodo;
	while(lista_iterador_tiene_siguiente(iterador)){
		nodo=lista_iterador_siguiente(iterador);
		free((char*)nodo);
	}
	lista_iterador_destruir(iterador);
}

/*
 * Destruye el hash liberando la memoria reservada y asegurandose de
 * invocar la funcion destructora con cada elemento almacenado en el
 * hash.
 */
void hash_destruir(hash_t* hash){
	if(!hash)
		return;
	for(int i=0;i<(hash->capacidad);i++){
		vaciar_lista(hash->tabla_hash[i],hash->destructor);
		lista_destruir(hash->tabla_hash[i]);
	}
	free(hash->tabla_hash);
	liberar_claves(hash->claves);
	lista_destruir(hash->claves);
	free(hash);
}

/*
 * Crea un iterador de claves para el hash reservando la memoria
 * necesaria para el mismo. El iterador creado es válido desde su
 * creación hasta que se modifique la tabla de hash (insertando o
 * removiendo elementos);
 *
 * Devuelve el puntero al iterador creado o NULL en caso de error.
 */
hash_iterador_t* hash_iterador_crear(hash_t* hash){
	if(!hash)
		return NULL;
	hash_iterador_t* iterador_hash=calloc(1,sizeof(hash_iterador_t));
	if(!iterador_hash)
		return NULL;
	iterador_hash->iterador_lista=lista_iterador_crear(hash->claves);
	if(iterador_hash->iterador_lista == NULL)
		return NULL;
	return iterador_hash;
}

/*
 * Devuelve true si quedan claves por recorrer o false en caso
 * contrario.
 */
bool hash_iterador_tiene_siguiente(hash_iterador_t* iterador){
	if(!iterador)
		return false; 
	return(lista_iterador_tiene_siguiente(iterador->iterador_lista));
}

/*
 * Devuelve la próxima clave almacenada en el hash y avanza el iterador.
 * Devuelve la clave o NULL si no habia mas.
 */
void* hash_iterador_siguiente(hash_iterador_t* iterador){
	if(!iterador)
		return NULL;
	return(lista_iterador_siguiente(iterador->iterador_lista));
}

/*
 * Destruye el iterador del hash.
 */
void hash_iterador_destruir(hash_iterador_t* iterador){
	if(!iterador)
		return;
	lista_iterador_destruir(iterador->iterador_lista);
	free(iterador);
}
