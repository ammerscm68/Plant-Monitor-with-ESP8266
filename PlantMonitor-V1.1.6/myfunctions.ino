// ****************************Hilfsfunktionen******************************************************
void clearscreen() { 
for(int i=0; i<10; i++) {
Serial.println("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
}

// *************************************************************************************************

void LoadAdjustments(){
// *** Einstellungen laden .... ***

// WEMOS D1 Little-Filesystem aktivieren
    if (LittleFS.begin()){littleFSInit = true;}
    else{littleFSInit = false;}
    MyWaitLoop(250); //waits for data

if (littleFSInit == true)
{
  Serial.println("Load Plant-Sensor Adjustments ...");
  // Zugangsdaten WiFi laden
  if (LittleFS.exists("/ssid.txt")){
  lssid = SetupLoad("ssid.txt");} else {lssid = "";}
  lssid.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/ssid-passwd.txt")){
  lpassword = SetupLoad("ssid-passwd.txt");} else {lpassword = "";}
  lpassword.trim(); // Leerzeichen vorne und Hinten entfernen
  ssid = lssid.c_str();
  password = lpassword.c_str();
  // MQTT Daten laden
  if (LittleFS.exists("/mqtt-host.txt")){
  lmqtthost = SetupLoad("mqtt-host.txt");} else {lmqtthost = "";}
  lmqtthost.trim(); // Leerzeichen vorne und Hinten entfernen
  mqtthost = lmqtthost.c_str();
  if (LittleFS.exists("/mqtt-hostusername.txt")){
  lmqtt_username = SetupLoad("mqtt-hostusername.txt");} else {lmqtt_username = "";}
  lmqtt_username.trim(); // Leerzeichen vorne und Hinten entfernen
  mqtt_username = lmqtt_username.c_str();
  if (LittleFS.exists("/mqtt-hostpassword.txt")){
  lmqtt_password = SetupLoad("mqtt-hostpassword.txt");} else {lmqtt_password = "";}
  lmqtt_password.trim(); // Leerzeichen vorne und Hinten entfernen
  mqtt_password = lmqtt_password.c_str();
  if (LittleFS.exists("/soilmoist-sensorname.txt")){
  SoilSensorname = SetupLoad("soilmoist-sensorname.txt");} else {SoilSensorname = "";}
  SoilSensorname.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/mqtt-topic.txt")){
  mqtttopic = SetupLoad("mqtt-topic.txt");} else {mqtttopic = "";}
  mqtttopic.trim(); // Leerzeichen vorne und Hinten entfernen
  if (LittleFS.exists("/soilmoist-highlevel.txt")){
  soilmoisthighlevel = SetupLoad("soilmoist-highlevel.txt");} else {soilmoisthighlevel = "";}
  soilmoisthighlevel.trim(); // Leerzeichen vorne und Hinten entfernen
  soilMoistLevelHigh = soilmoisthighlevel.toInt(); // Wert in Integer umwandeln
  if (LittleFS.exists("/soilmoist-lowlevel.txt")){
  soilmoistlowlevel = SetupLoad("soilmoist-lowlevel.txt");} else {soilmoistlowlevel = "";}
  soilmoistlowlevel.trim(); // Leerzeichen vorne und Hinten entfernen
  soilMoistLevelLow = soilmoistlowlevel.toInt(); // Wert in Integer umwandeln
  if (LittleFS.exists("/measurementinterval.txt")){
  measurementinterval = SetupLoad("measurementinterval.txt");} else {measurementinterval = "";}
  measurementinterval.trim(); // Leerzeichen vorne und Hinten entfernen
  measurementinterval1 = measurementinterval.toInt(); // Wert in Integer umwandeln

  if (SoilSensorname == "kA" || SoilSensorname == "") {SoilSensorname = "Pflanze";} // wenn kein Sensorname angegeben wurde
  
  // for WiFi-Manager
  if (lmqtthost != "kA" && lmqtthost != ""){
  lmqtthost.toCharArray(WMmqtthost, 50);} else {IsMQTT = false;}  // MQTT-Host
  if (lmqtt_username != "" && lmqtt_password != "" && lmqtt_username != "kA" && lmqtt_password != "kA" && IsMQTT != false) {
  lmqtt_username.toCharArray(WMmqtthostusername, 50);  // MQTT-Host Username
  lmqtt_password.toCharArray(WMmqtthostpassword, 50);}  // MQTT-Host Password
  if (SoilSensorname != "") { 
  SoilSensorname.toCharArray(WMsoilsname, 30);} else {WMsoilsname[30] = 'Pflanze';} // Sensor-Name
  if (mqtttopic != "kA" && mqtttopic != "") {
  mqtttopic.toCharArray(WMmqtttopic, 80);} else {IsMQTT = false;} // MQTT-Topic
  if (soilmoisthighlevel != "") {
  soilmoisthighlevel.toCharArray(WMSMLH, 4);} else {WMSMLH[4] = '856';} // soilMoistLevelHigh
  if (soilmoistlowlevel != "") {
  soilmoistlowlevel.toCharArray(WMSMLL, 4);} else {WMSMLL[4] = '382';} // soilMoistLevelLow
  if (measurementinterval != "") {
  measurementinterval.toCharArray(WMMessureInt, 3);} else {WMMessureInt[2] = '10';} // Messintervall
  

  /*Serial.println("[Loaded-WIFI] SSID: " + (String)ssid); // Only for Debuging 
  Serial.println("[Loaded-WIFI] PASS: " + (String)password); // Only for Debuging 
  Serial.println("[Loaded- MQTT] MQTT-Host: " + (String)mqtthost); // Only for Debuging 
  Serial.println("[Loaded- MQTT] MQTT-Host Username: " + (String)mqtt_username); // Only for Debuging 
  Serial.println("[Loaded- MQTT] MQTT-Host Password: " + (String)mqtt_password); // Only for Debuging 
  Serial.println("[Loaded- MQTT] MQTT-Topic: " + mqtttopic);  // Only for Debuging  
  Serial.println("[Loaded- soilMoist] Sensor-Name: " + SoilSensorname); // Only for Debuging 
  Serial.println("[Loaded-soilMoist] soilMoistLevelHigh: " + soilmoisthighlevel);  // Only for Debuging 
  Serial.println("[Loaded-soilMoist] soilMoistLevelLow: " + soilmoistlowlevel);  // Only for Debuging
  Serial.println("[Loaded-soilMoist] Messurement Interval: " + measurementinterval);  // Only for Debuging*/
} else
    {
     // Standardwerte setzen bei "Little FS" Fehler
    ssid = "";
    password = ""; 
    mqtthost = "";
    SoilSensorname = "";
    mqtttopic = "";
    soilmoisthighlevel = "";
    soilmoistlowlevel = "";
    }  
}

