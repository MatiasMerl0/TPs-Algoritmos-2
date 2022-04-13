En la carpeta se encuentra la entrega correspondiente al TDA hash de la catedra mendez de algoritmos y programacion 2 de la FIUBA.

Se entrega en la carpeta los siguientes archivos:
-hash.h
-hash.c
-hash_iterador.h
-minipruebas_hash.c
-lista.c
-lista.h
-tda_hash.pdf (enunciado del tda)

El hash implementado es un hash abierto, por lo que se recibe tambien un "TDA lista" -anteriormente implementado para esta misma materia- para ser usado en la implementacion del hash.
Comentarios a realizar sobre la implementacion del hash:
-Se incluyo un campo "claves" a la estructura del hash para facilitar la implementacion del iterador.
-El campo "capacidad" del hash determina cuantas listas distintas se haran.
-Al criterio para realizar el rehash es que la cantidad de elementos superen el triple de la capacidad del hash.
-Al hacer la operacion de rehash, se obtiene el siguiente numero primo y esa es la nueva capacidad del hash.

Linea de compilacion:
gcc *.c -o hash -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0

Linea de ejecucion con valgrind:
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./hash