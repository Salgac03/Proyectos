import os
import shutil

#Ruta del directorio a ordenar
path = '/Your/path'

#directorios en base al tipo de dato. Preferible crearlos en la ruta path antes de ejecutar
pdfs = path + '/PDFs'
imagenes = path + '/Imagenes'
comprim = path + '/Comprimidos'
sonidos = path + '/MusicaySonidos'
videos = path + '/PeliculasyVideos'
txt = path + '/PuntoTxt'

#array de extensiones para ciertos tipos de datos
imagenesext = ['.jpg','.jpge','.png','.xcf']
sonidosext = ['.mp3','.wav']
videosext = ['.mp4','.mov','.avi']
comprimidos = ['.zip','.rar','.tar','.tar.gz','.gz']

#gurada cada archivo de path en su carpeta correspondiente en función al tipo de archivo que sea
for archivo in os.listdir(path):
    (nombre,extension) = os.path.splitext(archivo)
    if(extension == '.pdf'):
        shutil.move(path+f'/{archivo}',pdfs+f'/{archivo}')
    elif(extension in comprimidos):
        shutil.move(path+f'/{archivo}',comprim+f'/{archivo}')
    elif(extension in sonidosext):
         shutil.move(path+f'/{archivo}',sonidos+f'/{archivo}')
    elif(extension in videosext):
         shutil.move(path+f'/{archivo}',videos+f'/{archivo}')
    elif(extension in imagenesext):
        shutil.move(path+f'/{archivo}',imagenes+f'/{archivo}')
    elif extension == '.txt':
        shutil.move(path+f'/{archivo}',txt+f'/{archivo}')