// ************************************************************************************************************************

void SaveAdjustments(){
String sassid, sapasswd, samqtthost, samqtthostusername, samqtthostpassword, samqttname, samqtttopic, saWMSMLH, saWMSMLL, saWMMessure;  
int saWMMessureInt;
Serial.println(""); 
Serial.println("Save Plant-Monitor Adjustments ..."); 
sassid = (String)wifiManager.getWiFiSSID();
sassid.trim();
sapasswd = (String)wifiManager.getWiFiPass();
sapasswd.trim();
samqtthost = (String)WMmqtthost;
samqtthost.trim();
samqtthostusername = (String)WMmqtthostusername;
samqtthostusername.trim();
samqtthostpassword = (String)WMmqtthostpassword;
samqtthostpassword.trim();
samqttname = (String)WMsoilsname;
samqttname.trim();
samqtttopic = (String)WMmqtttopic;
samqtttopic.trim();
saWMSMLH = (String)WMSMLH;
saWMSMLH.trim();
saWMSMLL = (String)WMSMLL;
saWMSMLL.trim();
saWMMessure = (String)WMMessureInt;
saWMMessure.trim();
/*Serial.println("[Save-WIFI] SSID: " + sassid);  // Only for Debuging 
Serial.println("[Save-WIFI] PASS: " + sapasswd);   // Only for Debuging 
Serial.println("[Save-MQTT] MQTT-Host: " + samqtthost);  // Only for Debuging 
Serial.println("[Save-MQTT] MQTT-Topic: " + samqtttopic); // Only for Debuging
Serial.println("[Save-soilMoist] Sensor-Name: " + samqttname);  // Only for Debuging  
Serial.println("[Save-soilMoist] soilMoistLevelHigh: " + saWMSMLH);  // Only for Debuging  
Serial.println("[Save-soilMoist] soilMoistLevelLow: " + saWMSMLL);  // Only for Debuging
Serial.println("[Save-soilMoist] Messurement Interval: " + saWMMessureInt);  // Only for Debuging*/ 

// Plausi Prüfung
if (samqtthost == "") {samqtthost = "kA";} // MQTT-Host 
if (samqtthostusername == "") {samqtthostusername = "kA";}
if (samqtthostpassword == "") {samqtthostpassword = "kA";}
if (samqttname == "") {samqttname = "kA";} // Sensorname
if (samqtttopic == ""){samqtttopic = "kA";} else {if (samqtttopic.substring(0,1) != "/") {samqtttopic = "/"+samqtttopic;}} // Topic ev. korrigieren Begin with -->  /
if (!IsNumeric(saWMSMLH)) {saWMSMLH = "856";} // Default - Prüfen ob soilMoistLevelHigh eine Zahl ist
if (!IsNumeric(saWMSMLL)) {saWMSMLL = "382";} // Default - Prüfen obsoilMoistLevelLow eine Zahl ist
if (!IsNumeric(saWMMessure)) {saWMMessureInt = 10;} else {saWMMessureInt = saWMMessure.toInt();} // Prüfen ob Messinterval eine Zahl ist

if (sassid != "" && sapasswd != "") {
   // Messintervall begrenzen
  if (saWMMessureInt < 1 && saWMMessureInt > 60) {saWMMessureInt = 10;} // Default = 10 Minuten
  // Daten speichern
  SetupSave("ssid.txt", (String)wifiManager.getWiFiSSID()); // SSID WLAN
  SetupSave("ssid-passwd.txt", (String)wifiManager.getWiFiPass()); // Password WLAN
  SetupSave("mqtt-host.txt", samqtthost); // MQTT-Host
  SetupSave("mqtt-hostusername.txt", samqtthostusername); // MQTT-Host Username
  SetupSave("mqtt-hostpassword.txt", samqtthostpassword); // MQTT-Host Password
  SetupSave("mqtt-topic.txt", samqtttopic);  // Topic-Name
  SetupSave("soilmoist-sensorname.txt", samqttname); // Sensor-Name
  SetupSave("soilmoist-highlevel.txt", saWMSMLH); // soil Moist High Level
  SetupSave("soilmoist-lowlevel.txt", saWMSMLL); // soil Moist Low Level
  SetupSave("measurementinterval.txt", (String)saWMMessureInt); // Messurement Interval
  MyWaitLoop(500); // 0,5 sek. warten 
  Serial.println("Zugangsdaten gespeichert... "); 
  for (int i=0; i <= 5; i++){BlinkLED(GRUEN);} // grün
} else
  {
// Daten unvollständig
 Serial.println("Daten unvollständig -  AccessPoint wird neu gestartet"); 
 for (int i=0; i <= 10; i++){BlinkLED(ROT);} // rot
 // Little-Filesystem formatieren
 LittleFS.format();
 wifiManager.resetSettings(); // alle Einstellungen zurücksetzen   
  }
}

