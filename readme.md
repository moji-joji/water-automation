# Smart Water Tank

## Description
#### CMS: 341699

Water automation system  has two use cases
- Fill water in tank upto percentage specified by user
- Heat water to temperature specified by user

The system is controlled using the webpage which is served by the ESP 32 webserver.

## Parts Used
Total Cost =  PKR 1700
 - ESP 32 
- SR-04 Ultrasonic sensor 
- DS18B20 Temperature sensor  
- Bread board
-  Leds
- Resistors
- Water Container
- USB data cable
- Jumper wires
## Interface
The user interface is implemented through a web server which is acessible through the localhost. The ESP 32 connects to the wifi using SSID and password predefined in a secret file. The user connects to the same wifi network to access the webpage.

## Status Leds
Onboard LEDs are present to reflect status.
|                |LED                          |State                         |
|----------------|-------------------------------|-----------------------------|
||Yellow Led             |WiFi is connected            |
|       |Green Led            |Water is being filled           |
|         |Red Led|Water is being heated|




## System design:

```mermaid
graph LR
C[Ultra sonic sensor] --Data--> B[ESP 32]
D[Temperature sensor] --Data--> B
A[Webpage] -- Request --> B
B --Response-->A
E[User]--Interacts-->A