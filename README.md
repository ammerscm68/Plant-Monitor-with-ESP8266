# Plant-Monitor-with-ESP8266 (MQTT-Broker Version)
A Plant Monitor with ESP8266 and Capacitive Soil Moisture Sensor and MQTT Broker

# Hardware: 
RGB LED  --> Green = GPIO 2 (D4) / Blue = GPIO 0 (D3) / Red = GPIO 4 (D2) <br>
ESP8266 (Wemos D1 Mini) <br>
Capacitive Soil Moisture Sensor V1.2 an A0 Pin from then D1 Mini <br>

# Manual RGB-LED Indikator
When the sensor is started for the first time, the "Captive Portal" is started and the LED lights up "BLUE" <br>
After entering the data, the sensor restarts and the RGB LED lights up as follows: <br>

1. Checking the RGB LED --- All three colors flash three times (Red/Blue/Green) <br>
2. Establishing WiFi connection RGB LED blinks BLUE/GREEN <br>
3. If the WLAN connection is successful and everything is loaded, the RGB LED lights up green --- otherwise RED <br>
4. With each measurement, the RGB LED flashes Green once briefly. <br>

Note: If the WLAN connection cannot be established for about 15 minutes, the "Captive Portal" will be started!

# Software:
A "captive portal" is created when the software is started for the first time. (Name = Plant-Monitor) <br>
Make the appropriate entries here. A website can be opened via the assigned IP address or with the Hostname. <br>
(The Hostname is "Sensorname".local - Example: kitchenplant.local) <br>
Should it ever be necessary to change the WLAN access data and other parameters, this can be <br>
done by a RESET on the website. If this is not possible then simply wait (when WLAN is switched off!) <br>
until the LED lights up only blue and the "Captive Portal" starts again. <br>
The new data can now be entered.<br>
<br>
New from version 1.4.0: <br>
new Captive Portal (WiFi-Manager)<br>
bug fix<br>
<br>
New from version 1.2.8: <br> 
Now with Arduino OTA. <br>
Sketch updates without a USB cable over the network <br>
<br>
New from version 1.1.8: <br>
Improved webinterface <br>
bug fix <br>
The webinterface can now be reached with the DNS name. The DNS name is equal to the name of the sensor. <br>
(Example : http://kitchenplant.local) <br>
<br>
New from version 1.1.5: <br>
After switching on the sensor, a measurement is taken every 15 seconds to check the values. (Number of measurements: 20 - start mode)  <br>
After these 20 measurements, the measurement takes place again at the set interval.  <br>
<br>
New from version 1.1.2: <br>
The sensor now also works without MQTT (do not specify the MQTT host in the captive portal) <br>
The sensor can now also be calibrated during operation.
