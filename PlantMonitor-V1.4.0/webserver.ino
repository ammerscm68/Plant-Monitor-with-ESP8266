// Webserver Startseite
void handleRoot() {
// &nbsp = Leerzeichen | <meta http-equiv=\"refresh\" content=\"30\">\ = Refresh alle 30 sekunden 
if (SoilCalibrate == true) 
{SoilCalibrate = false; 
GetSoilMoistValue(25); // Nach dem Kalibrieren aktuellen Messwert holen  
Serial.println("Sensorkalibrierung beendet ...");}  
String RefreshTime = "";
if (!ResetNow) {RefreshTime = "15";} else {RefreshTime = "3";} // Je nach Status
// Startmodus anzeigen
String MInterval,SMMessureCount;
if (StartMessureCount > 0) {
MInterval = "Messintervall: alle "+(String)(soilMessureTrigger/2)+" Sekunden (Startmodus)";
if (MCount < 0) {SMMessureCount = "Anzahl der Messungen im Startmodus: k.A.";} else {
SMMessureCount = "Anzahl der Messungen im Startmodus:  &nbsp;noch "+ (String)(MCount-1);}
} else {
MInterval = "Messintervall: alle "+measurementinterval+" Minuten";
SMMessureCount = "<br />";} // Nach Startmodus eine Leezeile
// Farbe des Messwertes je nach Wert
String ValueColor;
if (percentSoilMoist < 15) {
ValueColor = "<tr><td style=\"height: 10px;\">Aktuelle Bodenfeuchte:&nbsp;&nbsp;</td><td id='Rvalue'><span>"+String(percentSoilMoist)+"</span>%</td></tr>";  
} else {
if (percentSoilMoist >= 15 && percentSoilMoist <= 60) {ValueColor = "<tr><td style=\"height: 10px;\">Aktuelle Bodenfeuchte:&nbsp;&nbsp;</td><td id='Yvalue'><span>"+String(percentSoilMoist)+"</span>%</td></tr>";} else {  
ValueColor = "<tr><td style=\"height: 10px;\">Aktuelle Bodenfeuchte:&nbsp;&nbsp;</td><td id='Gvalue'><span>"+String(percentSoilMoist)+"</span>%</td></tr>";}
}

 yield();
 MyWaitLoop(100); // kurze Pause sonst Absturz bei Browser refresh
 server.send(200, "text/html", 
  "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv='content-type' content='text/html'; charset='utf-8'>\
  <meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">\
  <style>\
  body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 24px; margin-top: 0px; margin-bottom: 10px; }\
  h3 { font-size: 18px; margin-top: 0px; margin-bottom: 10px; }\
  #Rvalue { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #FC1E17; background-color: #000000; font-size: 60px; }\
  #Yvalue { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #FFC300; background-color: #000000; font-size: 60px; }\
  #Gvalue { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #9AFE2E; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
  <body>\
  <h1>"+SoilSensorname+"</h1>\
  <a><br /></a>\
  <h2>Letzte Messung: "+LastMessureDateTime+"</h2>\
  <h3>"+SMMessureCount+"</h3>\
  <a><br /></a>\
  <table>"+ValueColor+"</table>\
  <a><br /></a>\
  <a><br /></a>\
  <a href =\"/getadjustments\"><button class=\"button\" style=\"margin-left: 100px;height: 48px; width: 180px;color: blue; font-size: 14px\">Einstellungen Anzeigen</button></a>\
  <a href =\"/scalibrate\"><button class=\"button\" style=\"margin-left: 50px;height: 48px; width: 180px;color: green; font-size: 14px\">Sensor Kalibrieren</button></a>\
  <a><br /></a>\
  <a><br /></a>\
  <a><br /></a>\
  <h3>"+MInterval+"</h3>\
  </body>\
  </html>\
  ");
MyWaitLoop(150); // kurze Pause sonst Absturz bei Browser refresh 
if (littleFSInit == true && ResetNow == true) {
ResetNow = false;  
ResetPlantSensor();} // WLAN Zugangsdaten werden gel??scht    
if (RestartNow == true) {
RestartNow = false;  
Serial.println("Sensor wird neu gestartet");
ESP.restart();} 
}

/********************************************************************************************************************************************************/

