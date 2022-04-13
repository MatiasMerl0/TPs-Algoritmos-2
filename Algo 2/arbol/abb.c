#include "abb.h"
#include <stdbool.h>
#include <stdlib.h>

#define ERROR -1
#define EXITO 0
#define IGUALES 0
#define MENOR -1
#define MAYOR 1
#define INSERTAR 2
#define YA_INSERTE -2
#define BORRAR 2

abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){
	abb_t* arbol=calloc(1,sizeof(abb_t));
	if(arbol==NULL || comparador==NULL){
		free(arbol);
		return NULL;
	}
	arbol->comparador=comparador;
	arbol->destructor=destructor;
	return arbol;
}

/*
* Recibe el nodo a insertar, y recursivamente recorre el arbol usando el comparador hasta llegar al lugar (null) donde debe insertar.
* Cuando el elemento es repetido, se mueve a la derecha.
*/
int insertar_recursivo(nodo_abb_t* nodo,abb_comparador comparador, nodo_abb_t* nodo_a_insertar){
	int estado;
	if(nodo==NULL)
		return INSERTAR;
	if(comparador(nodo_a_insertar->elemento,nodo->elemento)==MAYOR || comparador(nodo_a_insertar->elemento,nodo->elemento)==IGUALES){ // si el elemento ese mayor o igual, voy a la derecha.
		estado=insertar_recursivo(nodo->derecha,comparador,nodo_a_insertar);
		if(estado==INSERTAR)
			nodo->derecha=nodo_a_insertar;
	}
	else if(comparador(nodo_a_insertar->elemento,nodo->elemento)==MENOR){
		estado=insertar_recursivo(nodo->izquierda,comparador,nodo_a_insertar);
		if(estado==INSERTAR)
			nodo->izquierda=nodo_a_insertar;
	}
	return YA_INSERTE;
}

 int arbol_insertar(abb_t* arbol, void* elemento){
 	nodo_abb_t* nodo=calloc(1,sizeof(nodo_abb_t));

 	if(arbol==NULL || nodo==NULL || arbol->comparador==NULL){
 		free(nodo);
 		return ERROR;
 	}
 	nodo->elemento=elemento;
 	if(arbol_vacio(arbol)) //caso particular
 		arbol->nodo_raiz=nodo;
 	else insertar_recursivo(arbol->nodo_raiz,arbol->comparador,nodo);
 	return EXITO;

 }

/*
* Funcion auxiliar de "buscar_mayor_de_menores", se usa sobre el hijo izquierdo del nodo que quiero borrar.
* Obtiene el padre del mayor de estos nodos, para poder manejar bien las referencias sin perder nada.
*/
nodo_abb_t* buscar_padre_de_mayor(nodo_abb_t* nodo){
	if((nodo->derecha)->derecha==NULL)
		return nodo;
	else return(buscar_padre_de_mayor(nodo->derecha));
}

/*
* Busca al nodo mayor de menores de la raiz, ordenando todos los punteros correctamente y 
* devuelve el nodo que es el mayor de los menores.
*/
nodo_abb_t* buscar_mayor_de_menores(nodo_abb_t* nodo){
	nodo_abb_t *nueva_raiz, *auxiliar;
	if((nodo->izquierda)->derecha==NULL){ //caso particular
		nueva_raiz=nodo->izquierda;
		nodo->izquierda=(nueva_raiz)->izquierda; 
		return nueva_raiz;
	}
	auxiliar=buscar_padre_de_mayor(nodo->izquierda);
	nueva_raiz=auxiliar->derecha;
	auxiliar->derecha=nueva_raiz->izquierda;
	return nueva_raiz;
}
/*
* Cheque los 4 posibles casos en los que un nodo puede tener hijos y lo borra ordenadamente, devolviendo el nodo que tomara 
* su lugar en el arbol. En el caso que tengan dos hijos, el nodo que sera la nueva raiz es el mayor de los menores.
*/
nodo_abb_t* borrar_nodo(nodo_abb_t* nodo, abb_liberar_elemento destructor){
	if(nodo->izquierda==NULL && nodo->derecha==NULL){
		if(destructor!=NULL)
			destructor(nodo->elemento);
		free(nodo);
		return NULL; //porque quiero que el padre apunte a NULL
	}
	if(nodo->izquierda==NULL && nodo->derecha!=NULL){
		nodo_abb_t* aux=nodo->derecha; //para no perder la referencia
		if(destructor!=NULL)
			destructor(nodo->elemento);
		free(nodo);
		return aux;
	}
	if(nodo->derecha==NULL && nodo->izquierda!=NULL){
		nodo_abb_t* aux=nodo->izquierda; 
		if(destructor!=NULL)
			destructor(nodo->elemento);
		free(nodo);
		return aux;
	}
	else{
		nodo_abb_t* aux=buscar_mayor_de_menores(nodo);
		aux->izquierda=nodo->izquierda;
		aux->derecha=nodo->derecha;
		if(destructor!=NULL)
			destructor(nodo->elemento);
		free(nodo);
		return aux;
	}

}
/*
* Funcion recursiva que recorre el arbol hasta encontrar el elemento a borrar, si no lo encuentra devuelve -1, 
* si lo encuentra lo borra y devuelve 0.
*/
int borrar_recursivo(nodo_abb_t* nodo,void* elemento,abb_comparador comparador,abb_liberar_elemento destructor){
	if(nodo==NULL)
		return ERROR;
	int comparacion,estado;
	comparacion=comparador(elemento,nodo->elemento);
	if(comparacion==IGUALES)
		return BORRAR;
	else if(comparacion==MAYOR){
		estado=borrar_recursivo(nodo->derecha,elemento,comparador,destructor);
		if(estado==BORRAR){
			nodo->derecha=borrar_nodo(nodo->derecha,destructor);
			return EXITO;
		}
		return estado;
	}
	else {
		estado=borrar_recursivo(nodo->izquierda,elemento,comparador,destructor);
		if(estado==BORRAR){
			nodo->izquierda=borrar_nodo(nodo->izquierda,destructor);
			return EXITO;
		}
		return estado;
	}

}

