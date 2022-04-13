El siguiente archivo zip corresponde a la entrega del tipo de dato abstracto "pila" (enunciado adjunto en el archivo zip).

La linea de compilacion es: gcc *.c -o  pila_vd -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
Dicha linea deberá ser ejecutada en la terminal.

El archivo comprimido cuenta ademas con 3 archivos:
-pila.h
-pila.c
-minipruebas_pila_vd.c

El archivo .h cuenta con las firmas de las funciones y sus descripciones.


El archivo .c cuenta con las implementaciones de las funciones.
Sobre las implementaciones caben destacar las siguientes cuestiones: 
-Se decidio inicializar el tamaño de la pila en 10 para facilitar las pruebas al redimensionar la pila.
-El criterio utilizado para agrandar el tamaño de la pila es duplicar el tamaño una vez llenada la pila.
-El criterio utilizado para achicarla es: una vez que la cantidad de elementos alcanza el 25% del tamaño de la pila, se achica la pila a la mitad (para que el tamaño siga siendo el doble de la cantidad de elementos). De todas formas la pila no puede tener un tamaño menor a su tamaño inicial de 10.


El archivo de pruebas cuenta con las pruebas corridas para asegurar el funcionamiento del codigo, corroborando que el tamaño y los campos de la pila cambia acorde a lo esperado cuando se apilan y desapilan elementos.
Las pruebas estan ordenadas de forma que:
-Primero se hacen pruebas con una pila que falló al crearse
-Luego se hacen pruebas con la pila vacia
-Luego se apilan elementos para chequear el agrandamiento de la pila
-Por ultimo se desapilan más elementos de los que hay en la pila para asegurarse que el tope no baja de 0 y que los elementos estaban ordenados correctamente.

En caso de que se quieran agregar pruebas, deberan ser agregadas a cualquiera de las partes o, si se cree necesario, agregar una nueva parte que no haya sido tenido en cuenta a la hora de correr las pruebas.

