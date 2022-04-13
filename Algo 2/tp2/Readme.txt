Se entrega el TP2 correspondiente a la catedra Mendez de Algoritmos y programacion II.
Se entregan los siguientes archivos:
-simulacion.c
-lista.h (correspondiente al TDA lista)
-lista.c
-abb.h (correspondiente al TDA  arbol)
-abb.c
-cola.h (correspondiente al TDA cola)
-cola.c
-casas.txt
-Ayuda.txt (archivo del cual se lee para imprimir las caracteristicas de cada comando)
-El enunciado del tp en PDF

Linea de compilacion:
gcc *.c -o simulacion -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0

Linea de ejecucion con valgrind
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./simulacion

ACLARACIONES:
-En caso de que dos casas tengan la misma cantidad de integrantes, la que quedara en el trono de hierro sera la que venga primero en orden alfabetico.
-Al pedir los años, el programa impide ingresar numeros negativos, sin embargo no impide ingresar letras, en cuyo caso (probablemente) la cantidad de años sea cero, esto depende del comportamiento de la funcion atoi.

Cuidado al ingresar una gran cantidad de integrantes y/o casas, el programa empieza a funcionar lento y puede que salga un mensaje de erro de valgrind como el siguiente. El cual se da por problemas en el stack.

Warning: client switching stacks?  SP change: 0x1ffefffe00 --> 0x1ffee00770
==30357==          to suppress, use: --max-stackframe=2094736 or greater
==30357== Invalid write of size 8
==30357==    at 0x10A1E1: actualizar_edades (simulacion.c:279)
==30357==    by 0x10A2CB: envejecer_integrantes (simulacion.c:300)
==30357==    by 0x10A706: simular (simulacion.c:375)
==30357==    by 0x10A755: correr_simulacion (simulacion.c:392)
==30357==    by 0x10A847: main (simulacion.c:423)
==30357==  Address 0x1ffee00768 is on thread 1's stack
==30357==  in frame #0, created by actualizar_edades (simulacion.c:269)