void HTMLAnswer(String WebFunction) {
String HTMLString, HTMLAnswerString;  
if (WebFunction == "SensorMoistHighLevel") {HTMLAnswerString = "Der Wert "+soilmoisthighlevel+" wurde f??r den 100% trockenen Sensor ??bernommen und gespeichert.";} 
if (WebFunction == "SensorMoistLowLevel") {HTMLAnswerString = "Der Wert "+soilmoistlowlevel+" wurde f??r den 100% nassen Sensor ??bernommen und gespeichert.";} 
if (WebFunction == "MessureLEDFlashEIN1") {HTMLAnswerString = "Die RGB-LED blinkt kurz bei der Messung";} 
if (WebFunction == "MessureLEDFlashEIN2") {HTMLAnswerString = "Die RGB-LED blinkt kurz bei der Messung - Die Auswahl konnte nicht gespeichert werden!!!";} 
if (WebFunction == "MessureLEDFlashAUS1") {HTMLAnswerString = "Die RGB-LED blinkt nicht kurz bei der Messung";} 
if (WebFunction == "MessureLEDFlashAUS2") {HTMLAnswerString = "Die RGB-LED blinkt nicht kurz bei der Messung - Die Auswahl konnte nicht gespeichert werden!!!";} 
if (WebFunction == "ESPRestart") {HTMLAnswerString = "Der Sensor wird neu gestartet wenn Sie zur Startseite zur??ckkehren.";} 
if (WebFunction == "ResetYes") {HTMLAnswerString = "Der Access Point (Captive Portal) wird gestartet wenn Sie zur Startseite zur??ckkehren.";}
HTMLString = "<!DOCTYPE html> <html>\
  <head>\
  <title>LED-Matrix-Uhr</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
  body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 18px; margin-top: 0px; margin-bottom: 50px; }\
  h3 { font-size: 20px; margin-top: 0px; margin-bottom: 50px; }\
  </style>\
  </head>\
    <body>\
    <h1>"+SoilSensorname+"</h1>\
    <a><br /></a>\
    <h2>"+HTMLAnswerString+"</h2>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 180px;color: black; font-size: 14px\">zur??ck zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
}

/********************************************************************************************************************************************************/

