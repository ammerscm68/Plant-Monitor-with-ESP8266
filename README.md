# Plant-Monitor-with-ESP8266
A Plant Monitor with ESP8266 and Capacitive Soil Moisture Sensor and MQTT Client

# Hardware: 
RGB LED  --> Green = GPIO 2 (D4) / Blue = GPIO 0 (D3) / Red = GPIO 4 (D2) <br>
ESP8266 (Wemos D1 Mini) <br>
Capacitive Soil Moisture Sensor V1.2 an A0 Pin from then D1 Mini <br>

# Software:
A "captive portal" is created when the software is started for the first time. (Name = Plant-Monitor)
Make the appropriate entries here. A website can be opened via the assigned IP address or with the Hostname.
(The Hostname is "Sensorname".local - Example: kitchenplant.local) 
Should it ever be necessary to change the WLAN access data and other parameters, this can be
done by a RESET on the website. If this is not possible then simply
wait (when WLAN is switched off!) until the LED lights up only blue and the "Captive Portal" starts again.
The new data can now be entered.
