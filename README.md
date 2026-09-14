iResQ — Emergency Response System 

Overview

iResQ is an IoT-based emergency response system developed as a BSIT capstone project for the San Miguel Municipal Disaster Risk Reduction and Management Office (MDRRMO).

The system allows registered users to send emergency alerts using a physical button on a portable device. The device obtains the user's location and transmits the emergency information to a web-based system where authorized responders can monitor and manage alerts.

Objectives

- Provide a simple way to send emergency alerts
- Capture the user's location using GPS
- Transmit emergency information through GSM
- Store alert information in a MySQL database
- Provide responders with a web-based monitoring dashboard

System Architecture

Emergency Button
       ↓
     ESP32
       ↓
      GPS
       ↓
  GSM / SIM800L
       ↓
    PHP API
       ↓
 MySQL Database
       ↓
 Web Dashboard
       ↓
 MDRRMO / Responders

Technologies

Hardware: ESP32, GPS module, SIM800L GSM module, SH1106 OLED display, 18650 battery, TP4056 charging module

Software: C++, PHP, MySQL, HTML, CSS, JavaScript

Tools: Arduino IDE, XAMPP, Git, GitHub, Cisco Packet Tracer

Key Features

- Emergency alert activation through a physical button
- Long-press activation
- GPS location acquisition
- GSM-based communication
- Emergency alert transmission
- MySQL database storage
- Web-based alert monitoring
- User registration
- Device registration
- Alert status management
- Alert history

System Components

ESP32 Firmware — Controls the emergency device and connected hardware.

PHP API — Receives and processes alert data.

MySQL Database — Stores users, devices, and emergency alerts.

Web Dashboard — Allows authorized personnel to monitor and manage emergency alerts.

Screenshots

Screenshots and system documentation will be added here.

Project Information

Project: iResQ — Emergency Response System
Project Type: BSIT Capstone Project
Institution: Bulacan State University
Client: San Miguel MDRRMO

Project Status

In Development
