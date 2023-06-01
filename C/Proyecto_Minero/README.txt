Proyecto de la universidad consistente en un  "minero crypto" en lenguaje C.

Debido a las circunstancias en las que fue hecho, carece en gran parte de 
documentación.

El proyecto consistía en realizar un sistema de minado multiproceso y
multihilo con memoria compartida y ficheros. De tal manera que se pudiesen
ejecutar varios porcesos mineros (miner) ya fuese a la vez o de manera indivi-
dual, cada proceso minero, se enfrenta al resto de procesos mineros partici-
pantes en una ronda de minado realizando una prueba de trabajo, cuando un pro-
ceso minero encuentra la solución, se sincroniza con el resto de mineros
mediante el uso de semáforos y señales, quienes deben de validar a través de
una votación si el resultado encontrado es correcto, la información de la
ronda de minado, se encuentra en un segmento de memoria compartida por todos
los mineros, tras la votación, la información de este segmento es actualizada
y cada minero que ha participado en la ronda de minado, escribe la informa-
ción de la ronda de minado en su bloque, el bloque es un fichero .txt que 
tiene por nombre el pid del proceso monitor que lo ha generado. Si un minero
entra al sistema cuando ya hay una ronda de minado activa, espera inactivo 
hasta que comience la siguiente ronda.

El proceso monitor muestra por terminal el resultado de las rondas de minado,   
recibe dicha información a través de una cola de mensajes, si no hay ningún
minero activo espera a que lo haya.

Se puede salir de cualquiera de los procesos pulsando Ctrl+C

Modo de ejecución de un proceso minero:
./miner numero_rondas numero_hilos

Modo de ejecución del monitor:
./monitor
