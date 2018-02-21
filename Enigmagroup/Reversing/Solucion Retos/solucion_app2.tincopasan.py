#-*- coding:utf -8 -*-
import win32gui,win32con
import time
 
def click(identificador):                                           #para simular el click del mouse                                                        
    win32gui.SendMessage(identificador, win32con.WM_LBUTTONDOWN, 0, 0)       #señal de presionar el botón   
    win32gui.SendMessage(identificador, win32con.WM_LBUTTONUP, 0, 0)         #señal de soltar el botón  
 
nombre = "Enigma Group - App Challenge 2"        #acá va el título de la ventana
ventana = win32gui.FindWindow(None,nombre)       #asigno a la variable ventana un valor entero que sería el identificador
if ventana != 0:                                 #si el valor es distinto de 0 es que la encontró  
    print("Encontrada")
    win32gui.SetForegroundWindow(ventana)        #traigo la ventana al frente para ver que hace,aunque no es necesario    
    boton1 = win32gui.FindWindowEx(ventana,None,None,"Submit")        #el identificador del botón submit 
    boton2 = win32gui.FindWindowEx(ventana,None,None,"Cancel")        #el identificador del botón cancel  
    texto= win32gui.FindWindowEx(ventana,None,"ThunderRT6TextBox","")  # lo bueno de conocer otros lenguajes y sus clases ThunderRT6TextBox
    texto1= win32gui.FindWindowEx(ventana,None,None,"")                #no tiene nada como titulo y se obtiene igual el identificador  
    clave="topgun"
    time.sleep(2)                  #espero para poner la clave, porque quiero nada más
    win32gui.SendMessage(texto1,win32con.EM_SETPASSWORDCHAR,None,3) #cambio el "*" por su caracter
    win32gui.SendMessage(texto, win32con.WM_SETTEXT, 8, clave)    #envío la clave al textbox, también podría usar texto1
    time.sleep(2)                                                  #otra espera porque si
    click(boton1)                                                  #aunque no este activado ejecuta las sentencias igual
    time.sleep(5)                                                  #ya sabemos
    click(boton2)                                                  #cerramos la aplicación   
else:
    print("No encontrada")
 