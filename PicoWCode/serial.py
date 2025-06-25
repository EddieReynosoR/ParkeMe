from machine import UART, Pin
import time

uart = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1))

while True:
    if uart.any():
        data = uart.readline()  # Read data sent from Arduino
        print(data)  # Print to console
    time.sleep(0.5)