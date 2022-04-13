#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "abb.h"
#include "cola.h"
#include "lista.h"
#define AYUDA "Ayuda.txt"
#define ERROR -1
#define EXITO 0
#define IGUALES 0
#define MENOR -1
#define MAYOR 1
#define SIMULAR 'S'
#define AGREGAR_CASA 'A'
#define LISTAR_CASAS 'L' 
#define MOSTRAR_CASAS_EXTINTAS 'E'
#define FINALIZAR_EJECUCION 'Q'
#define CANTIDAD_NOMBRES_ALEATORIOS 8
#define NOMBRE1 "Jon "
#define NOMBRE2 "Daenerys "
#define NOMBRE3 "Sansa "
#define NOMBRE4 "Arya "
#define NOMBRE5 "Bran "
#define NOMBRE6 "Tyrion "
#define NOMBRE7 "Rickon "
#define NOMBRE8 "Varys "
#define INPUT_USUARIO 1000
#define NOMBRE_ARCHIVO 200
#define NOMBRE_CASA 200
#define NOMBRE_INTEGRANTE 200
#define ARGUMENTOS_CASAS 3
#define ARGUMENTOS_PERSONAS 2
#define ARGUMENTOS_TIPOS 1
#define INTEGRANTE 'I'
#define CASA 'C'
#define EDAD_MUERTE 60

typedef struct westeros{
	abb_t* arbol_casas;
	cola_t* casas_extintas;
	int cantidad_casas;
}westeros_t;

typedef struct persona{
	char nombre[NOMBRE_INTEGRANTE];
	int edad; 
}persona_t;

typedef struct casa{
	char nombre_casa[NOMBRE_CASA];
	int factor_envejecimiento;
	int factor_nacimiento;
	lista_t* integrantes;
}casa_t;

/*
**Funcion para la creacion del ABB.
**Compara 2 casas.
*/
int comparador_de_casas(void* casa1, void* casa2){
		if(strcmp(((casa_t*)casa1)->nombre_casa,((casa_t*)casa2)->nombre_casa)<0)
			return MENOR;
		if(strcmp(((casa_t*)casa1)->nombre_casa,((casa_t*)casa2)->nombre_casa)>0)
			return MAYOR;
		else return IGUALES;
}

/*
**Destructor de un integrante.
*/
void destruir_integrante(void* integrante){
	free((persona_t*)integrante);
}

/*
**Funcion para la creacion del ABB.
**Destruye las casas.
*/
void destructor_de_casas(void* casa){
	if(casa){
		lista_con_cada_elemento( ((casa_t*)casa)->integrantes,destruir_integrante );
		lista_destruir( ((casa_t*)casa)->integrantes);
		free((casa_t*)casa);
	}
}

/*
**Funcion para la entrada de datos del usuario.
*/
char* leer_linea(FILE* archivo){
  static char linea[INPUT_USUARIO];
  
  char* string = fgets(linea, INPUT_USUARIO, archivo);
  if(string)
	  linea[strlen(linea)-1] = 0;
  return linea;
}

/*
**Funcion que pide un comando al usuario y repite el proceso hasta que el comando sea válido.
**La funcion UNICAMENTE acepta los comandos válidos, sin letras de más y sin minúsculas.
*/
char menu_comandos(){
	printf("Por favor ingrese el comando a ejecutar: ");
	char* input=leer_linea(stdin);
	char comando= input[0]; //ignoramos todo caracter ingresado despues del primero.
	while( (strlen(input)!=1) || ((comando != SIMULAR) && (comando != AGREGAR_CASA) && (comando != LISTAR_CASAS) && (comando != MOSTRAR_CASAS_EXTINTAS) && (comando != FINALIZAR_EJECUCION)) ){
		printf("Comando invalido, por favor ingrese el comando de nuevo: ");
		input=leer_linea(stdin);
		comando=input[0];
	}
	return comando;
}

/*
**Funcion que pide  el nombre del archivo al usuario.
*/
void pedir_nombre_archivo(char nombre_archivo[NOMBRE_ARCHIVO]){
	printf("Introduzca el nombre del archivo de casas: ");
	strcpy(nombre_archivo,leer_linea(stdin));
}

