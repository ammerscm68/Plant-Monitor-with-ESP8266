// Webserver Startseite
void handleRoot() {
// &nbsp = Leerzeichen | <meta http-equiv=\"refresh\" content=\"30\">\ = Refresh alle 30 sekunden 
if (SoilCalibrate == true) 
{SoilCalibrate = false; 
GetSoilMoistValue(10); // Nach dem Kalibrieren aktuellen Messwert holen  
Serial.println("Sensorkalibrierung beendet ...");}  
String RefreshTime = "";
if (!ResetNow) {RefreshTime = "15";} else {RefreshTime = "3";} // Je nach Status
// Startmodus anzeigen
String MInterval,SMMessureCount;
if (StartMessureCount > 0) {
MInterval = "Messintervall: alle "+(String)(soilMessureTrigger/2)+" Sekunden (Startmodus)";
if (MCount < 0) {SMMessureCount = "Anzahl der Messungen im Startmodus: k.A.";} else {
SMMessureCount = "Anzahl der Messungen im Startmodus: "+ (String)(MCount-1);}
} else {
MInterval = "Messintervall: alle "+measurementinterval+" Minuten";
SMMessureCount = "<br />";} // Nach Startmodus eine Leezeile
// Farbe des Messwertes je nach Wert
String ValueColor;
if (percentSoilMoist < 15) {
ValueColor = "<tr><td style=\"height: 10px;\">Aktuelle Bodenfeuchte:&nbsp;</td><td id='Rvalue'><span>"+String(percentSoilMoist)+"</span>%</td></tr>";  
} else {
if (percentSoilMoist >= 15 && percentSoilMoist <= 60) {ValueColor = "<tr><td style=\"height: 10px;\">Aktuelle Bodenfeuchte:&nbsp;</td><td id='Yvalue'><span>"+String(percentSoilMoist)+"</span>%</td></tr>";} else {  
ValueColor = "<tr><td style=\"height: 10px;\">Aktuelle Bodenfeuchte:&nbsp;</td><td id='Gvalue'><span>"+String(percentSoilMoist)+"</span>%</td></tr>";}
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
  body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 32px; margin-top: 0px; margin-bottom: 10px; }\
  h3 { font-size: 20px; margin-top: 0px; margin-bottom: 10px; }\
  h4 { font-size: 18px; margin-top: 0px; margin-bottom: 10px; }\
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
  <a href =\"/scalibrate\"><button class=\"button\" style=\"margin-left: 50px;height: 48px; width: 180px;color: green; font-size: 14px\">Sensor Kalibieren</button></a>\
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
ResetPlantSensor();} // WLAN Zugangsdaten werden gelöscht    
if (RestartNow == true) {
RestartNow = false;  
Serial.println("Sensor wird neu gestartet");
ESP.restart();} 
}
 

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void SensorCalibrate() {
// &nbsp = Leerzeichen | <meta http-equiv=\"refresh\" content=\"30\">\ = Refresh alle 30 sekunden 
String RefreshTime = "3"; // Site Refresh Time in Sekunden
SoilCalibrate = true;
 yield();
 MyWaitLoop(100); // kurze Pause sonst Absturz bei Browser refresh
 GetSoilMoistValue(10); // Messwert Auslesen
 server.send(200, "text/html", 
  "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv='content-type' content='text/html'; charset='utf-8'>\
  <meta http-equiv=\"refresh\" content=\""+RefreshTime+"\">\
  <style>\
  body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 28px; margin-top: 0px; margin-bottom: 50px; }\
  h3 { font-size: 20px; margin-top: 0px; margin-bottom: 10px; }\
  h4 { font-size: 12px; margin-top: 0px; margin-bottom: 0px; color: #FCCA00;}\
  h5 { font-size: 12px; margin-top: 0px; margin-bottom: 0px; color: #FC7100;}\
  h6 { font-size: 12px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}\
  #value { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #9AFE2E; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
  <body>\
  <h1>"+SoilSensorname+"</h1>\
  <a><br /></a>\
  <h2>"+SoilSensorname+" Sensorkalibierung</h2>\
  <a><br /></a>\
  <table><tr><td style=\"height: 10px;\">Aktueller Sensorwert:&nbsp;</td>\
  <td id='value'><span>"+String(soilMoist)+"</span></td></tr></table>\
  <a><br /></a>\
  <a><br /></a>\
  <a href =\"/soilhighlevel\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;color: green; font-size: 14px\">Wert für 100% Trocken Übernehmen</button></a>\
  <a href =\"/soillowlevel\"><button class=\"button\" style=\"margin-left: 80px;height: 48px; width: 180px;color: green; font-size: 14px\">Wert für 100% Nass Übernehmen</button></a>\
  <a><br /></a>\
  <a><br /></a>\
  <a><br /></a>\
  <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 180px;color: black; font-size: 14px\">zurück zur Startseite</button></a>\
  <a><br /></a>\
  <a><br /></a>\
  <h3>Messintervall: alle 3 Sekunden</h3>\
  <a><br /></a>\
  <h6>Hilfe:</h6>\
  <h4>für den Wert von '100% trocken' den höchsten Wert übernehmen.</h4>\
  <h4>für den Wert von '100% nass' den niedrigsten Wert übernehmen. (Sensor in ein Glas Wasser tauchen)</h4>\
  <h5>Lassen Sie den Sensor mehrere Messintervalle durchlaufen bevor Sie den Wert übernehmen !</h5>\
  </body>\
  </html>\
  ");
MyWaitLoop(100); // kurze Pause sonst Absturz bei Browser refresh   
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void SensorMoistHighLevel(){
soilMoistLevelHigh = soilMoist; 
soilmoisthighlevel = (String)soilMoistLevelHigh;
SetupSave("soilmoist-highlevel.txt", soilmoisthighlevel); // soil Moist High Level
Serial.println("Sensor-Messwert "+soilmoisthighlevel+" für HighLevel (Trocken) übernommen und gespeichert"); 
server.send(200, "text/html", 
  "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv='content-type' content='text/html'; charset='utf-8'>\
  <style>\
  body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 18px; margin-top: 0px; margin-bottom: 50px; }\
  h3 { font-size: 20px; margin-top: 0px; margin-bottom: 50px; }\
  #value { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #9AFE2E; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
  <body>\
  <h1>"+SoilSensorname+"</h1>\
  <a><br /></a>\
  <h2>Der Wert "+soilmoisthighlevel+" wurde für den 100% trockenen Sensor übernommen und gespeichert.</h2>\
  <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 180px;color: black; font-size: 14px\">zurück zur Startseite</button></a>\
  </body>\
  </html>\
  "); 
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void SensorMoistLowLevel(){ 
soilMoistLevelLow = soilMoist;  
soilmoistlowlevel = (String)soilMoistLevelLow;  
SetupSave("soilmoist-lowlevel.txt", soilmoistlowlevel); // soil Moist Low Level    
Serial.println("Sensor-Messwert "+soilmoistlowlevel+" für LowLevel (Nass) übernommen und gespeichert");
server.send(200, "text/html", 
  "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv='content-type' content='text/html'; charset='utf-8'>\
  <style>\
  body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 18px; margin-top: 0px; margin-bottom: 50px; }\
  h3 { font-size: 20px; margin-top: 0px; margin-bottom: 50px; }\
  #value { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #9AFE2E; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
  <body>\
  <h1>"+SoilSensorname+"</h1>\
  <a><br /></a>\
  <h2>Der Wert "+soilmoistlowlevel+" wurde für den 100% nassen Sensor übernommen und gespeichert.</h2>\
  <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 180px;color: black; font-size: 14px\">zurück zur Startseite</button></a>\
  </body>\
  </html>\
  "); 
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void GetAdjustments()
{
String GAmqttusername, GAmqttpassword;
if (lmqtt_username != "kA"){GAmqttusername = mqtt_username;} else {GAmqttusername = "k.A.";}
if (lmqtt_password != "kA"){GAmqttpassword = "**********";} else {GAmqttpassword = "k.A.";}
server.send(200, "text/html", 
  "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
  body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 42px; margin-top: 0px; margin-bottom: 50px; }\
  h3 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; }\
  h4 { font-size: 24px; margin-top: 0px; margin-bottom: 0px; }\
  h5 { font-size: 14px; margin-top: 0px; margin-bottom: 0px; color: #2ECCFA;}\
  #value { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #9AFE2E; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
    <body>\
    <h1>"+SoilSensorname+"</h1>\
    <a><br /></a>\
    <h4>Einstellungen aus dem Captive Portal.</h4>\
     <a><br /></a>\
     <h3>WLAN-SSID:  &nbsp;&nbsp;&nbsp;"+(String)ssid+"</h3>\
     <h5>IP-Adresse: &nbsp;&nbsp;&nbsp;"+(String)ip+"</h5>\
     <h3><br /></h3>\
     <h3>MQTT-Host:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+mqtthost+"</h3>\
     <h3>MQTT-Host Username:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+GAmqttusername+"</h3>\
     <h3>MQTT-Host Passwort:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+GAmqttpassword+"</h3>\
     <h3>MQTT-Topic:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+mqtttopic+"</h3>\
     <h3><br /></h3>\
     <h3>Sensorname:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+SoilSensorname+"</h3>\
     <h3>Sensor-Wert für 100% Trocken:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+soilmoisthighlevel+"</h3>\
     <h3>Sensor-Wert für 100% Nass:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+soilmoistlowlevel+"</h3>\
     <h3>Messintervall:  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;alle "+measurementinterval+" Minuten</h3>\
     <h3><br /></h3>\
     <h3><br /></h3>\
     <h3><br /></h3>\
     <a href =\"/esprestart\"><button class=\"button\" style=\"margin-left: 10px;height: 48px; width: 180px;color: green; font-size: 14px\">Sensor neu starten</button></a>\
     <a href =\"/reset\"><button class=\"button\" style=\"margin-left: 50px;height: 48px; width: 180px;color: red; font-size: 14px\">Zurücksetzen! (Reset)</button></a>\
     <a><br /></a>\
     <a><br /></a>\
     <a><br /></a>\
     <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>"); 
Serial.println("MQTT- und Sensor-Zugangedaten");  
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ESPRestart(){
// ESP neu starten
server.send(200, "text/html", 
 "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
   <style>\
  body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 20px; margin-left: 5px; margin-top: 0px; margin-bottom: 50px; }\
  h3 { font-size: 18px; margin-left: 180px; margin-top: 0px; margin-bottom: 50px; color: #2ECCFA;}\
  #value { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #9AFE2E; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
    <body>\
     <h1>"+SoilSensorname+"</h1>\
     <a><br /></a>\
     <h2>Der Sensor wird neu gestartet wenn Sie zur Startseite zurückkehren.</h2>\
     <a href =\"/\"><button class=\"button\" style=\"margin-left: 0px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>");
RestartNow = true; // Neu starten initialisieren        
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ResetSettings()
{
server.send(200, "text/html", 
 "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
  body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 20px; margin-left: 10px; margin-top: 0px; margin-bottom: 50px; }\
  h3 { font-size: 18px; margin-left: 180px; margin-top: 0px; margin-bottom: 50px; color: #2ECCFA;}\
  #value { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #9AFE2E; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
    <body>\
    <h1>"+SoilSensorname+"</h1>\
    <a><br /></a>\
    <h2>WLAN-Zugangsdaten werden gelöscht. (Die Parameter bleiben erhalten)</h2>\
    <h3>Jetzt WLAN-Zugangsdaten löschen ?</h3>\
    <a href =\"/goreset\"><button class=\"button\" style=\"margin-left: 190px;height: 28px; width: 90px; color: red\">JA</button></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 100px;height: 28px; width: 90px\">NEIN</button></a>\
  </body>\
    </html>");
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ResetYes()
{ /*Weiterleiter - <meta http-equiv=\"refresh\" content=\"3\" URL=\"http://www.google.de\">\     <meta http-equiv=\"refresh\" content=\"3\"; URL=\"http://"+ip1+"\">\*/
server.send(200, "text/html", 
 "<!DOCTYPE html>\
  <html>\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
   <style>\
  body { background-color: #585858; font-size: 36px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 20px; margin-left: 5px; margin-top: 0px; margin-bottom: 50px; }\
  h3 { font-size: 18px; margin-left: 180px; margin-top: 0px; margin-bottom: 50px; color: #2ECCFA;}\
  #value { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #9AFE2E; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
    <body>\
     <h1>"+SoilSensorname+"</h1>\
     <a><br /></a>\
     <h2>Der Access Point (Captive Portal) wird gestartet wenn Sie zur Startseite zurückkehren.</h2>\
     <a href =\"/\"><button class=\"button\" style=\"margin-left: 0px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>");
ResetNow = true; // Reset initialisieren      
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
