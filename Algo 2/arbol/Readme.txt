Se entregan los archivos adjuntos: 
-abb.c 
-abb.h
-minipruebas_abb.c

Correspondientes a la entrega del TDA arbol binario de busqueda para la catedra de Mariano Mendez de Algoritmos y programacion 2.

La linea de compilacion es:
gcc *.c -o abb -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0

La linea de ejecucion con valgrind es:
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./abb

COMENTARIOS:
-El criterio al insertar un elemento repetido, es ponerlo a la derecha del elemento al que equivale.
-Al eliminar un nodo con dos hijos, se procede a reemplazarlo con el nodo "mayor de los menores"

Las pruebas fueron ejecutadas tomando en cuenta casos borde como que no exista la estructura arbol, los parametros pasados sean incorrectos y/o NULL, que la estructura este vacia, y demas casos aclarados en el archivo minipruebas_abb.c