from sys import argv, exit
from PyPDF2 import PdfMerger
import os
'''
Script de python que une dos PDFs pasados por terminal
'''

def unidor():

    try:
        #objeto de tipo PdfMerger
        mezclador = PdfMerger()
    except:
        print("Error al crear el mezclador")
        exit()

    #array con las direcciones o nombres de los archivos
    archivos = argv[1:len(argv)]

    #array de palabras del primer argumento
    palabras = archivos[0].split('/')
    #nombre final del archivo
    nombre = palabras[len(palabras)-1]

    #bucle donde se comprueba que existen los archivos, si es así se añaden al mezclador
    for paths in archivos:
        if(os.path.exists(paths) == False):
            print("Uno o mas argumentos pasados, es un archivo que no existe en el directorio actual"
            + "o es un dirección completa que no existe.")
            mezclador.close()
            exit()
        else:
            mezclador.append(paths)

    try:    
        #generador del archivo 
        mezclador.write(f'{os.getcwd()}/{nombre}_mix.pdf')
        #Se descarga en la carpeta que contiene el script
    except:
        print("Error al intentar crear el archivo")
        exit()
        
    mezclador.close()
    print("PDF unido y guardado en el directorio actual")
    exit() 



if __name__:
    unidor()
