import serial
import platform
import tkinter as tk
from tkinter import *
import tkinter.messagebox
import tkinter.scrolledtext
from time import sleep
SERIAL_PORT_WIN = "COM4"
SERIAL_PORT_LNX = "/dev/ttyACM0"

if(platform.system().lower() == 'windows'):
    ser = serial.Serial(SERIAL_PORT_WIN, 9600)
else:
    ser = serial.Serial(SERIAL_PORT_LNX, 9600)
    
window = tk.Tk()
frame = tk.Frame(window, height = 30, width = 50)
text_area = tk.scrolledtext.ScrolledText(window)

def operatea():
        ser.write(bytes("a", 'utf-8'))
        #tk.messagebox.showinfo("Container A", ser.readline())

def operateb():
        ser.write(bytes("b", 'utf-8'))
        #tk.messagebox.showinfo("Container B", ser.readline())
  
def operatec():
        ser.write(bytes("c", 'utf-8'))
        #tk.messagebox.showinfo("Container C", ser.readline())
        
def operatew():
        ser.write(bytes("w", 'utf-8'))
        #tk.messagebox.showinfo("Reading Weight", ser.readline())
        
def operatex():
        ser.write(bytes("x", 'utf-8'))
        #tk.messagebox.showinfo("Finished Reading Weight", ser.readline())
    
def updateText():
    fullDown = text_area.yview()[1] == 1.0
    text_area.configure(state='normal')
    if(ser.in_waiting > 0):
        text_area.insert(tk.END, ser.readline())
    if fullDown:
        text_area.see("end")
    text_area.configure(state='disabled')
    window.after(100, updateText)
    
frame.grid_propagate(False)
frame.columnconfigure(0, weight = 1)
frame.rowconfigure(0, weight = 1)

buttona = tk.Button(frame, text = 'a', height = 5, width = 10, command = operatea)
buttonb = tk.Button(frame, text = 'b', height = 5, width = 10, command = operateb)
buttonc = tk.Button(frame, text = 'c', height = 5, width = 10, command = operatec)
buttonw = tk.Button(frame, text = 'w', height = 5, width = 10, command = operatew)
buttonx = tk.Button(frame, text = 'x', height = 5, width = 10, command = operatex)
text_area.pack(side=BOTTOM)
frame.pack(side=LEFT)
buttona.pack(side=LEFT)
buttonb.pack(side=LEFT)
buttonc.pack(side=LEFT)
buttonw.pack(side=LEFT)
buttonx.pack(side=LEFT)
window.after(0, updateText)
window.mainloop()