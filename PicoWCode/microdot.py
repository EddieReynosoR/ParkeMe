import machine
from machine import Pin,UART

import time
import network

from microdot import Microdot, send_file
from microdot.websocket import with_websocket

ssid = "Eddy's Galaxy A53 5G"
password = '12345678'

#ssid = "TecNM-ITT"
#password = ''

#ssid = "TecNM-ITT-Docentes"
#password = "Person@L2024-2"

#ssid = "INFINITUM1930"
#password = "Fed22dEbSa"

def connect_to_wifi():
    connection = network.WLAN(network.STA_IF)
    if not connection.isconnected():
        print("Connecting to the network...")
        connection.active(True)
        connection.connect(ssid, password)
        while not connection.isconnected():
            pass
        print("Connected to IP: ", connection.ifconfig()[0])
    else:
        print("Connected to Internet")
        print("Connected to IP: ", connection.ifconfig()[0])
        
connect_to_wifi()

uart = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1))

html = """<!DOCTYPE html>
<html>
<head> <title>Pico W</title> </head>
<body> <h1>Pico W HTTP Server</h1>
<p>Hello, World!</p>
</body>
</html>
"""

app = Microdot()

@app.route("/")
async def index(request):
    return html, {"Content-Type": "text/html"}

@app.route("/test")
@with_websocket
async def index(request, ws):
    while True:
        try:
            data = None
            
            if uart.any():
                data = uart.readline()  # Read data sent from Arduino
                time.sleep(0.2)
                print(data)  # Print to console
                
            else:
                data = "No data available from UART"
            
            await ws.send(data)
            
        except OSError as e:
            print('connection closed')
            pass
        
app.run(port=80, debug=True)