int arbol_borrar(abb_t* arbol, void* elemento){
	if(arbol_vacio(arbol))
		return ERROR;
	if(arbol->comparador((arbol->nodo_raiz)->elemento,elemento)==IGUALES){  //caso especial
		arbol->nodo_raiz=borrar_nodo(arbol->nodo_raiz,arbol->destructor);	
		return EXITO;
	}
	return borrar_recursivo(arbol->nodo_raiz,elemento,arbol->comparador,arbol->destructor);
}

/*
* Recorre el arbol recursivamente usando la funcion de comparacion, si los elementos comparados son iguales, devuelve el elemento encontrado, 
* si no lo encuentra devuelve null.
*/
void* buscar_recursivo(nodo_abb_t* nodo,void* elemento,abb_comparador comparador){
	if(nodo==NULL || comparador==NULL)
		return NULL;
	int comparacion=comparador(nodo->elemento,elemento);
	switch (comparacion){
		case IGUALES:
			return nodo->elemento;
		case MAYOR: //Si el nodo sobre el que estoy parado es mayor que el elemento que busco, me muevo para la izquierda.
			return(buscar_recursivo(nodo->izquierda,elemento,comparador));
		default: //Si el nodo sobre el que estoy parado es menor que el elemento que busco, me muevo a la derecha.
			return(buscar_recursivo(nodo->derecha,elemento,comparador));
	}
}

void* arbol_buscar(abb_t* arbol, void* elemento){
	if(arbol_vacio(arbol)) //aca dentro tambien chequea si arbol es NULL
		return NULL;
	else return buscar_recursivo(arbol->nodo_raiz,elemento,arbol->comparador);
}

void* arbol_raiz(abb_t* arbol){
	if(arbol_vacio(arbol))
		return NULL;
	else return ((arbol->nodo_raiz)->elemento);
}

bool arbol_vacio(abb_t* arbol){
	if(arbol==NULL || arbol->nodo_raiz==NULL)
		return true;
	else return false;
}

/*
* Funcion auxiliar para los recorridos, si el tamanio del array lo permite, mete un elemento en el.
*/
void meter_en_array(void* elemento, void** array,int tamanio_array, int* cantidad_elementos){
	if(tamanio_array>(*cantidad_elementos)){
		array[(*cantidad_elementos)]=elemento;
		(*cantidad_elementos)++;
	}
}

/*
* Funcion recursiva para el recorrido inorden.
*/
void inorden_recursivo(nodo_abb_t* nodo, void** array, int tamanio_array, int* cantidad_elementos){
	if(nodo==NULL || tamanio_array==(*cantidad_elementos) ) 
		return;
	inorden_recursivo(nodo->izquierda, array, tamanio_array, cantidad_elementos);
	meter_en_array(nodo->elemento,array,tamanio_array,cantidad_elementos);
	inorden_recursivo(nodo->derecha, array, tamanio_array, cantidad_elementos);
	return;
}

int arbol_recorrido_inorden(abb_t* arbol, void** array, int tamanio_array){
	if(arbol_vacio(arbol) || tamanio_array==0 || array==NULL)
		return 0;
	int cantidad_elementos=0;
	inorden_recursivo(arbol->nodo_raiz,array,tamanio_array,&cantidad_elementos);
	return cantidad_elementos;
}

/*
* Funcion recursiva para recorrido preorden.
*/
void preorden_recursivo(nodo_abb_t* nodo,void** array, int tamanio_array,int* cantidad_elementos){
	if(nodo==NULL || tamanio_array==(*cantidad_elementos) ) 
		return;
	meter_en_array(nodo->elemento,array,tamanio_array,cantidad_elementos);
	preorden_recursivo(nodo->izquierda, array, tamanio_array, cantidad_elementos);
	preorden_recursivo(nodo->derecha, array, tamanio_array, cantidad_elementos);
	return;
}

