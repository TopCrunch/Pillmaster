import serial
import tkinter as tk
import tkinter.messagebox
import tkinter.scrolledtext
from time import sleep
ser = serial.Serial("/dev/ttyACM0", 9600)
window = tk.Tk()
tk.messagebox.showinfo("Initializing...", ser.read(90))
text_area = tk.scrolledtext.ScrolledText(window)
def operatea():
        ser.write(bytes("a", 'utf-8'))
        tk.messagebox.showinfo("Container A", ser.readline())

def operateb():
        ser.write(bytes("b", 'utf-8'))
        tk.messagebox.showinfo("Container B", ser.readline())
  
def operatec():
        ser.write(bytes("c", 'utf-8'))
        tk.messagebox.showinfo("Container C", ser.readline())
        
def operatew():
        ser.write(bytes("w", 'utf-8'))
        tk.messagebox.showinfo("Reading Weight", ser.readline())
        
def operatex():
        ser.write(bytes("x", 'utf-8'))
        tk.messagebox.showinfo("Finished Reading Weight", ser.readline())
        
buttona = tk.Button(master = window, text = 'a', command = operatea)
buttonb = tk.Button(master = window, text = 'b', command = operateb)
buttonc = tk.Button(master = window, text = 'c', command = operatec)
buttonw = tk.Button(master = window, text = 'w', command = operatew)
buttonx = tk.Button(master = window, text = 'x', command = operatex)
buttona.pack()
buttonb.pack()
buttonc.pack()
buttonw.pack()
buttonx.pack()
window.mainloop()
