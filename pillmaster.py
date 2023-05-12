import serial
import platform
import tkinter as tk
from tkinter import *
from tkinter.ttk import *
from tkinter import messagebox
from tkinter import simpledialog
import tkinter.font as font
import tkinter.scrolledtext
from tkcalendar import Calendar
from time import sleep
import datetime
SERIAL_PORT_WIN = "COM4"
SERIAL_PORT_LNX = "/dev/ttyACM0"
GLOBAL_FONT = 'Fixedsys'

if(platform.system().lower() == 'windows'):
    ser = serial.Serial(SERIAL_PORT_WIN, 9600)
else:
    ser = serial.Serial(SERIAL_PORT_LNX, 9600)
    
window = tk.Tk()
window.geometry("800x480")

buttonFrame = tk.Frame(window)
diagFrame = tk.Frame(window)
text_area = tk.scrolledtext.ScrolledText(diagFrame)
text_area.config(height = 10, width = 30)

bigFont = font.Font(family = GLOBAL_FONT, size = 30, weight = 'bold')
subBigFont = font.Font(family = GLOBAL_FONT, size = 24, weight = 'bold')

def operatea():
    ser.write(bytes("a", 'utf-8'))

def operateb():
    ser.write(bytes("b", 'utf-8'))

def operatec():
    ser.write(bytes("c", 'utf-8'))

def operatew():
    ser.write(bytes("w", 'utf-8'))
    buttona['state'] = 'disabled'
    buttonb['state'] = 'disabled'
    buttonc['state'] = 'disabled'

def operatex():
    ser.write(bytes("x", 'utf-8'))
    buttona['state'] = 'normal'
    buttonb['state'] = 'normal'
    buttonc['state'] = 'normal'

def openCalendar():
    calWindow = tk.Toplevel(window)
    calWindow.title("Calendar")
    calWindow.geometry("200x200")
    
    calendar = Calendar(calWindow,\
                        selectmode = "day",\
                        year = datetime.date.today().year,\
                        month = datetime.date.today().month,
                        day = datetime.date.today().day)
                        
    calendar.pack(fill = 'both', expand = True, side = TOP)

def addPill():
    pillName = simpledialog.askstring("Input", "Enter the name of the medication:", parent = window)
    numTimes = simpledialog.askinteger("Input", "How many times per day should you take " + pillName + "?",\
                                        parent = window, minvalue = 1)
    confirm = messagebox.askyesno("Confirmation", "Is this information correct?\n"\
                                    + pillName + ": " + str(numTimes) + " times a day", parent = window)
    
    
def updateText():
    fullDown = text_area.yview()[1] == 1.0
    text_area.configure(state='normal')
    if(ser.in_waiting > 0):
        text_area.insert(tk.END, ser.readline())
    if fullDown:
        text_area.see("end")
    text_area.configure(state='disabled')
    window.after(100, updateText)

buttonA = tk.Button(buttonFrame, text = 'A', command = operatea)
buttonA['font'] = bigFont

buttonB = tk.Button(buttonFrame, text = 'B', command = operateb)
buttonB['font'] = bigFont

buttonC = tk.Button(buttonFrame, text = 'C', command = operatec)
buttonC['font'] = bigFont

buttonW = tk.Button(buttonFrame, text = 'Enable\nWeight', command = operatew)
buttonW['font'] = subBigFont

buttonX = tk.Button(buttonFrame, text = 'Disable\nWeight', command = operatex)
buttonX['font'] = subBigFont

calButton = tk.Button(diagFrame, text = 'Open\nCalendar', command = openCalendar)
calButton['font'] = subBigFont
    
pillButton = tk.Button(diagFrame, text = 'Add New\nMedication', command = addPill)
pillButton['font'] = subBigFont

text_area.pack(side=BOTTOM, expand = True, fill = 'both')

diagFrame.pack(side=RIGHT, expand = True, fill = 'both')
buttonFrame.pack(side=LEFT, expand = True, fill = 'both')

buttonA.pack(side=LEFT, expand = True, fill = 'both')
buttonB.pack(side=LEFT, expand = True, fill = 'both')
buttonC.pack(side=LEFT, expand = True, fill = 'both')
buttonW.pack(side=LEFT, expand = True, fill = 'both')
buttonX.pack(side=LEFT, expand = True, fill = 'both')

calButton.pack(side = TOP, expand = True, fill = 'both')
pillButton.pack(side = TOP, expand = True, fill = 'both')
window.after(0, updateText)
window.mainloop()