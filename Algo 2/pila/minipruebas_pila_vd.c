#include "pila.h"
#include <stdio.h>
#include <stdbool.h>
#define ERROR -1
#define TAMANIO_PILA 10
void PilaVieneVacia_CuandoDesapilo_NoHaceNadaYDevuelveMenosUno(pila_t* pila){
	int prueba=pila_desapilar(pila); 
	bool paso=false;
	if (prueba==ERROR && pila->tope==0){
		paso=true;
	}
	printf("Pila vacia, no desapila: %s\n",paso?"Paso":"No paso" );
}

void PilaVieneVacia_CuandoPreguntoSiEstaVacia_DevuelveTrue(pila_t* pila){
	bool paso=pila_vacia(pila);
	printf("Pila vacia, devuelve que esta vacia: %s\n",paso?"Paso":"No paso" );
}

void PilaVieneVacia_CuandoPreguntoCantidad_DevuelveCero(pila_t* pila){
	int prueba=pila_cantidad(pila);
	bool paso=(prueba==0);
	printf("Pila vacia, devuelve que la cantidad de elementos es 0: %s\n",paso?"Paso":"No paso" );
}

void PilaVieneVacia_CuandoAgrego30Elementos_TamanioSeExpandeA40YCantidadEs30(pila_t* pila, char* el1, char* el2, char* el3, char* el4, char* el5){
	for (int i = 0; i < 7; i++) {
		pila_apilar(pila, el1);
		pila_apilar(pila, el2);
		pila_apilar(pila, el3);
		pila_apilar(pila, el4);
		pila_apilar(pila, el5);
	}
	printf("Tamanio se expande a 40: %s\n",pila->tamanio==40?"Paso":"No paso");
	
}

void PilaVieneConElementos_CuandoDesapiloEImprimo_ImprimeLosElementoscorrectos(pila_t* pila, char el1, char el2, char el3, char el4, char el5){
	printf("Salida esperada:\n%c\n%c\n%c\n%c\n%c\n",el5,el4,el3,el2,el1);
	printf("Salida obtenida:\n");
	for (int i = 0; i < 5; i++) {
		printf("%c\n", *(char*)pila_tope(pila));
		pila_desapilar(pila);
	}
}

void PilaVieneConElementos_CuandoQuieroDesapilarMucho_PilaSeAchicaYSePoneTopeEnCero(pila_t* pila){
	bool paso=false;
	for (int i = 0; i < 50; i++) 
		pila_desapilar(pila);
	if(pila_vacia(pila) && pila->tope==0 && pila->tamanio==TAMANIO_PILA){ //chequeo que el tope no se haya ido a negativo.
		paso=true;
	}
	printf("Se desapila muchas veces, la pila se vacia y se achica el tamanio: %s\n",paso?"Paso":"No paso" );
}

void NoExistePila_IntentoApilar_DevuelveError(pila_t* pila,void* elemento){
	printf("No existe pila, se intenta apilar y no pasa nada: %s\n",pila_apilar(pila,elemento)==ERROR?"Paso":"No Paso" );
}

void NoExistePila_IntentoDesapilar_DevuelveError(pila_t* pila){
	printf("No existe pila, se intenta desapilar y no pasa nada: %s\n",pila_desapilar(pila)==ERROR?"Paso":"No paso" );
}

void NoExistePila_IntentoVerLaCantidad_DevuelveCero(pila_t* pila){
	printf("No existe pila, se intenta ver cantidad y devuelve 0: %s\n",pila_cantidad(pila)==0?"Paso":"No paso" );
}

void NoExistePila_IntentoObtenerElTope_DevuelveNULL(pila_t* pila){
	printf("No existe pila, se intenta obtener el tope y devuelve NULL: %s\n",pila_tope(pila)==NULL?"Paso":"No paso" );
}

void NoExistePila_IntentoVerSiEstaVacia_Devuelvetrue(pila_t* pila){
	printf("No existe pila, se intenta ver si está vacia y devuelve true: %s\n",pila_vacia(pila)?"Paso":"No paso" );
}
int main(){
	pila_t* pila = NULL;
	
	char elemento_1 = '2';
	char elemento_2 = 'o';
	char elemento_3 = 'g';
	char elemento_4 = 'l';
	char elemento_5 = 'A';
	
	//Pruebas con pila que no existe (pila_t* apunta a NULL)
	NoExistePila_IntentoApilar_DevuelveError(pila,&elemento_1);
	NoExistePila_IntentoDesapilar_DevuelveError(pila);
	NoExistePila_IntentoVerSiEstaVacia_Devuelvetrue(pila);
	NoExistePila_IntentoVerLaCantidad_DevuelveCero(pila);
	NoExistePila_IntentoObtenerElTope_DevuelveNULL(pila);

	//Pruebas con la pila vacia
	pila = pila_crear();

	PilaVieneVacia_CuandoDesapilo_NoHaceNadaYDevuelveMenosUno(pila);
	PilaVieneVacia_CuandoPreguntoSiEstaVacia_DevuelveTrue(pila);
	PilaVieneVacia_CuandoPreguntoCantidad_DevuelveCero(pila);

	//Pruebas apilando elementos
	PilaVieneVacia_CuandoAgrego30Elementos_TamanioSeExpandeA40YCantidadEs30(pila,&elemento_1,&elemento_2,&elemento_3,&elemento_4,&elemento_5);

	//Pruebas chequeando el orden de los elementos de la pila
	PilaVieneConElementos_CuandoDesapiloEImprimo_ImprimeLosElementoscorrectos(pila,elemento_1,elemento_2,elemento_3,elemento_4,elemento_5);

	//Pruebas desapilando muchos elementos y chequeando que todos los campos esten en orden
	PilaVieneConElementos_CuandoQuieroDesapilarMucho_PilaSeAchicaYSePoneTopeEnCero(pila);
	
	pila_destruir(pila);
	return 0;
}