/*
**Funcion que pide los años a ser simulados.
**No acepta numeros negativos. Al ingresarse letras, el comportamiento depende de la funcion atoi (probablemente sea cero).
*/
int pedir_anios(){
	printf("Por favor ingrese los años a ser simulados:");
	int anios_ejecucion;
	char* input=leer_linea(stdin);
	anios_ejecucion=atoi(input);
	while(anios_ejecucion < 0){
		printf("Por favor ingrese un numero valido (positivo):");
		input=leer_linea(stdin);
		anios_ejecucion=atoi(input);
	}
	return anios_ejecucion;
}

/*
**Funcion auxiliar de actualizar_casa.Recibe una casa a actualizar y otra casa con los integrantes nuevos.
**Inserta los integrantes nuevos en la casa ya existente.
*/
void actualizar_integrantes(casa_t* casa, casa_t* casa_a_actualizar){
	lista_iterador_t* iterador=lista_iterador_crear(casa->integrantes);

	while(lista_iterador_tiene_siguiente(iterador)){
		persona_t* integrante=(persona_t*)lista_iterador_siguiente(iterador);
		lista_insertar(casa_a_actualizar->integrantes,integrante);
	}

	lista_iterador_destruir(iterador);

}

/*
**Funcion auxiliar de llenar arbol. Se llama cuando la casa ingresada por el usuario ya existe.
**Actualiza la casa existente y destruye la estructuras dinamicas creadas que ya no son necesarias.
*/
void actualizar_casa(abb_t* arbol_casas,casa_t* casa,casa_t* casa_a_actualizar){
	casa_a_actualizar=(casa_t*)arbol_buscar(arbol_casas,casa);
	actualizar_integrantes(casa,casa_a_actualizar); 
	lista_destruir(casa->integrantes);
	free(casa); 
}

/*
**Funcion auxiliar de agregar_casa. Lee del archivo de casas ingresado por el usuario.
**Crea la estructuras de casas y personas, y los inserta en el ABB correspondiente.
*/
int llenar_arbol(abb_t* arbol_casas, FILE* archivo_casas, int* cantidad_casas){
	int casas_leidas, personas_leidas, tipos_leidos;
	casa_t *casa=NULL, *casa_a_actualizar=NULL;
	persona_t* integrante;
	char tipo; // C si es casa, I si es integrante.
	bool hubo_error=false,actualizar_lista_integrantes=false;

	tipos_leidos=fscanf(archivo_casas,"%c;",&tipo);
	if(tipo!=CASA)
		hubo_error=true;
	while(tipos_leidos==ARGUMENTOS_TIPOS && !feof(archivo_casas) && !hubo_error){
		if(tipo==CASA){
			if(actualizar_lista_integrantes==true){ //este caso solo se va a dar cuando la casa ya este en el arbol
				actualizar_casa(arbol_casas,casa,casa_a_actualizar);
				actualizar_lista_integrantes=false; //reseteo la variable
			}
			casa=malloc(sizeof(casa_t));
			if(casa != NULL){
				casas_leidas=fscanf(archivo_casas,"%[^;];%i;%i\n",casa->nombre_casa,&(casa->factor_envejecimiento),&(casa->factor_nacimiento));
				if(casas_leidas==ARGUMENTOS_CASAS){	
					casa->integrantes=lista_crear();
					if(casa->integrantes == NULL)
						hubo_error=true;
					if(arbol_buscar(arbol_casas,casa)==NULL){ //si es null, la casa no existe entonces inserto
						arbol_insertar(arbol_casas,casa);
						(*cantidad_casas)++;
					}
					else actualizar_lista_integrantes=true;
				}
				else hubo_error=true; //fallo al leer el archivo
			}
			else hubo_error=true; //fallo el malloc
			tipos_leidos=fscanf(archivo_casas,"%c;",&tipo);
		}
		if(tipo==INTEGRANTE){
			while(tipo==INTEGRANTE && !feof(archivo_casas)){
				integrante=malloc(sizeof(persona_t));
				if(integrante != NULL){
					personas_leidas=fscanf(archivo_casas,"%[^;];%i\n",integrante->nombre,&(integrante->edad));
					if(personas_leidas == ARGUMENTOS_PERSONAS)	
						lista_insertar(casa->integrantes,integrante);
					else hubo_error=true; //fallo al leer el archivo
				}
				else hubo_error=true; //fallo el malloc
				tipos_leidos=fscanf(archivo_casas,"%c;",&tipo);
			}
		}
	}
	if(actualizar_lista_integrantes==true){ 
		actualizar_casa(arbol_casas,casa,casa_a_actualizar);
		actualizar_lista_integrantes=false; 
	}
		
	if(hubo_error){
		printf("Error al ingresar las casas.\n");
		return ERROR;
	}
	else return EXITO;
}