// *************************************************************************************************

// Setup Einstellungen laden und speichern
String SetupLoad(String file_name) {
String result = ""; // init
  File this_file = LittleFS.open(file_name, "r");
  if (!this_file) { // failed to open the file, retrn empty result
    return result;
  }
  while (this_file.available()) {
      result += (char)this_file.read();
  }
  this_file.close();
  MyWaitLoop(10); //wait
  return result;
}

// *************************************************************************************************

bool SetupSave(String file_name, String contents) {  
  File this_file = LittleFS.open(file_name, "w");
  if (!this_file) { // failed to open the file, return false
    return false;
  }
  int bytesWritten = this_file.print(contents);
 
  if (bytesWritten == 0) { // write failed
      return false;
  }
  this_file.close();
  MyWaitLoop(10); //wait
  return true;
}

// *************************************************************************************************

void GetSoilMoistValue(int MinValue) {
soilMoist = analogRead(soilMoistPin);
// Auswertung der Bodenfeuchtigkeit in Prozent
if (soilMoist > MinValue) {
percentSoilMoist = map(soilMoist, soilMoistLevelHigh+1, soilMoistLevelLow-1, 0, 100);  
SwitchLEDLight(false,false,true); // Grün 
MyWaitLoop(150);
  digitalWrite(ROT, LOW); 
   digitalWrite(BLAU, LOW);
    digitalWrite(GRUEN, LOW);} else {
percentSoilMoist = 0; // kein Messwert
SwitchLEDLight(true,false,false); // Rot
MyWaitLoop(150);
  digitalWrite(ROT, LOW); 
   digitalWrite(BLAU, LOW);
    digitalWrite(GRUEN, LOW);} 
// percentSoilMoist = map(soilMoist, 856,323 , 0, 100); // Only for Debuging
LastMessureDateTime = FormatedDateTime(); // Datum / Uhrzeit - letzte Messung 
if (!SoilCalibrate) { 
Serial.println("Sensor Analog Value: "+(String)soilMoist+" - Feuchte: "+String(percentSoilMoist)+"%  -  "+LastMessureDateTime);} else {
Serial.println("Sensor Kalibrierung - Analog Value: "+ (String)soilMoist + " - "+LastMessureDateTime);}
MyWaitLoop(150);
}