void SensorCalibrate() {
// &nbsp = Leerzeichen | <meta http-equiv=\"refresh\" content=\"30\">\ = Refresh alle 30 sekunden 
String RefreshTime = "3"; // Site Refresh Time in Sekunden
SoilCalibrate = true;
 yield();
 MyWaitLoop(100); // kurze Pause sonst Absturz bei Browser refresh
 GetSoilMoistValue(25); // Messwert Auslesen
 server.send(200, "text/html", 
  "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv='content-type' content='text/html'; charset='utf-8'>\
  <meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">\
  <style>\
  body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 24px; margin-left: 5px; margin-top: 0px; margin-bottom: 50px; }\
  h3 { font-size: 18px; margin-top: 0px; margin-bottom: 10px; }\
  h4 { font-size: 12px; margin-top: 0px; margin-bottom: 0px; color: #FCCA00;}\
  h5 { font-size: 12px; margin-top: 0px; margin-bottom: 0px; color: #FC7100;}\
  h6 { font-size: 12px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}\
  #value { width: 190px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #2ECCFA; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
  <body>\
  <h1>"+SoilSensorname+"</h1>\
  <a><br /></a>\
  <h2>"+SoilSensorname+" Sensorkalibierung</h2>\
  <a><br /></a>\
  <table><tr><td style=\"height: 10px;\">Aktueller Sensorwert:&nbsp;&nbsp;</td>\
  <td id='value'><span>"+String(soilMoist)+"</span></td></tr></table>\
  <a><br /></a>\
  <a><br /></a>\
  <a href =\"/soilhighlevel\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;color: green; font-size: 14px\">Wert f??r 100% Trocken ??bernehmen</button></a>\
  <a href =\"/soillowlevel\"><button class=\"button\" style=\"margin-left: 80px;height: 48px; width: 180px;color: green; font-size: 14px\">Wert f??r 100% Nass ??bernehmen</button></a>\
  <a><br /></a>\
  <a><br /></a>\
  <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;color: black; font-size: 14px\">zur??ck zur Startseite</button></a>\
  <a><br /></a>\
  <a><br /></a>\
  <a><br /></a>\
  <h3>Messintervall: alle 3 Sekunden</h3>\
  <a><br /></a>\
  <h6>Hilfe:</h6>\
  <h4>f??r den Wert von '100% trocken' den h??chsten Wert ??bernehmen.</h4>\
  <h4>f??r den Wert von '100% nass' den niedrigsten Wert ??bernehmen. (Sensor in ein Glas Wasser tauchen)</h4>\
  <h5>Lassen Sie den Sensor mehrere Messintervalle durchlaufen bevor Sie den Wert ??bernehmen !</h5>\
  </body>\
  </html>\
  ");
MyWaitLoop(100); // kurze Pause sonst Absturz bei Browser refresh   
}

/********************************************************************************************************************************************************/

void SensorMoistHighLevel(){
soilMoistLevelHigh = soilMoist; 
soilmoisthighlevel = (String)soilMoistLevelHigh;
SetupSave("soilmoist-highlevel.txt", soilmoisthighlevel); // soil Moist High Level
Serial.println("Sensor-Messwert "+soilmoisthighlevel+" f??r HighLevel (Trocken) ??bernommen und gespeichert"); 
HTMLAnswer("SensorMoistHighLevel");
}

/********************************************************************************************************************************************************/

void SensorMoistLowLevel(){ 
soilMoistLevelLow = soilMoist;  
soilmoistlowlevel = (String)soilMoistLevelLow;  
SetupSave("soilmoist-lowlevel.txt", soilmoistlowlevel); // soil Moist Low Level    
Serial.println("Sensor-Messwert "+soilmoistlowlevel+" f??r LowLevel (Nass) ??bernommen und gespeichert");
HTMLAnswer("SensorMoistLowLevel");
}

/********************************************************************************************************************************************************/

void GetAdjustments()
{
String GAmqtthost,GAmqttusername, GAmqttpassword, GAmqtttopic, MQTTStatus;
if (lmqtthost != "kA" && lmqtthost != ""){GAmqtthost = mqtthost;} else {GAmqtthost = "k.A.";}
if (lmqtt_username != "kA" && lmqtt_username != "") {GAmqttusername = mqtt_username;} else {GAmqttusername = "k.A.";}
if (lmqtt_password != "kA" && lmqtt_password != ""){GAmqttpassword = "**********";} else {GAmqttpassword = "k.A.";}
if (mqtttopic != "kA" && mqtttopic != "") {GAmqtttopic = mqtttopic;} else {GAmqtttopic = "k.A.";}
String LEDMessureFlash1, LEDMessureFlash2;
if (MessureLEDFlash == true) {LEDMessureFlash1 = "EIN*";} else {LEDMessureFlash1 = "EIN";} 
if (!MessureLEDFlash) {LEDMessureFlash2 = "AUS*";} else {LEDMessureFlash2 = "AUS";} 
if (!IsMQTT) {MQTTStatus = "Der MQTT-Broker ist deaktiviert";} else {MQTTStatus ="<br />";}
server.send(200, "text/html", 
  "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
  body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 42px; margin-top: 0px; margin-bottom: 50px; }\
  h3 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; }\
  h4 { font-size: 24px; margin-top: 20px; margin-bottom: 30px; }\
  h5 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}\
  h6 { font-size: 18px; margin-top: 0px; margin-bottom: 0px; color: lime;}\
  h7 { font-size: 14px; margin-top: 10px; margin-bottom: 10px; color: #EB4034;}\
  h8 { font-size: 12px; margin-top: 0px; margin-bottom: 0px; color: white;}\
  h9 { font-size: 14px; margin-left: 3px; margin-top: 0px; margin-bottom: 50px; color: white;}\
  #value {width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #9AFE2E; background-color: #000000; font-size: 60px;}\
  </style>\
  </head>\
    <body>\
     <h1>"+SoilSensorname+"</h1>\
     <h9>Version: "+PMVersion+"</h9>\
     <h4>Einstellungen aus dem Captive Portal.</h4>\
     <h3>WLAN-SSID:  &nbsp;&nbsp;&nbsp;"+(String)ssid+"</h3>\
     <h5>IP-Adresse: &nbsp;&nbsp;&nbsp;"+(String)ip+"</h5>\
     <h3><br /></h3>\
     <h3>MQTT-Host:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+GAmqtthost+"</h3>\
     <h3>MQTT-Host Username:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+GAmqttusername+"</h3>\
     <h3>MQTT-Host Passwort:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+GAmqttpassword+"</h3>\
     <h3>MQTT-Topic:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+GAmqtttopic+"</h3>\
     <h3><br /></h3>\
     <h3>Sensorname:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+SoilSensorname+"</h3>\
     <h3>Sensor-Wert f??r 100% Trocken:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+soilmoisthighlevel+"</h3>\
     <h3>Sensor-Wert f??r 100% Nass:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+soilmoistlowlevel+"</h3>\
     <h3>Messintervall:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;alle "+measurementinterval+" Minuten</h3>\
     <h3><br /></h3>\
     <h7>"+MQTTStatus+"</h7>\
     <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
     <h6>Auswahl - LED Blinken bei Messung &nbsp; EIN / AUS:</h6>\
     <a href =\"/mledflashein\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+LEDMessureFlash1+"</button></a>\
     <a href =\"/mledflashaus\"><button class=\"button\" style=\"margin-right: 10px;height: 26px; width: 90px;\">"+LEDMessureFlash2+"</button></a>\
     <hr style=\"border-top: 3px solid #095484;margin-left: 0px;width: auto;\">\
     <h3><br /></h3>\
     <h3>Sensor-Laufzeit: &nbsp;&nbsp;"+runtime()+"</h3>\
     <h3><br /></h3>\
     <h3><br /></h3>\
     <h3><br /></h3>\
     <a href =\"/esprestart\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;color: green; font-size: 14px\">Sensor neu starten</button></a>\
     <a href =\"/reset\"><button class=\"button\" style=\"margin-left: 50px;height: 48px; width: 180px;color: red; font-size: 14px\">Zur??cksetzen! (Reset)</button></a>\
     <a><br /></a>\
     <a><br /></a>\
     <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px\">zur??ck zur Startseite</button></a>\
     <a><br /></a>\
     <a><br /></a>\
     <a><br /></a>\
     <h8>*) - aktuelle Auswahl</h8>\
  </body>\
    </html>"); 