/*
**Funcion ejecutada con el comando AGREGAR_CASA.
**Pide el nombre del archivo al usuario y llena el ABB con los datos correspondientes. Le avisa al usuario en caso de error.
*/
int agregar_casa(westeros_t* westeros){
	char nombre_archivo[NOMBRE_ARCHIVO];
	pedir_nombre_archivo(nombre_archivo);
	FILE* archivo_casas=fopen(nombre_archivo,"r");
	while(!archivo_casas){
		printf("Error al abrir el archivo. Por favor chequear que el archivo exista.\n");
		return EXITO;
	}
	int estado_ejecucion=llenar_arbol(westeros->arbol_casas,archivo_casas,&(westeros->cantidad_casas));
	fclose(archivo_casas);
	if(estado_ejecucion==EXITO){
		printf("Casas ingresadas correctamente.\n");
	}
	return estado_ejecucion;
}

/*
**Ordena las casas de mayor a menor en el vector de casas.
*/
void ordenamiento_burbujeo(casa_t* casas[],int tope){
	int i,j;
	casa_t* aux;
	for(i=0;i<tope-1;i++)
		for(j=i+1;j<tope;j++)
			if( lista_elementos(casas[i]->integrantes) < lista_elementos(casas[j]->integrantes) ){
				aux=casas[i];
				casas[i]=casas[j];
				casas[j]=aux;
			}
}

/*
**Imprime los integrantes de una casa.
*/
void mostrar_integrantes(casa_t* casa){
	lista_iterador_t* iterador=lista_iterador_crear(casa->integrantes);
	while(lista_iterador_tiene_siguiente(iterador)){
		persona_t* integrante=(persona_t*)lista_iterador_siguiente(iterador);
		printf("Nombre: %s  Edad:%i\n",integrante->nombre,integrante->edad);
	}
	lista_iterador_destruir(iterador);
}

/*
**Imprime las casas por orden de cantidad de integrantes
*/
void imprimir_casas(casa_t* casas[],int tope){
	printf("\nLas casas no extintas por orden de cantidad de integrantes son:\n");
	for(int i=0;i<tope;i++){
		printf("\nCasa %s: %zu integrantes.\n",casas[i]->nombre_casa,lista_elementos(casas[i]->integrantes));
		mostrar_integrantes(casas[i]);
	}
}

/*
**Funcion ejecutada con el comando LISTAR_CASAS
**Imprime las casas vivas por orden de integrantes. Avisa al usuario si no hay casas.
*/
int listar_casas(westeros_t* westeros){
	if(arbol_vacio(westeros->arbol_casas)){
		printf("No hay casas.\n");
		return EXITO;
	}
	casa_t* casas[westeros->cantidad_casas];
	arbol_recorrido_inorden(westeros->arbol_casas,(void**)casas,westeros->cantidad_casas); //el recorrido es inorden asi que estaran por orden alfabetico
	ordenamiento_burbujeo(casas,westeros->cantidad_casas);
	imprimir_casas(casas,westeros->cantidad_casas); 

	return EXITO;
}

