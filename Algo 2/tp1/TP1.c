#include "cuervo_3_ojos.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define DAENERYS_TARGARYEN "Daenerys Targaryen"
#define JON_SNOW "Jon Snow"
#define STARK "Stark"
#define TARGARYEN "Targaryen"
#define PASADO 'P'
#define FUTURO 'F'
#define BRYNDEN_RIVERS "Brynden Rivers"
#define BRAN "Bran"
#define EXIT_ERROR -1
#define SUCCESS 0
#define CAMPOS_VISION_T 4

/* 
** PRE:Recibe un vision_t
** POST:Devuelve true si la vision es sobre el pasado de Jon Snow.
*/
bool es_jon_pasado(vision_t vision){
	if( (strcmp(vision.protagonista,JON_SNOW)==0) && (vision.epoca==PASADO) )
		return true;
	else return false;
}

/* 
** PRE:Recibe un vision_t
** POST:Devuelve true si la vision es del futuro.
*/
bool es_futuro(vision_t vision){
	if( (vision.epoca==FUTURO) )
		return true;
	else return false;
}

/* 
** PRE:Recibe un vision_t
** POST:Devuelve true si la vision es sobre un Stark.
*/
bool es_stark(vision_t vision){
	if( (strcmp(vision.casa_protagonista,STARK)==0) )
		return true;
	else return false;
}

/* 
** PRE:Recibe un vision_t
** POST:Devuelve true si la vision es del pasado.
*/
bool es_pasado(vision_t vision){
	if(vision.epoca==PASADO)
		return true;
	else return false;
}

/* 
** PRE:Recibe un FILE* correctamente abierto y un vision_t*.
** POST:Lee el archivo y pone los datos en el vision_t.
*/
int leer_vision(FILE* visiones, vision_t* vision){ //Hago una funcion por mas que sea solo una linea porque es mas legible de esta forma.
	return fscanf(visiones,"%[^;];%[^;];%[^;];%c\n",vision->protagonista,vision->casa_protagonista,vision->descripcion,&(vision->epoca));
}

/*
** Mensaje de error que se manda cuando hubo problemas al manejar la memoria dinámica.
*/
void mensaje_error(){
	printf("Hubo un error al reservar la memoria.\n");
}

int main(){
	int estado_de_programa=SUCCESS;
	vision_t vision;
	FILE* visiones=fopen("visiones.txt","r");
	if(!visiones)
		return EXIT_ERROR;

	cuervo_tres_ojos_t* cuervo_tres_ojos=NULL;
	cuervo_aprendiz_t* cuervo_aprendiz=NULL;
	cuervo_tres_ojos=invocar_cuervo(BRYNDEN_RIVERS);
	cuervo_aprendiz=invocar_aprendiz(BRAN);
	if(cuervo_aprendiz==NULL || cuervo_tres_ojos==NULL){
		free(cuervo_aprendiz);
		free(cuervo_tres_ojos); //Si solo uno de los dos fuera NULL, al hacerle free no ocurrira nada (según el manual)
		mensaje_error();
		return EXIT_ERROR;
	}
	while ( (leer_vision(visiones,&vision)==CAMPOS_VISION_T) && (estado_de_programa==SUCCESS) ){
		estado_de_programa=agregar_vision(cuervo_tres_ojos,vision);
	}
	fclose(visiones);
	if(estado_de_programa==SUCCESS){
		transmitir_visiones(cuervo_tres_ojos,cuervo_aprendiz,es_stark);
		listar_visiones_transmitidas(*cuervo_aprendiz,es_pasado);
		listar_visiones_transmitidas(*cuervo_aprendiz,es_jon_pasado); 
		listar_visiones_transmitidas(*cuervo_aprendiz,es_futuro);
	}
	else mensaje_error();
	destruir_aprendiz(cuervo_aprendiz);
	destruir_cuervo(cuervo_tres_ojos);

return 0;
}