Serial.println("MQTT- und Sensor-Zugangsdaten aufgerufen");  
}

/********************************************************************************************************************************************************/

void MessureLEDFlashEIN() {
MessureLEDFlash = true; // Bei Messung LED Flash  
if (littleFSInit == true) {
HTMLAnswer("MessureLEDFlashEIN1");  
Serial.println("Die RGB-LED blinkt kurz bei der Messung");     
SetupSave("flashledbymeasurement.txt", "True");  
} else {
HTMLAnswer("MessureLEDFlashEIN2");  
Serial.println("Die RGB-LED blinkt kurz bei der Messung - Die Auswahl konnte nicht gespeichert werden!!!");}     
}

/********************************************************************************************************************************************************/

void MessureLEDFlashAUS() {
MessureLEDFlash = false; // Bei Messung LED Flash  
if (littleFSInit == true) {
HTMLAnswer("MessureLEDFlashAUS1");  
Serial.println("Die RGB-LED blinkt nicht kurz bei der Messung");     
SetupSave("flashledbymeasurement.txt", "False");  
} else {
HTMLAnswer("MessureLEDFlashAUS2");  
Serial.println("Die RGB-LED blinkt nicht kurz bei der Messung - Die Auswahl konnte nicht gespeichert werden!!!");} 
}

/********************************************************************************************************************************************************/

void ESPRestart(){
// ESP neu starten
HTMLAnswer("ESPRestart");
RestartNow = true; // Neu starten initialisieren   
Serial.println("Der Sensor wird neu gestartet");       
}

/********************************************************************************************************************************************************/

void ResetSettings()
{
server.send(200, "text/html", 
 "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
  body { background-color: #585858; font-size: 32px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 20px; margin-left: 10px; margin-top: 0px; margin-bottom: 50px; }\
  h3 { font-size: 18px; margin-left: 180px; margin-top: 0px; margin-bottom: 50px; color: #2ECCFA;}\
  #value { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #9AFE2E; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
    <body>\
    <h1>"+SoilSensorname+"</h1>\
    <a><br /></a>\
    <h2>WLAN-Zugangsdaten werden gel??scht. (Die Parameter bleiben erhalten)</h2>\
    <h3>Jetzt WLAN-Zugangsdaten l??schen ?</h3>\
    <a href =\"/goreset\"><button class=\"button\" style=\"margin-left: 190px;height: 28px; width: 90px; color: red\">JA</button></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 100px;height: 28px; width: 90px\">NEIN</button></a>\
  </body>\
    </html>");
}

/********************************************************************************************************************************************************/

void ResetYes()
{ /*Weiterleiter - <meta http-equiv=\"refresh\" content=\"3\" URL=\"http://www.google.de\">\     <meta http-equiv=\"refresh\" content=\"3\"; URL=\"http://"+ip1+"\">\*/
HTMLAnswer("ResetYes");
ResetNow = true; // Reset initialisieren      
}

/********************************************************************************************************************************************************/
/********************************************************************************************************************************************************/