/*
**Funcion ejecutada con el comando MOSTRAR_CASAS_EXTINTAS.
**Imprime las casas extintas desde la cola de casas extintas.
*/
int mostrar_casas_extintas(westeros_t* westeros){
	if(cola_vacia(westeros->casas_extintas)){
		printf("No hay casas extintas.\n");
		return EXITO;
	}

	casa_t* casa;
	int estado_ejecucion=EXITO;
	int numero_iteraciones=cola_cantidad(westeros->casas_extintas);

	printf("Las casas extintas son:\n");
	for(int i=0;i<numero_iteraciones;i++){ //va a recorrer toda la cola
		casa=(casa_t*)cola_primero(westeros->casas_extintas);
		printf("%s\n",casa->nombre_casa);
		estado_ejecucion=cola_desencolar(westeros->casas_extintas);
		estado_ejecucion=cola_encolar(westeros->casas_extintas,casa); //encolo y desencolo para imprimir todas las casas y que la cola quede igual.
	}
	return estado_ejecucion;
}

/*
**Procedimiento que se encarga de borrar a los integrantes mayores de edad de la lista de 
**integrantes de una casa. Es un procedimiento auxiliar de actualizar_edades.
*/
void eliminar_fallecidos(lista_t* integrantes, int posiciones_a_eliminar[], int tope){
	persona_t* integrante;
	for(int i=tope-1;i>=0;i--){ //lo hace de atras para adelante asi no hay problemas con la reasignacion de posiciones.
		integrante=(lista_elemento_en_posicion(integrantes,(size_t)posiciones_a_eliminar[i]));
		destruir_integrante(integrante);
		lista_borrar_de_posicion(integrantes,(size_t)posiciones_a_eliminar[i]);
	}
}

/*
**Funcion auxiliar de envejecer_integrantes. Itera por la lista de integrantes
**y actualiza sus edades. Si superan la edad de muerte, ingresa su posicion de la lista en un vector de posiciones
**que mas tarde se le mandará a la funcion eliminar_fallecidos.
*/
int actualizar_edades(casa_t* casa){
	lista_iterador_t* iterador=lista_iterador_crear(casa->integrantes);
	if(!iterador || casa->factor_envejecimiento<0 )
		return ERROR;

	persona_t* integrante;
	size_t cantidad_integrantes=lista_elementos(casa->integrantes);
	int posiciones_a_eliminar[cantidad_integrantes],tope=0;
	int posicion_actual=0;

	while(lista_iterador_tiene_siguiente(iterador)){
		integrante=(persona_t*)lista_iterador_siguiente(iterador);
		integrante->edad += casa->factor_envejecimiento;
		if(integrante->edad >= EDAD_MUERTE){
			posiciones_a_eliminar[tope]=posicion_actual;
			tope++;
		}
		posicion_actual++;
	}
	lista_iterador_destruir(iterador);
	eliminar_fallecidos(casa->integrantes,posiciones_a_eliminar,tope);
	return EXITO; 
		
}

/*
**Funcion auxiliar de simular. Aumenta las edades de los integrantes de cada casa segun su factor de envejecimiento.
**Si una casa se queda sin integrantes, se manda a la cola de casas extintas.
*/
int envejecer_integrantes(abb_t* arbol_casas, cola_t* casas_extintas, int* cantidad_casas){
	int estado_ejecucion=EXITO,i,cantidad_casas_a_eliminar=0;
	casa_t* casas[(*cantidad_casas)];
	if(*cantidad_casas != arbol_recorrido_inorden(arbol_casas,(void**)casas,*cantidad_casas))
		estado_ejecucion=ERROR;
	for(i=0;i<(*cantidad_casas);i++)
		estado_ejecucion=actualizar_edades(casas[i]);
	i=0;
	while(estado_ejecucion==EXITO && i<(*cantidad_casas)){
		if(lista_elementos(casas[i]->integrantes)==0){
			casa_t* casa=calloc(1,sizeof(casa_t));
			if(casa){
				strcpy(casa->nombre_casa,casas[i]->nombre_casa);
				casa->factor_nacimiento=casas[i]->factor_nacimiento;
				casa->factor_envejecimiento=casas[i]->factor_envejecimiento;
			}
			estado_ejecucion=arbol_borrar(arbol_casas,casas[i]);
			cantidad_casas_a_eliminar++;
			estado_ejecucion=cola_encolar(casas_extintas,casa);
		}
		i++;
	}
	(*cantidad_casas) -= cantidad_casas_a_eliminar;
	return estado_ejecucion;
}