// *************************************************************************************************

void StartMessureTimer() {
if (StartMessureCount > 0) {
StartMessureCount--;
MCount = StartMessureCount +1;
Serial.println("Startmodus - Noch für "+(String)MCount+" Messungen");}
if (StartMessureCount == 0) {
StartMessureCount = -1;  
soilMessureTrigger = measurementinterval1 * 120; // Messintervall von Minuten in Sekunden umrechnen  - 120 wenn Loop 0,5 sek.
Serial.println("");
Serial.println("Startmodus beendet");
Serial.println("");
}
}

// *************************************************************************************************

// check a string to see if it is numeric and accept Decimal point
boolean IsNumeric(String str)
{
if(str.length()<1){return false;}
bool bPoint=false;
for(unsigned char i = 0; i < str.length(); i++)
{
if ( !(isDigit(str.charAt(i)) || str.charAt(i) == '.' )|| bPoint) {return false;}
if(str.charAt(i) == '.'){bPoint=true;};
}
return true;
}

// *************************************************************************************************

String FormatedDateTime()
{
String DateTimeString;
getTimeLocal(); // aktuelle Uhrzeit holen
// Uhrzeit
if (h < 10){DateTimeString = "0"+String(h); } else {DateTimeString = String(h);}
if (m < 10){DateTimeString += ":0"+String(m);} else {DateTimeString += ":"+String(m);}
if (s < 10){DateTimeString += ":0"+String(s)+" Uhr / ";} else{DateTimeString += ":"+String(s)+" Uhr / ";}
// Datum
if (d < 10){DateTimeString += "0"+String(d);} else {DateTimeString += String(d);}
if (mo < 10){DateTimeString += ".0"+String(mo)+"."+String(ye);} else {DateTimeString += "."+String(mo)+"."+String(ye);}
return DateTimeString;    
}

// *************************************************************************************************

// Webserverabfrage nur jede 0,5 sek.
void HandleWebServer()
{ 
if (millis() - hwsTime > 500) 
 { 
 hwsTime = millis();
 server.handleClient();  // Webserver Abfrage
 delay(1); // kurze Pause 
 } 
}

// *************************************************************************************************