int arbol_recorrido_preorden(abb_t* arbol, void** array, int tamanio_array){
	if(arbol_vacio(arbol) || tamanio_array==0 || array==NULL)
		return 0;
	int cantidad_elementos=0;
	preorden_recursivo(arbol->nodo_raiz,array,tamanio_array,&cantidad_elementos);
	return cantidad_elementos;
}

/*
* Funcion recursiva para el recorrido postorden.
*/
void postorden_recursivo(nodo_abb_t* nodo,void** array,int tamanio_array,int* cantidad_elementos){
	if(nodo==NULL || tamanio_array==(*cantidad_elementos) ) 
		return;
	postorden_recursivo(nodo->izquierda, array, tamanio_array, cantidad_elementos);
	postorden_recursivo(nodo->derecha, array, tamanio_array, cantidad_elementos);
	meter_en_array(nodo->elemento,array,tamanio_array,cantidad_elementos);
	return;
}

int arbol_recorrido_postorden(abb_t* arbol, void** array, int tamanio_array){
	if(arbol_vacio(arbol) || tamanio_array==0 || array==NULL)
		return 0;
	int cantidad_elementos=0;
	postorden_recursivo(arbol->nodo_raiz,array,tamanio_array,&cantidad_elementos);
	return cantidad_elementos;
}

/*
* Recorre recursivamente el arbol (en forma postorden) y en caso de que haya una funcion destructora
* entonces la aplica sobre cada elemento. Luego hace free del nodo sobre el que esta parado.
*/
void destruir_recursivo(nodo_abb_t* nodo,abb_liberar_elemento destructor){
	if(nodo!=NULL){
		destruir_recursivo(nodo->izquierda,destructor);
		destruir_recursivo(nodo->derecha,destructor);
		if(destructor!=NULL) //En caso de que el usuario decida no pasar una funcion de destruccion.	
			destructor(nodo->elemento);
		free(nodo);
	}
}

void arbol_destruir(abb_t* arbol){
	if(arbol!=NULL){
		destruir_recursivo(arbol->nodo_raiz,arbol->destructor);
	}
	free(arbol);
}

/*
* Recorrido inorden aplicando la funcion recibida por el usuario.
*/
void inorden_recursivo_interno(nodo_abb_t* nodo,bool (*funcion)(void*, void*), void* extra,bool* cortar_recorrido){
	if(nodo==NULL || (*cortar_recorrido))
		return;
	inorden_recursivo_interno(nodo->izquierda,funcion,extra,cortar_recorrido);
	if(!(*cortar_recorrido)) //si la funcion cambio a true en la linea anterior, no quiero reasignarle un valor pues tiene que dejar de iterar ahora.
		(*cortar_recorrido)=funcion(nodo->elemento,extra);
	inorden_recursivo_interno(nodo->derecha,funcion,extra,cortar_recorrido);
}

/*
* Recorrido preorden aplicando la funcion recibida por el usuario.
*/
void preorden_recursivo_interno(nodo_abb_t* nodo,bool (*funcion)(void*, void*), void* extra,bool* cortar_recorrido){
	if(nodo==NULL || (*cortar_recorrido))
		return;
	*(cortar_recorrido)=funcion(nodo->elemento,extra);
	preorden_recursivo_interno(nodo->izquierda,funcion,extra,cortar_recorrido);
	preorden_recursivo_interno(nodo->derecha,funcion,extra,cortar_recorrido);
}

/*
* Recorrido postorden aplicando la funcion recibida por el usuario.
*/
void postorden_recursivo_interno(nodo_abb_t* nodo,bool (*funcion)(void*, void*), void* extra,bool* cortar_recorrido){
	if(nodo==NULL || (*cortar_recorrido))
		return;
	postorden_recursivo_interno(nodo->izquierda,funcion,extra,cortar_recorrido);
	postorden_recursivo_interno(nodo->derecha,funcion,extra,cortar_recorrido);
	if(!(*cortar_recorrido)) //En caso de que haya cambiado a true en las lineas anteriores.
		(*cortar_recorrido)=funcion(nodo->elemento,extra);
}


void abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
	if(arbol_vacio(arbol) || funcion==NULL)
		return;
	bool cortar_recorrido=false;
	if(recorrido== ABB_RECORRER_INORDEN)
		inorden_recursivo_interno(arbol->nodo_raiz,funcion,extra,&cortar_recorrido);
	else if(recorrido==ABB_RECORRER_PREORDEN)
		preorden_recursivo_interno(arbol->nodo_raiz,funcion,extra,&cortar_recorrido);
	else if(recorrido==ABB_RECORRER_POSTORDEN)
		postorden_recursivo_interno(arbol->nodo_raiz,funcion,extra,&cortar_recorrido);
}