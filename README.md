# MN_ESPNOW_ESP32_Scheduler
* Node 0 is the server, it has the main animation configuration that you can set up via a web broser.
* Node 1 and all nodes afterwards are the nodes under controll
* We use CUSTOM MAC addressing for each node to create a dasiy chain network
* A custom written software stack that utelises ESPNOW to BRODCAST or UNICAST messages across the daisy chain network
* Buit in redudancy in case of power outages/ drop outs etc.. Nodes can run independently and can request schedule after they reboot 

# Prerequisite Software
* The Arduino IDE via [Arduino](https://www.arduino.cc/)
* ESP32 Sketch Data upload module for the Arduino IDE

# Seting up the Arduino IDE
* Goto https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/tag/1.0
* Download this zip file https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/download/1.0/ESP32FS-1.0.zip to your desktop
* Open the following folder on your pc C:\Users\YOUR_USER_NAME\Documents\Arduino\tools
* Extract the folder ESP32FS into the TOOLS folder above
* Close the folder, open the Arduino IDE
* Click on the TOOLS menu and check that you see "ESP32 Sketch DATA Uplaod tool" there
* Close the IDE

# Setting up the NODE0 - Combined SERVER+NODE Module

I have left the boot button for this as well so you will still need to connect a resistor with any value from 100ohm to 1 Mega Ohm between GND and Pin 36. Then grab a small pushbutton and conect one terminal to 3.3v on the esp32 and the other to pin 36. This is the boot button that will let you boot the server into config mode

* Open the Arduino IDE and open the Node0.ino file
* Set up the board and select the correct Serial port
* Upload the code, once done proceed to the next step (make sure the serial monitor is closed)
* Click on the TOOLS menu, click on "ESP32 Sketch DATA Upload" watch the status board, when done it will say restarted do not do anything untill it says this
* Open the serial terminal
* upload the code AGAIN, the server is now done and ready to be configured
* Procceed to set up a basic animation schedule

# Configuring the animation schedule 

You will need a phone or tablet with WIFI and a web browser

* WIth the system powered OFF, hod down the bott button hooked up to pin 36. Power on the system and release teh push butotn after 2 seconds
* conect your device to the following wifi network "WOW-APP" with the password "wowaccesspoint"
* open the web browser and type in "http://10.10.10.1" and hit enter to go to it (if it loads a blank page relaod it)
* Use this https://wow.elec-tron.org/collections/kits-standalone-modules/products/wifi-animation-que-controll-option as a guide on how to set up the animations

# Setting up the NODE 1 to Node X
* make sureyou configure the MAC address of THIS NODE, NEXT NODE and PREVIOUS NODE
* Upload the code

For the server SYNC to work, you need to make sure you have a daisy chain set up. The server sends a sync mesage to node 1, node 1 sends it to node 2, etc...