void wificonnect(bool Start) 
{
WiFiConnectLoop = 0; // Variable zurücksetzen  
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) 
{ 
MyWaitLoop(500);
Serial.print("."); 
BlinkLEDWifiConnect();
WiFiConnectLoop += 1;
if (WiFiConnectLoop >= MaxWiFiConnectLoop) {break;}
}
if (WiFi.status() != WL_CONNECTED){
WiFiConnectFail = true;
if (!Start) {Serial.println("WLAN-Verbindung getrennt - "+(String)WiFiConnectLoop2);} else {StartFail = true;} 
} else
{
WiFiConnectFail = false;
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Serial.print("WiFi Connect-Loop: "+String(WiFiConnectLoop)+"/"+String(MaxWiFiConnectLoop)); // Zeit bis Connected
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Serial.println("");
LoadValue = 0;   // Zurücksetzen
Serial.print("Verbunden mit IP-Adresse: "); Serial.println(WiFi.localIP());
}
}

// *************************************************************************************************

void MQTTConnect()
{
MQTTConnectLoop = 0; // Variable zurücksetzen 
String client_id = "esp8266-client-"; 
client_id += String(WiFi.macAddress())+" ("+SoilSensorname+")";
Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());  
while(!client.connected())
{ 
client.connect(client_id.c_str(), mqtt_username, mqtt_password); 
MyWaitLoop(1000);
Serial.print("."); 
BlinkLEDMQTTConnect(); // Blau/Rot
MQTTConnectLoop += 1;
if (MQTTConnectLoop >= MaxMQTTConnectLoop) {break;}
}
if (!client.connected()) {
MQTTConnectFail = true;
SwitchLEDLight(true,false,false); // Rot
Serial.println("MQTT-Verbindung: getrennt/fehlgeschlagen");
Serial.println("");
} else
{
MQTTConnectFail = false;
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Serial.print("MQTT Connect-Loop: "+String(MQTTConnectLoop)+"/"+String(MaxMQTTConnectLoop)); // Zeit bis Connected
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Serial.println("");
LoadValue = 0;   // Zurücksetzen
Serial.println("Mit MQTT verbunden");
Serial.println("");
Serial.println("Messintervall: alle "+measurementinterval+" Minuten");
Serial.println("");
Serial.println("");
}
}

// *************************************************************************************************

/*void MQTTConnect()
{
while (!client.connected()) {
String client_id = "esp8266-client-";
client_id += String(WiFi.macAddress());
Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
} else 
{
Serial.print("failed with state ");
Serial.print(client.state());
MyWaitLoop(2000);
    }
}
}*/

// *************************************************************************************************

void WiFiUDPStart()
{ 
Udp.begin(localPort);
Serial.print("lokaler Port: ");
Serial.println(Udp.localPort());
Serial.println("NTP-Time - Warten auf die Synchronisation");
setSyncProvider(getNtpTime);
setSyncInterval(86400); // Anzahl der Sekunden zwischen dem erneuten Synchronisieren ein. 86400 = 1 Tag
// setSyncInterval(60); // Test
Serial.println("UDP Server gestartet - "+FormatedDateTime());  
}

// *************************************************************************************************

void WiFiUDPStop()
{
Udp.stop();
Serial.println("UDP Server gestoppt - "+FormatedDateTime());   
}

// *************************************************************************************************

void WebServerStart()
{
server.begin();
Serial.println("Webserver gestartet - "+FormatedDateTime());  
}

// *************************************************************************************************

void WebServerStop()
{
server.stop();
Serial.println("Webserver gestoppt - "+FormatedDateTime()); 
}

// *************************************************************************************************

void MyWaitLoop(int wlt)
{// Pause Loop
waitloop = 0; // Variable zurücksetzen
while (waitloop < wlt) // Pause in Millisekunden
{ 
waitloop += 1; // inc waitloop 1
HandleWebServer();  // Webserver Abfrage
delay(1);
yield(); // Equivalent zu  Application.ProcessMessages; 
// Serial.println(String(waitloop));
}
}

// *************************************************************************************************

void BlinkLED(int farbe) {
 toggle = !toggle;
 if (toggle) {
 if (farbe == ROT) {
 SwitchLEDLight(true,false,false); // Rot
 }
 if (farbe == BLAU) {
 SwitchLEDLight(false,true,false); // Blau
 }
 if (farbe == GRUEN) {
 SwitchLEDLight(false,false,true); // Grün
 }
  } else 
     {
 digitalWrite(ROT, LOW); 
 digitalWrite(BLAU, LOW);
 digitalWrite(GRUEN, LOW); 
      } 
MyWaitLoop(500); // kurze Pause 
}

