// Webserver Startseite
void handleRoot() {
// &nbsp = Leerzeichen | <meta http-equiv=\"refresh\" content=\"30\">\ = Refresh alle 30 sekunden 
String PersentValue = String(percentSoilMoist); // letzter Messwert in Prozent
String RefreshTime = "";
if (!ResetNow) {RefreshTime = "30";} else {RefreshTime = "3";} // Je nach Status
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
  body { background-color: #585858; font-size: 50px; font-family: Arial, Helvetica, Sans-Serif; color: #F2F2F2; margin-left: 30px; }\
  h1 { color: #2ECCFA; margin-top: 50px; margin-bottom: 0px; }\
  h2 { font-size: 36px; margin-top: 0px; margin-bottom: 50px; }\
  h3 { font-size: 20px; margin-top: 0px; margin-bottom: 50px; }\
  #value { width: 240px; height: 80px; border: 5px solid #F2F2F2; text-align:center; padding: 1px; color: #9AFE2E; background-color: #000000; font-size: 60px; }\
  </style>\
  </head>\
  <body>\
  <h1>"+SoilSensorname+"</h1>\
  <a><br /></a>\
  <h2>Letzte Messung: "+LastMessureDateTime+"</h2>\
  <a><br /></a>\
  <table><tr><td style=\"height: 10px;\">Aktuelle Bodenfeuchte:&nbsp;</td>\
  <td id='value'><span>"+String(percentSoilMoist)+"</span>%</td></tr></table>\
  <a><br /></a>\
  <a><br /></a>\
  <a href =\"/\"><button class=\"button\" style=\"margin-left: 30px;height: 48px; width: 160px; color: black; font-size: 14px\">Aktualisieren</button></a>\
  <a href =\"/getadjustments\"><button class=\"button\" style=\"margin-left: 80px;height: 48px; width: 180px;color: blue; font-size: 14px\">Einstellungen Anzeigen</button></a>\
  <a href =\"/reset\"><button class=\"button\" style=\"margin-left: 100px;height: 48px; width: 180px;color: red; font-size: 14px\">Zurücksetzen! (Reset)</button></a>\
  <a><br /></a>\
  <a><br /></a>\
  <a><br /></a>\
  <h3>Messintervall: alle "+measurementinterval+" Minuten</h3>\
  </body>\
  </html>\
  ");
MyWaitLoop(100); // kurze Pause sonst Absturz bei Browser refresh 
if (littleFSInit == true && ResetNow == true) {
ResetNow = false;  
ResetPlantSensor();} // WLAN Zugangsdaten werden gelöscht    
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void GetAdjustments()
{
String GAmqttusername, GAmqttpassword;
String HTMLString;
if (lmqtt_username != "kA"){GAmqttusername = mqtt_username;} else {GAmqttusername = "k.A.";}
if (lmqtt_password != "kA"){GAmqttpassword = "**********";} else {GAmqttpassword = "k.A.";}
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <h1>+++ Pflanzen-Monitor +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <p style=\"font-family:arial;color:black;font-size: 24px\">Einstellungen aus dem Captive Portal.</p>\
     <a><br /></a>\
     <p style=\"font-family:arial;color:black;font-size: 16px\">WLAN-SSID:  &nbsp;&nbsp;"+(String)ssid+"</p>\
     <p style=\"font-family:arial;color:red;font-size: 16px\">"+(String)ip+"</p>\
     <a><br /></a>\
     <p style=\"font-family:arial;color:black;font-size: 16px\">MQTT-Host:  &nbsp;&nbsp;"+mqtthost+"</p>\
     <p style=\"font-family:arial;color:black;font-size: 16px\">MQTT-Host Username:  &nbsp;&nbsp;"+GAmqttusername+"</p>\
     <p style=\"font-family:arial;color:black;font-size: 16px\">MQTT-Host Passwort:  &nbsp;&nbsp;"+GAmqttpassword+"</p>\
     <p style=\"font-family:arial;color:black;font-size: 16px\">Sensorname:  &nbsp;&nbsp;"+SoilSensorname+"</p>\
     <p style=\"fonMQTT-Topict-family:arial;color:black;font-size: 16px\">MQTT-Topic:  &nbsp;&nbsp;"+mqtttopic+"</p>\
     <p style=\"fonMQTT-Topict-family:arial;color:black;font-size: 16px\">Sensor-Wert für 100% Trocken:  &nbsp;&nbsp;"+soilmoisthighlevel+"</p>\
     <p style=\"fonMQTT-Topict-family:arial;color:black;font-size: 16px\">Sensor-Wert für 100% Nass:  &nbsp;&nbsp;"+soilmoistlowlevel+"</p>\
     <p style=\"fonMQTT-Topict-family:arial;color:black;font-size: 16px\">Messintervall:  &nbsp;&nbsp;alle "+measurementinterval+" Minuten</p>\
     <a><br /></a>\
    <a><br /></a>\
    <a><br /></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 10px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
Serial.println("API-Zugangedaten - OpenweatherMap und NewsAPI");  
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ResetSettings()
{
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
  <style>\
  h2 {font-size: 36px; margin-top: 0px; margin-bottom: 50px;}\
  h3 {font-size: 18px; margin-top: 10px; margin-bottom: 50px; margin-left: 60px;}\
  </style>\
  <h1>+++ Pflanzen-Monitor +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a style=\"font-family:arial;color:black;font-size: 18px\">WLAN-Zugangsdaten werden gelöscht. (Die Parameter bleiben erhalten)</a>\
     <a><br /></a>\
     <a><br /></a>\
     <h3>Jetzt WLAN-Zugangsdaten löschen ?</h3>\
    <a href =\"/goreset\"><button class=\"button\" style=\"margin-left: 70px;height: 28px; width: 90px\">JA</button></a>\
    <a href =\"/\"><button class=\"button\" style=\"margin-left: 80px;height: 28px; width: 90px\">NEIN</button></a>\
  </body>\
    </html>";
server.send(200, "text/html", HTMLString);  
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void ResetYes()
{ /*Weiterleiter - <meta http-equiv=\"refresh\" content=\"3\" URL=\"http://www.google.de\">\     <meta http-equiv=\"refresh\" content=\"3\"; URL=\"http://"+ip1+"\">\*/
String HTMLString;
HTMLString = "<!DOCTYPE html> <html>\
  <html lang='de'>\
  <a style=\"font-family:arial;color:blue;font-size:18px;\">\
  <head>\
  <title>Pflanzen-Monitor</title>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\
   <style>\
  h2 {font-size: 36px; margin-top: 0px; margin-bottom: 50px;}\
  h3 {font-size: 18px; margin-top: 0px; margin-bottom: 0px; margin-left: 0px; font-weight: bold; color: red;}\
  h4 {font-size: 20px; color: red; margin-top: 0px; margin-bottom: 50px; margin-left: 150px; font-weight: bold;}\
  </style>\
  <h1>+++ Pflanzen-Monitor +++</h1>\
  </head>\
    <body>\
    <font face=\"Arial\">\
     <a><br /></a>\
     <h3>Der Access Point (Captive Portal) wird gestartet</h3>\
     <h3>wenn Sie zur Startseite zurückkehren.</h3>\
     <a><br /></a>\
     <a href =\"/\"><button class=\"button\" style=\"margin-left: 0px;height: 28px; width: 160px\">zurück zur Startseite</button></a>\
  </body>\
    </html>";
ResetNow = true; // Reset      
server.send(200, "text/html", HTMLString); 
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
