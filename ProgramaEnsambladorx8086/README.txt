El Programa CESAR.COM instala en la interrupción 55h una rutina de 
codificación cesar, se puede usar este mismo .COM para desinstalar la rutina
el programa COMPROB.EXE ejecuta la interrupción 55h, para comprobar que 
nuestra rutina está instalada y funciona.

Para que la interrupción 55h codifique, se debe de inicializar AH=11h en DX
el offset del mensaje a codificar y en AL el número de posiciones que se 
rota.

El makefile funciona con turboasembler
