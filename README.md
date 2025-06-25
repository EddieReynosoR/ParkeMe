# Smart Parking System 🚗📶

A final integrative systems project that implements a **Smart Parking System** using **Arduino UNO**, **IR sensors**, **RFID modules**, **Raspberry Pi Pico W**, and various actuators to monitor and manage real-time parking space availability.

## 📌 Project Overview

The system uses **IR sensors** to detect parked vehicles, **ultrasonic sensors** to detect entry/exit, and an **RFID module** to control secure access. It integrates with a **Raspberry Pi Pico W**, which transmits parking data over Wi-Fi to external interfaces such as mobile apps or web applications using **WebSockets**. This project aims to optimize urban parking efficiency and enhance user convenience.

## 🎯 Objectives

### Main Objective
Develop a smart parking solution that detects and displays available parking spots in real time, using sensors and microcontrollers.

### Specific Objectives
- Real-time spot monitoring with IR sensors.
- Control car access with RFID and ultrasonic sensors.
- Display spot availability on an LCD.
- Share data via Wi-Fi using a Raspberry Pi Pico W.
- Reduce traffic congestion by guiding drivers to available spaces.
- Create a scalable and reliable system for smart cities.

## 🧠 Theoretical Background
This system builds upon previous research on mechanical parking systems and smart parking monitoring via RFID and visual sensors. It improves upon those by focusing on modularity, real-time data transmission, and integration with remote access interfaces.

## 🔧 Components Used

- **Arduino UNO** – Main controller for parking slot sensors.
- **IR Sensors (x6)** – Detect vehicle presence in each slot.
- **Ultrasonic Sensors (x2)** – Detect vehicle at entry/exit.
- **RFID Module** – Authorize entry with RFID tags.
- **LCD Display (16x2)** – Show spot availability and instructions.
- **Servo Motors (x2)** – Control entry/exit gates.
- **Buzzer** – Alert unauthorized access or errors.
- **Raspberry Pi Pico W** – Web server and wireless data transmission.

## ⚙️ System Workflow

### 🚪 Entrance System
1. Ultrasonic sensor detects incoming vehicle.
2. RFID reader scans tag.
3. If valid, servo opens the gate.
4. If invalid, buzzer sounds an alert.

### 🅿️ Parking Slot Monitoring
1. IR sensors detect presence in each slot.
2. Arduino collects data and sends to Pico W.
3. Pico W serves this data via WebSockets to external applications.
4. Users can view parking status remotely.

## 💻 Code Structure

- `Car_Parking_System_LCD`: Arduino code for monitoring 6 IR sensors and sending data via UART.
- `Parking_Entrance.ino`: Arduino code for RFID-based gate control and LCD messages.
- `microdot.py`: Python code for Raspberry Pi Pico W to serve parking data over Wi-Fi.

## 🌐 Remote Access

The Raspberry Pi Pico W hosts a lightweight web server, which:
- Accepts UART data from Arduino.
- Streams parking data via WebSockets.
- Displays availability via a simple HTML interface.

## 🧪 Usability

- Real-time visualization of parking slots.
- Remote availability checking via mobile/web.
- Scalable to larger parking areas.
- Enhances safety and reduces traffic congestion.

## 📸 Photos & Simulations

See `/docs` folder for:
- Circuit diagrams (Fritzing).
- Physical build photos.
- UI screenshots of the parking app.

## ✅ Conclusions
The Smart Parking System improves efficiency, reduces traffic and operational costs, and contributes to sustainable urban mobility. It offers an affordable and modular solution for environments like malls, hospitals, and business centers.

![Captura de pantalla 2025-06-24 212416](https://github.com/user-attachments/assets/d651870d-d4d6-4b7e-80e4-2eeef92b3f37)
![Captura de pantalla 2025-06-24 212456](https://github.com/user-attachments/assets/c2e498f2-2b3a-4da2-b521-ae1d3877b16c)
![Captura de pantalla 2025-06-24 212245](https://github.com/user-attachments/assets/95b05a77-7bb3-4ad1-b910-7e92a3b1387e)


