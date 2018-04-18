# AVR DHT11/DHT22 Library

---

**Description:**  
A simple, yet clean, library to control the DHT11/DHT22 temperature and humidity sensor using AVR microcontrollers.  
+ This library does not use any hardware timer or interrupts.  
+ By contrast, the readings are accomplished with delays.  
+ Functions include:  
  + Reading raw data in an array.  
  + Getting the calculated temperature and/or humidity directly.  
  + Converting the temperature from Celsius to Kelvin or Fahrenheit units.  
  _Read the bottom of "DHT.h" to see all functions available._  
+ **The readings are time-critical, so every time you perform a reading, you should temporarily disable interrupts.** 
+ My custom I/O macros are required and are included.  

---

**Memory usage:**  
+ Program: 0 bytes  
+ Data: 1 byte  
  + DHT status: 1 byte  
  
**Compiler:**  
AVR-GCC  
  
**Optimization Level:**  
Optimize (-O1)  
  
--- 

![picture alt](https://raw.githubusercontent.com/efthymios-ks/AVR-DHT-Library/master/Demonstration.png)  

---