/*
**Funcion auxiliar de agregar_integrantes.
**Genera un nombre aleatorio para los nuevos nacidos.
*/
void generar_nombre(casa_t* casa,char nombre_integrante[NOMBRE_INTEGRANTE]){
	char nombre_aleatorio[CANTIDAD_NOMBRES_ALEATORIOS][NOMBRE_INTEGRANTE]={NOMBRE1,NOMBRE2,NOMBRE3,NOMBRE4,NOMBRE5,NOMBRE6,NOMBRE7,NOMBRE8};
	int posicion_aleatoria=rand()%CANTIDAD_NOMBRES_ALEATORIOS;
	strcpy(nombre_integrante,strcat(nombre_aleatorio[posicion_aleatoria],casa->nombre_casa));  //concatena nombre y apellido(nombre de la casa) y luego lo copia en el nombre del integrante.
}

/*
**Funcion auxiliar de nacimiento_integrantes. Crea la cantidad de integrantes nuevos segun el factor de nacimiento.
*/
int agregar_integrantes(casa_t* casa){
	int estado_ejecucion=EXITO;
	if ( ((casa_t*)casa)->factor_nacimiento<=0 )
		return EXITO; //no se hace nada
	size_t cantidad_a_nacer= ( lista_elementos(((casa_t*)casa)->integrantes) / (size_t) ((casa_t*)casa)->factor_nacimiento );
	persona_t* integrante;
	for(int i=0;i<cantidad_a_nacer;i++){
		integrante=malloc(sizeof(persona_t));
		if(!integrante)
			estado_ejecucion=ERROR;
		generar_nombre(casa,integrante->nombre); //inicializa nombre del integrante
		integrante->edad=0; //porque recien nacio, no inicializo el nombre porque no importa.
		if(lista_insertar(((casa_t*)casa)->integrantes,integrante)==ERROR)
			estado_ejecucion=ERROR;
	}
	return estado_ejecucion;
}

/*
**Funcion auxiliar de simular. Genera un vector con las casas y le aplica a cada casa la 
**funcion agregar_integrantes
*/
int nacimiento_integrantes(abb_t* arbol_casas, int* cantidad_casas){
	int estado_ejecucion=EXITO;
	if(arbol_vacio(arbol_casas))
		return estado_ejecucion; //la funcion imprimir ganadora avisara que el arbol esta vacio
	casa_t* casas[(*cantidad_casas)];
	arbol_recorrido_inorden(arbol_casas,(void**)casas,*cantidad_casas);
	for(int i=0;i<(*cantidad_casas);i++)
		estado_ejecucion=agregar_integrantes(casas[i]);
	return estado_ejecucion;
}

/*
**Funcion auxiliar de simular. Genera un vector de casas y lo recorre una vez chequeando cual tiene mas integrantes.
**Devuelve la casa con mas integrantes. Avisa si no hay ninguna casa sobre el trono de hierro.
*/
void imprimir_ganadora(abb_t* arbol_casas,int* cantidad_casas){
	if(arbol_vacio(arbol_casas)){
		printf("No hay ninguna casa sobre el trono de hierro.\n");
		return;
	}
	casa_t* casas[(*cantidad_casas)];
	arbol_recorrido_inorden(arbol_casas,(void**)casas,(*cantidad_casas)); //llena el array.
	size_t mayor_cantidad_integrantes=0, posicion_ganadora=0;
	for(size_t i=0;i<(*cantidad_casas);i++){
		if(lista_elementos(casas[i]->integrantes)>mayor_cantidad_integrantes){
			mayor_cantidad_integrantes=lista_elementos(casas[i]->integrantes);
			posicion_ganadora=i;
		}
	}
	printf("La casa que ocupa el trono de hierro es la casa %s.\n",((casa_t*)casas[posicion_ganadora])->nombre_casa);
}

