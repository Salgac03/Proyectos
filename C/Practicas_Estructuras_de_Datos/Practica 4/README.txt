Este archivo tiene como finalidad explicar por qué he añadido otra implementación de bstree.c
(bstreealternativo), todo se debe a la implementación de _bst_remove_rec, pues Eduardo nos dijo
que implementasemos esta función suponiendo que los elementos del árbol se eliminaban por separado
a este. Pero en la clase del día 29 de abril en vez de darla con Eduardo la dimos con otro profesor
que nos dijo que cuando eliminabamos un elemento del árbol tambien lo liberasemos de la memoria.

Como no se cual es la implementación que usted quería que implementase, he implementado ambas,
por lo que la única diferencia entre ambos archivos .c son las funciones _bst_remove_rec y las
funciones de búsqueda de elemento _bst_find_min_rec y _bst_find_max_rec que en la implementación
del fichero bstree.c devuelve el menor y mayor elemento del árbol respectivamente mientras que las
funciones _bst_find_min_rec y _bst_find_max_rec implementadas en bstreealternativo.c devuelven el nodo
que contiene el elemento menor y mayor respectivamente.
