# Simple HTTP Server Example
# Control an LED and read a Button using a web browser

import utime
import time
import network
import socket
import machine
from machine import Pin,UART

ssid = "Eddy's Galaxy A53 5G"
password = '12345678'

# ssid = "INFINITUM1930"
# password = 'Fed22dEbSa'

uart = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1))

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect(ssid, password)

html = """<!DOCTYPE html>
<html>
<head> <title>Pico W</title> </head>
<body> <h1>Pico W HTTP Server</h1>
<p>Hello, World!</p>
<p>%s</p>
</body>
</html>
"""

ip = ""

# Wait for connect or fail
max_wait = 10
while max_wait > 0:
    if wlan.status() < 0 or wlan.status() >= 3:
        break
    max_wait -= 1
    print('waiting for connection...')
    time.sleep(1)
    
# Handle connection error
if wlan.status() != 3:
    raise RuntimeError('network connection failed')
else:
    print('Connected')
    status = wlan.ifconfig()
    print( 'ip = ' + status[0] )
    ip = status[0]
    
    
# Open socket
addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]
s = socket.socket()
s.bind(addr)
s.listen(1)
print('listening on', addr)

def send_data(data):
    if isinstance(data, str):
        data = data.encode()  # Convert string to bytes
    uart.write(data)

# Listen for connections, serve client
while True:
    try:
        if uart.any():
            cl, addr = s.accept()
            print('client connected from', addr)
            request = cl.recv(1024)
            print("request:")
            print(request)
            request = str(request)
            
            data = None
            
            send_data(ip)
            
            data = uart.readline()  # Read data sent from Arduino
            print(data)  # Print to console
            time.sleep(0.5)
            
            # Create and send respon se
            response = html % data
            cl.send('HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n')
            cl.send(response)
            cl.close()
        
    except OSError as e:
        cl.close()
        print('connection closed')




