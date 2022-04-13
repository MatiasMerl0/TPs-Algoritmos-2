#include "hash.h"
#include "hash_iterador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERROR -1
#define EXITO 0
// Necesario porque strdup es POSIX pero no C99
extern char* strdup(const char*);

void destruir_string(void* elemento){
  if(elemento){
    printf("(Destructor) Libero el vehiculo: %s\n", (char*)elemento);
    free(elemento);
  }
}

void guardar_vehiculo(hash_t* garage, const char* patente, const char* descripcion){
  int retorno = hash_insertar(garage, patente, strdup(descripcion));
  printf("Guardando vehiculo patente %s (%s): ", patente, descripcion);
  printf("%s\n", retorno==0?"OK":"ERROR");
}

void quitar_vehiculo(hash_t* garage, const char* patente){
  int retorno = hash_quitar(garage, patente);
  printf("Retirando vehiculo patente %s: ", patente);
  printf("%s\n", retorno==0?"OK":"ERROR");
}

void verificar_vehiculo(hash_t* garage, const char* patente, bool deberia_existir){
  printf("Verifico el vehiculo patente %s: ", patente);
  bool retorno = hash_contiene(garage, patente);
  printf("%s\n", (retorno==deberia_existir)?"OK":"ERROR");
}

int main(){

  //Prueba con capacidad = 0
  hash_t* garage=hash_crear(destruir_string,0);
  printf("No se deberia poder crear hash con capacidad igual a cero: %s\n",garage==NULL?"OK":"ERROR" );
  
  //Pruebas con un hash NULL.

  printf("No existe hash, no puede insertar: %s\n",hash_insertar(garage,"clave","elemento")==ERROR?"OK":"ERROR" );
  printf("No existe hash, no puede quitar: %s\n",hash_quitar(garage,"clave")==ERROR?"OK":"ERROR" );
  printf("No existe hash, hash_obtener devuelve NULL: %s\n",hash_obtener(garage,"clave")==NULL?"OK":"ERROR");
  printf("No existe hash, hash_contiene devuelve false: %s\n",hash_contiene(garage,"clave")==false?"OK":"ERROR");
  printf("No existe hash, hash_cantidad devuelve 0: %s\n",hash_cantidad(garage)==0?"OK":"ERROR" );
  printf("No existe hash, no puede crear iterador (devuelve NULL): %s\n",hash_iterador_crear(garage)==NULL?"OK":"ERROR" );

  //Pruebas con un hash creado pero vacio
  garage = hash_crear(destruir_string, 5);

  hash_iterador_t* iter = hash_iterador_crear(garage);
  printf("Creo iterador sin elementos, no deberia tener siguiente: %s\n",hash_iterador_tiene_siguiente(iter)==false?"OK":"ERROR" );
  hash_iterador_destruir(iter);
  printf("Verifico vehiculos que no estan en el garage:\n");
  verificar_vehiculo(garage, "AAA", false);
  verificar_vehiculo(garage, "BBB", false);
  verificar_vehiculo(garage, "CCC", false);
  verificar_vehiculo(garage, "DDD", false);
  printf("Intento quitar vehiculos que no estan en el garage: %s\n",hash_quitar(garage,"AAA")==ERROR?"OK":"ERROR" );
  printf("hash_cantidad devuelve cero: %s\n",hash_cantidad(garage)==0?"OK":"ERROR" );
  printf("hash_obtener devuelve NULL: %s\n",hash_obtener(garage,"AAA")==NULL?"OK":"ERROR" );

  //Agregando vehiculos
  printf("Agrego autos al garage\n");
  guardar_vehiculo(garage,"AAA","Auto 1");
  guardar_vehiculo(garage,"BBB","Auto 2");
  guardar_vehiculo(garage,"CCC","Auto 3");
  guardar_vehiculo(garage,"DDD","Auto 4");
  guardar_vehiculo(garage,"EEE","Auto 5");
  guardar_vehiculo(garage,"FFF","Auto 6");
  guardar_vehiculo(garage,"GGG","Auto 7");
  guardar_vehiculo(garage,"HHH","Auto 8");
  guardar_vehiculo(garage,"III","Auto 9");
  printf("La cantidad de autos deberia ser 9: %s\n",hash_cantidad(garage)==9?"OK":"ERROR" );

  verificar_vehiculo(garage, "AAA", true);
  verificar_vehiculo(garage, "BBB", true);
  verificar_vehiculo(garage, "BBb", false);
  verificar_vehiculo(garage, "III", true);
  verificar_vehiculo(garage, "iII", false);

  quitar_vehiculo(garage, "AAA");
  quitar_vehiculo(garage, "BBB");
  printf("No puedo volver a quitar el mismo vehiculo: %s\n",hash_quitar(garage,"BBB")==ERROR?"OK":"ERROR" );

  verificar_vehiculo(garage, "AAA", false);
  verificar_vehiculo(garage, "BBB", false);
  printf("hash_obtener devuelve NULL cuando le pido un vehiculo borrado: %s\n",hash_obtener(garage,"AAA")==NULL?"OK":"ERROR" );

  printf("La cantidad de autos deberia ser 7: %s\n",hash_cantidad(garage)==7?"OK":"ERROR" );


  printf("La funcion hash_obtener devuelve correctamente lo almacenado: %s\n",strcmp(hash_obtener(garage,"HHH"),"Auto 8")==0?"OK":"ERROR" );
  printf("Agrego muchos autos (Se realiza la operacion de rehash, no debe perder memoria ni generar invalid reads.):\n");
  guardar_vehiculo(garage,"JJJ","Auto 10");
  guardar_vehiculo(garage,"KKK","Auto 11");
  guardar_vehiculo(garage,"LLL","Auto 12");
  guardar_vehiculo(garage,"MMM","Auto 13");
  guardar_vehiculo(garage,"NNN","Auto 14");
  guardar_vehiculo(garage,"OOO","Auto 15");
  guardar_vehiculo(garage,"PPP","Auto 16");
  guardar_vehiculo(garage,"QQQ","Auto 17");
  guardar_vehiculo(garage,"RRR","Auto 18");
  guardar_vehiculo(garage,"SSS","Auto 19");
  guardar_vehiculo(garage,"TTT","Auto 20");
  guardar_vehiculo(garage,"UUU","Auto 21");
  guardar_vehiculo(garage,"VVV","Auto 22");
  guardar_vehiculo(garage,"WWW","Auto 23");
  guardar_vehiculo(garage,"XXX","Auto 24");
  guardar_vehiculo(garage,"YYY","Auto 25");
  guardar_vehiculo(garage,"ZZZ","Auto 26");
  guardar_vehiculo(garage,"AAAA","Auto 27");
  guardar_vehiculo(garage,"BBBB","Auto 28");
  guardar_vehiculo(garage,"CCCC","Auto 29");
  guardar_vehiculo(garage,"DDDD","Auto 30");
  guardar_vehiculo(garage,"EEEE","Auto 31");
  guardar_vehiculo(garage,"FFFF","Auto 32");
  guardar_vehiculo(garage,"GGGG","Auto 33");
  guardar_vehiculo(garage,"HHHH","Auto 34");
  guardar_vehiculo(garage,"IIII","Auto 35");
  guardar_vehiculo(garage,"JJJJ","Auto 36");
  guardar_vehiculo(garage,"KKKK","Auto 37");
  guardar_vehiculo(garage,"LLLL","Auto 38");

  printf("La cantidad de autos deberia ser 36: %s\n",hash_cantidad(garage)==36?"OK":"ERROR" );
  printf("Agrego vehiculos repetidos:\n");
  guardar_vehiculo(garage,"JJJ","Auto 10 modificado");
  guardar_vehiculo(garage,"MMM","Auto 13 modificado");
  guardar_vehiculo(garage,"WWW","Auto 23 modificado");

  //Probando el iterador
  iter = hash_iterador_crear(garage);
  size_t listados = 0;

  while(hash_iterador_tiene_siguiente(iter)){
    const char* clave = hash_iterador_siguiente(iter);
    if(clave){
      listados++;
      printf("Patente: %s -- Vehiculo: %s\n", clave, (char*)hash_obtener(garage, clave));
    }
  }

  printf("Cantidad de autos guardados: %zu. Cantidad de autos listados: %zu -- %s\n", hash_cantidad(garage), listados, (hash_cantidad(garage)==listados)?"OK":"ERROR");

  printf("Destruyo la estructura:\n");
  hash_iterador_destruir(iter);
  hash_destruir(garage);

  return 0;
}
