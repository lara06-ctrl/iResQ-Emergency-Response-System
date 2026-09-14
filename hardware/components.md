iResQ Hardware Components

Main Controller

ESP32 38-Pin CH340

Main microcontroller of the iResQ emergency response device. It receives button inputs, processes GPS data, and communicates with the SIM800L module.

Communication and Location

GPS Module

Provides the device's geographical coordinates, including latitude and longitude, when an emergency alert is triggered.

SIM800L

Provides GSM/GPRS communication so the ESP32 can transmit emergency alert information to the server.

Emergency Buttons

Fire Button

Triggers a fire emergency alert.

ESP32 GPIO: 25

Medical Button

Triggers a medical emergency alert.

ESP32 GPIO: 26

Police Button

Triggers a police emergency alert.

ESP32 GPIO: 27

Power System

Rechargeable Battery

Provides portable power for the iResQ device.

Charging Module

Used to safely charge the rechargeable battery.

Buck Converter

Regulates and steps down the battery voltage to the required voltage levels for the device's electronic components.

Power Switch

Allows the user to turn the iResQ device on and off.

Wiring

The hardware connections are documented in the "wiring" folder.

PCB

Photos and PCB-related documentation are stored in the "pcb" folder.