Se entrega junto con este archivo los siguientes:
-lista.c
-lista.h
-minipruebas_lista_se.c
Correspondientes al TDA lista pedido por la catedra.

La linea de compilacion es:
gcc *.c -o lista_se -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0

Para correr el codigo con valgrind la linea es:
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./ lista_se

Las anteriores deben ser ejecutadas en la terminal.

Comentarios a hacer:
La lista fue implementada con nodos enlazados, poniendo un puntero al ultimo elemento y al primero (el puntero al ultimo es para hacer que insertar un elemento al final tenga una complejidad de O(1) ). La lista cuenta con un iterador interno y un externo como detallan las descripciones en el .h .
El set de pruebas corridas detallan qué casos se estan tomando en cuenta en cada parte (ej: primero se corren las pruebas si la lista no existe, luego si la lista esta vacia, etc.).