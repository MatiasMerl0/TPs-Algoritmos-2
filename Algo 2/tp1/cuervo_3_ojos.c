#include "cuervo_3_ojos.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PASADO 'P'
#define FUTURO 'F'
#define EXIT_ERROR -1
#define SUCCESS 0

/*
**Descripcion en el .h
*/
cuervo_tres_ojos_t* invocar_cuervo(char* nombre){
	cuervo_tres_ojos_t* cuervo_maestro=calloc(1,sizeof(cuervo_tres_ojos_t)); //Calloc para inicializar topes en 0
	strcpy(cuervo_maestro->nombre,nombre);
	cuervo_maestro->visiones_del_pasado=malloc(0);
	cuervo_maestro->visiones_del_futuro=malloc(0);
	if ( (cuervo_maestro->visiones_del_futuro==NULL) || (cuervo_maestro->visiones_del_pasado==NULL) )
		return NULL;
	return cuervo_maestro; //Si calloc falla, cuervo_maestro sera NULL
}

/*
**Descripcion en el .h
*/
cuervo_aprendiz_t* invocar_aprendiz(char* nombre){
	cuervo_aprendiz_t* cuervo_aprendiz=calloc(1,sizeof(cuervo_aprendiz_t));
	strcpy(cuervo_aprendiz->nombre,nombre);
	cuervo_aprendiz->visiones_adquiridas=malloc(0);
	if (cuervo_aprendiz->visiones_adquiridas==NULL)
		return NULL;
	return cuervo_aprendiz;
}

/*
** PRE: Recibe un vision_t** junto con el tope y una vision para agregar.
** POST: Agrega la vision al vector de visiones, modifica el tope y devuelve 0 en caso de exito, -1 en caso de error.
*/
int incorporar_vision(vision_t** visiones,int* tope_visiones, vision_t vision){
	vision_t* puntero_aux=*(visiones);
	*(visiones)=realloc(*(visiones), sizeof(vision_t) * (size_t)((*tope_visiones)+1) ); 
	if(*visiones==NULL){
		*(visiones)=puntero_aux;
		return EXIT_ERROR;
	}
	*(*visiones +(*tope_visiones))=vision;
	(*tope_visiones)++;
	return SUCCESS;
}

/*
**Descripcion en el .h
*/
int agregar_vision(cuervo_tres_ojos_t* cuervo, vision_t vision){
	if(vision.epoca==PASADO)
	return incorporar_vision(&(cuervo->visiones_del_pasado),&(cuervo->cantidad_visiones_pasado),vision);
	else return incorporar_vision(&(cuervo->visiones_del_futuro),&(cuervo->cantidad_visiones_futuro),vision);	
}

/* 
** PRE:Recibe una vision para comparar y un vector de visiones junto con su tope.
** POST:Compara la vision recibida con todas las visiones dentro del vector y devuelve true si hay una igualdad.
*/
bool es_repetida(vision_t vision1, vision_t* visiones_aprendiz,int tope_visiones_aprendiz){
	bool es_repetida=false;
	for(int i=0;i<tope_visiones_aprendiz;i++){
		if( (strcmp(vision1.protagonista,visiones_aprendiz[i].protagonista)==0) && (strcmp(vision1.casa_protagonista,visiones_aprendiz[i].casa_protagonista)==0) && (strcmp(vision1.descripcion,visiones_aprendiz[i].descripcion)==0) && (vision1.epoca==visiones_aprendiz[i].epoca) )
			es_repetida=true;
	}
	return es_repetida;
}

/*
** PRE: Recibe un vector de visiones del cuervo maestro junto con su tope, un vision_t** con las visiones del aprendiz junto con su tope, y una funcion de comparacion.
** POST: Chequea si las visiones del maestro son transmisibles, en caso de serlas las pasa al vector del aprendiz y modifica su tope. Devuelve 0 en caso de exito, -1 en caso de error.
*/
int transmitir(vision_t* visiones_maestro,int tope_visiones_maestro, vision_t** visiones_aprendiz, int* tope_visiones_aprendiz,bool (*es_transmisible)(vision_t)){
	vision_t* puntero_aux;
	int i=0; 
	bool hay_memoria=true;
	while ( (i<tope_visiones_maestro) && (hay_memoria) ){
		if ( ( es_transmisible(visiones_maestro[i]) ) && (!es_repetida( visiones_maestro[i],*visiones_aprendiz,(*tope_visiones_aprendiz) ) )) {
			puntero_aux=(*visiones_aprendiz); 
			*(visiones_aprendiz)=realloc(*(visiones_aprendiz), sizeof(vision_t) * (size_t)((*tope_visiones_aprendiz)+1) );
			if((*visiones_aprendiz)!=NULL){
				*(*visiones_aprendiz+(*tope_visiones_aprendiz))=visiones_maestro[i];
				(*tope_visiones_aprendiz)++;
			}
			else{
				hay_memoria=false;
				*visiones_aprendiz=puntero_aux;
			}
		}
		i++;
	}
	if(hay_memoria)
		return SUCCESS;
	else return EXIT_ERROR;
}

/*
**Descripcion en el .h
*/
int transmitir_visiones(cuervo_tres_ojos_t* cuervo, cuervo_aprendiz_t* aprendiz, bool (*es_transmisible)(vision_t)){
	int pasado,futuro;
	pasado=transmitir(cuervo->visiones_del_pasado,cuervo->cantidad_visiones_pasado,&(aprendiz->visiones_adquiridas),&(aprendiz->cantidad_visiones),es_transmisible);
	futuro=transmitir(cuervo->visiones_del_futuro,cuervo->cantidad_visiones_futuro,&(aprendiz->visiones_adquiridas),&(aprendiz->cantidad_visiones),es_transmisible);
	if( pasado==EXIT_ERROR || futuro==EXIT_ERROR)
		return EXIT_ERROR;
	return SUCCESS;
}

/*
**Imprime por pantalla la vision.
*/
void listar(vision_t vision){
	printf("En el %s, %s de la casa %s: %s\n",vision.epoca==FUTURO?"futuro":"pasado",vision.protagonista,vision.casa_protagonista,vision.descripcion );
}

/*
**Descripcion en el .h
*/
void listar_visiones_transmitidas(cuervo_aprendiz_t aprendiz, bool (*es_listable)(vision_t)){
	for(int i=0;i<aprendiz.cantidad_visiones;i++)
		if( es_listable(aprendiz.visiones_adquiridas[i]))
			listar(aprendiz.visiones_adquiridas[i]);
}

/*
**Descripcion en el .h
*/
int destruir_cuervo(cuervo_tres_ojos_t* cuervo){
	if (cuervo==NULL){
		printf("No se pudo destruir el cuervo.\n");
		return EXIT_ERROR;
	}
	else{
		free(cuervo->visiones_del_futuro);
		free(cuervo->visiones_del_pasado);
		free(cuervo);
		return SUCCESS;
	}
}

/*
**Descripcion en el .h
*/
int destruir_aprendiz(cuervo_aprendiz_t* aprendiz){
	if (aprendiz==NULL){
		printf("No se pudo destruir al aprendiz.\n");
		return EXIT_ERROR;
	}
	else{
		free(aprendiz->visiones_adquiridas);
		free(aprendiz);
		return SUCCESS;
	}
}
