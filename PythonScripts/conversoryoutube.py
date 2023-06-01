from tkinter import *
from tkinter import filedialog
from tkinter.messagebox import showerror, showinfo
from tkinter.ttk import Combobox
from pytube import YouTube
from pytube import Playlist
import os

#Ruta desde la que se inicia la selección del directorio donde descargar
initialDir = ' /Your/path'
version = "Conversor Youtube a .mp3 V.0.5.1"


'''Función que descarga en un directorio determinado el audio de un vídeo de Youtube'''
def descargarVideo(link : str):
    global txtentry2

    try:
        yt = YouTube(link,use_oauth=False)
    except:
        showerror('Error en el Link 2','Link no válido o inexistente, u opcion incorrecta')

    try:
        new_name = yt.title + '.mp3'
    except:
        new_name = link+'.mp3'
        
        
    audio = yt.streams.get_audio_only()    

    direccion = txtentry2.get()

    if(len(direccion)==0):
        direccion = os.getcwd()

    try:
        audio.download(direccion,filename=new_name)
    except:
        showerror("Error audio 2","Error al intentar descargar el audio")
        return False, ''
    
    return True, direccion


'''Función para descargar como mp3 el contenido del enlace pasado'''
def descargador():
    global txtentry
    global txtentry2
    global comboBox

    try:
        link = txtentry.get()
    except:
        showerror('Error en el Link 1','Link no válido')

    
    try:
        opcion = comboBox.get()
    except:
        showerror('Error en la opcion del Combobox')

    if opcion == "Video":
        ret, dir = descargarVideo(link)

    elif opcion == "Lista":
        try:
            pl = Playlist(link)
        except:
            showerror('Error en el Link 2','Link no válido o inexistente, u opcion incorrecta')

        for video in pl:
            ret, dir = descargarVideo(video)
            if ret == False:
                showerror('Error de Lista','Error en el video con link' + video)
            
            

    if ret:
        showinfo('Descarga Completada',f'Se ha descargado con exito en {dir}')

    
'''Función para seleccionar el directorio donde descargar el archivo de audio'''
def selccionar():
    global txtentry2
    direc = filedialog.askdirectory(initialdir = initialDir)
    if direc != "":
        txtentry2.delete(0,END)
        
    txtentry2.insert(0,direc)

def main():
    global txtentry2
    global txtentry
    global comboBox

    opciones = ['Lista', 'Video']

    #Ventana principal y sus atributos
    root = Tk()
    root.title(version)
    root.geometry("650x500")
    root.resizable(False,False)
    #icono de la ventana
    imagen = Image("photo",'logoapp.ico')
    root.iconphoto(True,imagen)

    #Frame donde vamos a añadir los elementos
    frme = Frame(root,bg="#C4302B")
    frme.pack(fill="both",expand=True)

    #comboBox usado para seleccionar si el enlace pertenece a un vídeo o a una lista de reproducción
    comboBox = Combobox(root, values=opciones, state="readonly")
    comboBox.current(1)
    comboBox.configure(width=5, height=5)
    comboBox.place(x=5,y=200)

    #Entrada de texto para el enlace
    txtentry = Entry(frme)
    txtentry.configure(width=50)
    txtentry.place(x=75,y=200)

    #Boton para convertir
    btn = Button(frme,text="Convertir",command=descargador)
    btn.place(x=500, y=200)

    #Etiqueta para la dirección
    lbel2 = Label(frme,text="Carpeta:",bg="#C4302B",font='Arial')
    lbel2.place(x=5,y=250)

    #Entrada de texto para la carpeta
    txtentry2 = Entry(frme)
    txtentry2.configure(width=50)
    txtentry2.place(x=75, y=250)

    #Boton para seleccionar carpeta
    btn2 = Button(frme,text="Seleccionar",command=selccionar)
    btn2.place(x=500, y=250)

    root.mainloop()

if __name__ == '__main__':
    main()
