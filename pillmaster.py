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
topButtonFrame = tk.Frame(buttonFrame)
bottomButtonFrame = tk.Frame(buttonFrame)
weightFrame = tk.Frame(buttonFrame)
diagFrame = tk.Frame(window)
text_area = tk.scrolledtext.ScrolledText(diagFrame)
text_area.config(height = 10, width = 30)

bigFont = font.Font(family = GLOBAL_FONT, size = 30, weight = 'bold')
subBigFont = font.Font(family = GLOBAL_FONT, size = 24, weight = 'bold')

def operateA():
    ser.write(bytes("a", 'utf-8'))

def operateB():
    ser.write(bytes("b", 'utf-8'))

def operateC():
    ser.write(bytes("c", 'utf-8'))
    
def operateAdjA():
    ser.write(bytes("A", 'utf-8'))

def operateAdjB():
    ser.write(bytes("B", 'utf-8'))

def operateAdjC():
    ser.write(bytes("C", 'utf-8'))
    
def operateW():
    ser.write(bytes("w", 'utf-8'))
    buttonA['state'] = 'disabled'
    buttonB['state'] = 'disabled'
    buttonC['state'] = 'disabled'
    buttonA_Half['state'] = 'disabled'
    buttonB_Half['state'] = 'disabled'
    buttonC_Half['state'] = 'disabled'

def operateX():
    ser.write(bytes("x", 'utf-8'))
    buttonA['state'] = 'normal'
    buttonB['state'] = 'normal'
    buttonC['state'] = 'normal'
    buttonA_Half['state'] = 'normal'
    buttonB_Half['state'] = 'normal'
    buttonC_Half['state'] = 'normal'

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

buttonA = tk.Button(topButtonFrame, text = 'A', command = operateA)
buttonA['font'] = bigFont

buttonB = tk.Button(topButtonFrame, text = 'B', command = operateB)
buttonB['font'] = bigFont

buttonC = tk.Button(topButtonFrame, text = 'C', command = operateC)
buttonC['font'] = bigFont

buttonA_Half = tk.Button(bottomButtonFrame, text = '1/2', command = operateAdjA)
buttonA_Half['font'] = bigFont

buttonB_Half = tk.Button(bottomButtonFrame, text = '1/2', command = operateAdjB)
buttonB_Half['font'] = bigFont

buttonC_Half = tk.Button(bottomButtonFrame, text = '1/2', command = operateAdjC)
buttonC_Half['font'] = bigFont

buttonW = tk.Button(weightFrame, text = 'Enable\nWeight', command = operateW)
buttonW['font'] = subBigFont

buttonX = tk.Button(weightFrame, text = 'Disable\nWeight', command = operateX)
buttonX['font'] = subBigFont

calButton = tk.Button(diagFrame, text = 'Open\nCalendar', command = openCalendar)
calButton['font'] = subBigFont
    
pillButton = tk.Button(diagFrame, text = 'Add New\nMedication', command = addPill)
pillButton['font'] = subBigFont

text_area.pack(side=BOTTOM, expand = True, fill = 'both')

diagFrame.pack(side=RIGHT, expand = True, fill = 'both')
buttonFrame.pack(side=LEFT, expand = TRUE, fill = 'both')
topButtonFrame.pack(side=TOP, expand = True, fill = 'both')
bottomButtonFrame.pack(side=TOP, expand = True, fill = 'both')
weightFrame.pack(side=BOTTOM, expand = True, fill = 'both')

buttonA.pack(side=LEFT, expand = True, fill = 'both')
buttonB.pack(side=LEFT, expand = True, fill = 'both')
buttonC.pack(side=LEFT, expand = True, fill = 'both')
buttonA_Half.pack(side=LEFT, expand = True, fill = 'both')
buttonB_Half.pack(side=LEFT, expand = True, fill = 'both')
buttonC_Half.pack(side=LEFT, expand = True, fill = 'both')
buttonW.pack(side=LEFT, expand = True, fill = 'both')
buttonX.pack(side=LEFT, expand = True, fill = 'both')

calButton.pack(side = TOP, expand = True, fill = 'both')
pillButton.pack(side = TOP, expand = True, fill = 'both')
window.after(0, updateText)
window.mainloop()