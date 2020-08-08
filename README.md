# esp32_projects
various projects with an ESP32 microcontroller using various sensors and breadboards

I have purchased from Amazon.com two ESP32S development boards with the ESP32 microcontroller.

MELIFE 2 Pack ESP32 ESP-32S Development Board 2.4GHz Dual-Mode WiFi + Bluetooth Dual Cores Microcontroller Processor Integrated with ESP32s Antenna RF AMP Filter AP STA for Arduino IDE

**Product Description**

The product description says:
 - 2 PACK 2.4GHz Dual Mode WiFi + Bluetooth Development Board
 - Ultra-Low power consumption, works perfectly with the Arduino IDE
 - Support LWIP protocol, Freertos
 - SupportThree Modes: AP, STA, and AP+STA
 - ESP32 is a safe, reliable, and scalable to a variety of applications

Development Board:
 - https://github.com/espressif/arduino-esp32
 - https://github.com/nodemcu/nodemcu-firmware/tree/dev-esp32
 - https://github.com/Nicholas3388/LuaNode

See also https://www.arduino.cc/en/Tutorial/HomePage with various tutorials about Arduino IDE.

If it can't get bluetooth working?

Using Node32s as the Board selection. To reference GPIO pins in code use just the number, for example "digitalWrite(13, HIGH)" sets GPIO13 high.
The built-in LED to GPIO2.

My development environment is:
 - the Arduino IDE 1.8.13 on a Microsoft Surface Pro 2 with Windows 10
 - using a standard USB to micro-USB cable from a Samsung cell phone
   - powers the ESP32
   - provides serial cable connectivity for downloading programs and the IDE Serial Monitor

The Arduino IDE shows the following settings:
 - Board: "NodeMCU-32S"
 - Upload Speed: "921600"
 - Flash Frequency: "80MHz"
 
 Get Board Info shows "Unknown board".
 
**RTOS for the ESP32 Development Board**

FreeRTOS https://www.freertos.org/index.html

Espressif ESP-IDF Programming Guide - Section on FreeRTOS https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html

ESP32: Introduction to FreeRTOS https://icircuit.net/esp32-introduction-freertos/1297

Mongoose OS https://mongoose-os.com/

ARM Mbed OS https://os.mbed.com/