/*
**Simula los años ingresados por el usuario, iterando cada año envejeciendo y naciendo los integrantes.
**Imprime que casa quedo en el trono de hierro.
*/
int simular( abb_t* arbol_casas, cola_t* casas_extintas,int anios_ejecucion,int* cantidad_casas){
	int i=0;
	int estado_ejecucion=EXITO;
	while(i<anios_ejecucion && estado_ejecucion==EXITO){
		estado_ejecucion=envejecer_integrantes(arbol_casas,casas_extintas,cantidad_casas);
		if(estado_ejecucion==EXITO)
			estado_ejecucion=nacimiento_integrantes(arbol_casas,cantidad_casas);
		i++;
	}
	if(estado_ejecucion==EXITO)
		imprimir_ganadora(arbol_casas,cantidad_casas);
	return estado_ejecucion;
}

/*
**Funcion ejecutada por el comando CORRER_SIMULACION.
**Pide los años a simular al usuario y los simula.
*/
int correr_simulacion(westeros_t* westeros){
	if(arbol_vacio(westeros->arbol_casas)){
		printf("No hay casas ingresadas.\n");
		return EXITO;
	}
	int anios_ejecucion,estado_ejecucion; 
	anios_ejecucion=pedir_anios();
	estado_ejecucion=simular(westeros->arbol_casas,westeros->casas_extintas,anios_ejecucion,&(westeros->cantidad_casas));
	return estado_ejecucion;
}

/*
**Funcion destructora para liberar la memoria de las casas en la cola de casas extintas.
*/
void destruir_casas_extintas(cola_t* casas_extintas, void(*destructor)(void*)){
	int iteraciones=cola_cantidad(casas_extintas);
	for(int i=0;i<iteraciones;i++){
		destructor(cola_primero(casas_extintas));
		cola_desencolar(casas_extintas);
	}
}

void mostrar_ayuda(){
	FILE* ayuda=fopen(AYUDA,"r");
	int leidos;
	if(!ayuda)
		return;
	char string[INPUT_USUARIO];
	leidos=fscanf(ayuda,"%[^%]",string);
	while(leidos==1){
		printf("%s\n",string);
		leidos=fscanf(ayuda,"%[^%]",string);
	}
	fclose(ayuda);
}

int main(){
	char comando;
	int estado_ejecucion=EXITO;
	westeros_t* westeros=malloc(sizeof(westeros_t));
	if(!westeros)
		return ERROR;
	westeros->cantidad_casas=0;
	westeros->arbol_casas=arbol_crear(comparador_de_casas,destructor_de_casas);
	westeros->casas_extintas=cola_crear();
	srand((unsigned int)time(NULL)); //Para el generador de nombres aleatorios.

	if(!westeros->arbol_casas || !westeros->casas_extintas){
		arbol_destruir(westeros->arbol_casas);
		cola_destruir(westeros->casas_extintas);
		return ERROR;
	}

	mostrar_ayuda();
	comando=menu_comandos();
	while(comando != FINALIZAR_EJECUCION && estado_ejecucion==EXITO){
		if(comando == SIMULAR)
			estado_ejecucion=correr_simulacion(westeros);
		if(comando == AGREGAR_CASA)
			estado_ejecucion=agregar_casa(westeros);
		if(comando == LISTAR_CASAS)
			estado_ejecucion=listar_casas(westeros);
		if(comando == MOSTRAR_CASAS_EXTINTAS)
			estado_ejecucion=mostrar_casas_extintas(westeros);
		if(estado_ejecucion != ERROR)
			comando=menu_comandos();
	}
	arbol_destruir(westeros->arbol_casas);
	destruir_casas_extintas(westeros->casas_extintas, destructor_de_casas);
	cola_destruir(westeros->casas_extintas);
	free(westeros);
	if(estado_ejecucion == ERROR){
		printf("Ha ocurrido un error. Terminando simulación.\n");
		return ERROR;
	}
	return 0;
}

