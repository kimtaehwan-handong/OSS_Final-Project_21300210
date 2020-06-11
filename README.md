2020-2 OSS Final project - IOT smart home.
===========================
## 1. What does this project do?
> Purpose of this project
* Check the state of house by using web.
* Control the house according to the condtion.
* By using more ESP8266, you can check the state of each room like living room, kitchen, room1, ...

## 2. Why is this project useful?
* You can know about the condition of the house from the outside.
  1. Temperature
  2. Humidity
  3. Door State
  4. CDS (Light sensor)
* After check condition, you can control house.
  1. Control LED
  2. You can add more sensor and Actuator what you want.

## 3. How do I get started?
1. Prepare the ESP8266 board and Raspberry pi.
2. Upload OSS.ino file to ESP8266 board
>> How to upload
* Download Arduino IDE program at arduino.cc
* go File -> Preferences -> Additional board manger URL -> add http://arduino.esp8266.com/stable/package_esp8266com_index.json
* go Tool -> Board -> Board Manager -> serch ESP -> install esp8266 by ESP8266 Community
* gp Tool -> Board -> choose NodeMCU 1.0 (ESP-12E Module)
 
3. Upload project.py file to raspberry pi.
>> How to Upload
Python module install
* sudo apt-get install python-pip
Flask server install
* sudo pip install flask
MQTT protocol install
* sudo pip install paho-mqtt
* sudo nano project.py -> copy project.py file
* sudo python project.py -> execute

## 4. Where can I get more help, if I need it?

Please send e-mail what you need.
<21300210@handong.edu>

Also, I add the OSS_FinalProject_IOT.pptx file about how to execute this porject.

And, This is video link address. You can see the operation of this project. 
