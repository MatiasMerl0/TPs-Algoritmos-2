Se entregan los siguientes archivos correspodientes al tda cola de la cátedra Mendez de algoritmos II:

-cola.h
-cola.c
-minipruebas_cola.c
-El pdf con el enunciado

La linea de compilacion es:
gcc *.c -o cola_ne -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0

En caso de querer correr el programa con valgrind entonces la linea de compilacion es:
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./cola_ne

Sobre los archivos:
-cola.h cuenta con las declaraciones de las funciones de la cola junto con sus descripciones.

-cola.c cuenta con las implementaciones de dichas funciones, implementando la cola con nodos enlazados.

-minipruebas_cola.c cuenta con las pruebas corridas para verificar el correcto funcionamiento del contrato. Las pruebas analizan los casos donde no existe la cola, cuando la cola esta vacia, cuando se encolan muchos elementos y cuando se desencolan muchos elementos.