// *************************************************************************************************

void BlinkLEDWifiConnect() {
toggle = !toggle;
 if (toggle) {
 SwitchLEDLight(false,true,false); // Blau
 }
  else 
     {
  SwitchLEDLight(false,false,true); // Grün
      } 
 delay(1); // kurze Pause
}

// *************************************************************************************************

void BlinkLEDMQTTConnect() {
toggle = !toggle;
 if (toggle) {
 SwitchLEDLight(false,true,false); // Blau
 }
  else 
     {
  SwitchLEDLight(true,false,false); // Rot
      } 
 delay(1); // kurze Pause
}


// *************************************************************************************************

void SwitchLEDLight(bool TLRed, bool TLBlue, bool TLGreen){
if (TLRed){digitalWrite(ROT, HIGH);} else {digitalWrite(ROT, LOW);}
if (TLBlue){digitalWrite(BLAU, HIGH);} else {digitalWrite(BLAU, LOW);}
if (TLGreen){digitalWrite(GRUEN, HIGH);} else {digitalWrite(GRUEN, LOW);}
}

// *************************************************************************************************

void ResetPlantSensor()
{
APStart = 0; // zurücksetzen
bool IsFileExist = false; // zurücksetzen
// Little-Filesystem formatieren
LittleFS.format();
wifiManager.resetSettings(); // alle Einstellungen zurücksetzen

SetupSave("mqtt-host.txt", mqtthost); // MQTT-Host
if (mqtt_username != "" && mqtt_password != ""){ 
SetupSave("mqtt-hostusername.txt", mqtt_username); // MQTT-Host Username
SetupSave("mqtt-hostpassword.txt", mqtt_password);} // MQTT-Host Password
SetupSave("soilmoist-sensorname.txt", SoilSensorname); // Sensor-Name
if (mqtttopic.substring(0,1) != "/") {mqtttopic = "/"+mqtttopic;} // Topic ev. korrigieren Begin with -->  /
SetupSave("mqtt-topic.txt", mqtttopic); // Topic-Name
SetupSave("soilmoist-highlevel.txt", soilmoisthighlevel); // soil Moist High Level
SetupSave("soilmoist-lowlevel.txt", soilmoistlowlevel); // soil Moist Low Level
SetupSave("measurementinterval.txt", measurementinterval); // Messurement Interval

MyWaitLoop(500); // 0,5 sek. warten 
Serial.println("Reset - Sensor wird neu gestartet");
ESP.restart();   
}

// *************************************************************************************************

String cleanText(String text) {
  text.replace("’", "'");
  text.replace("“", "\"");
  text.replace("”", "\"");
  text.replace("`", "'");
  text.replace("‘", "'");
  text.replace("„", "'");
  text.replace("\\\"", "'");
  text.replace("•", "-");
  text.replace("é", "e");
  text.replace("è", "e");
  text.replace("ë", "e");
  text.replace("ê", "e");
  text.replace("à", "a");
  text.replace("â", "a");
  text.replace("ù", "u");
  text.replace("ç", "c");
  text.replace("î", "i");
  text.replace("ï", "i");
  text.replace("ô", "o");
  text.replace("…", "...");
  text.replace("–", "-");
  text.replace("Â", "A");
  text.replace("À", "A");
  text.replace("æ", "ae");
  text.replace("Æ", "AE");
  text.replace("É", "E");
  text.replace("È", "E");
  text.replace("Ë", "E");
  text.replace("Ô", "O");
  text.replace("Ö", "Oe");
  text.replace("ö", "oe");
  text.replace("œ", "oe");
  text.replace("Œ", "OE");
  text.replace("Ù", "U");
  text.replace("Û", "U");
  text.replace("Ü", "Ue");
  text.replace("ü", "ue");
  text.replace("Ä", "Ae");
  text.replace("ä", "ae");
  text.replace("ß", "ss");
  text.replace("»", "'");
  text.replace("«", "'");
  return text;
}

// *************************************************************************************************
// *************************************************************************************************
