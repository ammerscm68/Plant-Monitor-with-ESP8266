/*******************************************************************************************
 ** Autor: Mario Ammerschuber                                                             **
 ** Datum: Juli 2022                                                                      **
 ** Version: 1.0.8                                                                       ** 
 ** Lizenz:  Creative Commons Zero v1.0 Universal                                         **
 *******************************************************************************************
*/

/************************( Importieren der Bibliotheken )************************/

#include <ESP8266WiFi.h>                    // über Bibliothek installieren
#include <Timezone.h>                       
#include <TimeLib.h>                        
#include <ESP8266WebServer.h>               // Webserver - über Bibliothek installieren
#include <WiFiClient.h>                     // WLAN Client - über Bibliothek installieren
#include <ESP8266HTTPClient.h>              // HTTPClient - über Bibliothek installieren
#include "LittleFS.h"                       // Little Filesystem - über Bibliothek installieren
#include <WiFiUdp.h>
#include <WiFiManager.h>                    // Bibliothek "WiFiManager" by tablatronix [Version 2.0.3-alpha]  or  [2.0.5-beta ]
#include <string.h>
#include <PubSubClient.h>                   // MQTT Client - über Bibliothek installieren
#include <ESP8266mDNS.h>                    // DNS Auflösung - über Bibliothek installieren
#include <LEAmDNS.h>
#include <LEAmDNS_lwIPdefs.h>
#include <LEAmDNS_Priv.h>

// LED's
#define GRUEN 2 // NodeMCU v3 D4 -> GPIO 2
#define BLAU 0  // NodeMCU v3 D3 -> GPIO 0
#define ROT 4   // NodeMCU v3 D2 -> GPIO 4

// MQTT Timeout
#define MQTT_KEEPALIVE 650 // keep trying various numbers in sec.
#define MQTT_TIMEOUT 30 // keep trying various numbers in sec.

// Messinterval
// #####################################################################
#define MQTT_TRIGGERVALUE 600 // Messung alle 600 sek. (alle 10 Minuten)
// #####################################################################

ESP8266WebServer server(80); // Webserver auf Port 80
// Variable für den HTTP Request
String header;

WiFiClient wificlient; // WLAN Client 
PubSubClient client(wificlient); // Instanz von MQTT Client
#define MSG_BUFFER_SIZE  (5)
char msg[MSG_BUFFER_SIZE];

const char* ssid = "";     // SSID of local network 
String lssid = ""; // SSID of local network 
const char* password = "";   // Password on network 
String lpassword = ""; // SSID of local network 

const char* mqtt_username = ""; // MQTT-Host Username
String lmqtt_username = ""; // MQTT-Host Username
const char* mqtt_password = ""; // MQTT-Host Passwort
String lmqtt_password = ""; // MQTT-Host Passwort
int MQTTTrigger = MQTT_TRIGGERVALUE / 60;

// MQTT-Daten
const int soilMoistPin = A0; // Analog PIN
int soilMoistLevelLow = 832;    //Dieser Wert soll von euch entsprechend angepasst werden
int soilMoistLevelHigh = 382;   //Dieser Wert soll von euch entsprechend angepasst werden

const char* mqtthost = ""; // MQTT-Host
String lmqtthost = "";  // MQTT-Host
String mqttsensorname = ""; // MQTT-Sensorname
String mqtttopic = ""; // MQTT-Topic
String soilmoisthighlevel = ""; // soil Moist High Level
String soilmoistlowlevel = ""; // soil Moist Low Level
int percentSoilMoist = 0;
String LastMessureDateTime = "k.A. / k.A."; // letzte Messung

// for WiFiManager
char WMmqtthost[50];
char WMmqtthostusername[50];
char WMmqtthostpassword[50];
char WMmqttsname[30];
char WMmqtttopic[80];
char WMSMLH[4];
char WMSMLL[4];

// NTP Serverpool für Deutschland:
static const char ntpServerName[] = "de.pool.ntp.org";    //Finde lokale Server unter http://www.pool.ntp.org/zone/de
const int timeZone = 0;                     // 0 wenn mit <Timezone.h> gearbeitet wird !
WiFiUDP Udp;
unsigned int localPort = 1701; // lokaler Port zum Abhören von UDP-Paketen
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

// - Timezone. - //
// Bearbeiten Sie diese Zeilen entsprechend Ihrer Zeitzone und Sommerzeit.
// TimeZone Einstellungen Details https://github.com/JChristensen/Timezone
TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Time (Frankfurt, Paris)
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Time (Frankfurt, Paris)
Timezone CE(CEST, CET);

WiFiManager wifiManager; // Instanz von WiFiManager
/********************( Definieren der Globale Variablen und Konstanten)********************/
bool WiFiConnectFail = false; // false standard
bool MQTTConnectFail = false; // false standard
String date;

String Datum, Zeit;
String DateString;
String ip;

bool bNTPStarted = false;
bool gbIsConnecting = false;
bool await = false;
bool littleFSInit = true;

unsigned long dotTime = 0;
unsigned long hwsTime = 0;
int dx = 0;
int dy = 0;
byte del = 0;
int h, m, s, w, mo, ye, d;
String FormatedDay = "";
String FormatedMonth = "";
const char* wochenstring;
float utcOffset = 2;
long localEpoc = 0;
long localMillisAtUpdate = 0;
int waitloop = 0;
int toggle = 0;
int WiFiConnectLoop = 0;
int WiFiConnectLoop2 = 1;
int MQTTConnectLoop = 0;
int MQTTConnectLoop2 = 1;
int MaxWiFiConnectLoop = 30;  // Maximale Anzahl Loops bei Verbindung mit dem WLAN
int MaxMQTTConnectLoop = 3;   // Maximale Anzahl Loops bei Verbindung mit dem MQTT-Host
bool StartFail = false;
int StartStopServers = 0;
int APStart = 0;
int LoadValue = 0;
bool ProgramSetup = true;

//flag for saving data WiFiManager
bool shouldSaveConfig = false;
 
//callback notifying us of the need to save config WiFiManager
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

/*****************************************( Setup )****************************************/
void setup()
{
  Serial.begin(115200); // Serielle Verbindung mit 115200 Baud
  delay(250); //waits for data
  clearscreen(); // Serial Monitor clear screen

  // Pins als Output festlegen.
  pinMode(ROT, OUTPUT);
  pinMode(BLAU, OUTPUT);
  pinMode(GRUEN, OUTPUT);

  // Check LED's funktion
  Serial.println("Check RGB LED ...");
  SwitchLEDLight(false,false,false); // alle aus
  for (int i=0; i < 6; i++){BlinkLED(ROT);} // rot
  for (int i=0; i < 6; i++){BlinkLED(BLAU);} // blau
  for (int i=0; i < 6; i++){BlinkLED(GRUEN);} // grün
  SwitchLEDLight(false,false,false); // alle aus
  Serial.println("");
  
   LoadAdjustments(); // Plant Monitor Einstellungen laden ....

    // ***First Start***
     // Little-Filesystem formatieren
     // LittleFS.format(); // alle Dateien  danch wieder deaktivieren und nochmal hochladen
     // wifiManager.resetSettings(); // alle Einstellungen zurücksetzen

  // WiFiManager initialisieren
  Serial.println("WiFiManager (Accees Point) initialisieren ");
  if ((String)ssid == ""){wifiManager.resetSettings();} // alle Einstellungen zurücksetzen
  // wifiManager.setCountry("DE"); // Ländereinstellung
  wifiManager.setTimeout(650); // Nach 650 sekunden AccessPoint abschalten
  wifiManager.setConnectTimeout(60);
  // wifiManager.setSaveConnect(false); // do not connect, only save
  wifiManager.setConfigPortalTimeout(600); //sets timeout until configuration portal gets turned off - useful to make it all retry or go to sleep in seconds
  // wifiManager.setDarkMode(false); // Dunkler Hintergrund
  wifiManager.setScanDispPerc(true);  // show scan RSSI as percentage, instead of signal stength graphic
  wifiManager.setBreakAfterConfig(true); // needed to use saveWifiCallback
  // wifiManager.setParamsPage(true); // move params to seperate page, not wifi, do not combine with setmenu!
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  // wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0)); // andere IP als die aktuelle
    
  MyWaitLoop(1000);

  if ((String)ssid == "") {
    Serial.println("");
    Serial.println("***** First Start *****"); // erster Start der Uhr
    Serial.println("");
    }

  if ((String)ssid != ""){
  WiFi.begin(ssid, password);
  Serial.print("Verbinde mit WiFi SSID: ");
  Serial.println(ssid);
  wificonnect(true); // WLAN Verbindung herstellen
  } else {WiFiConnectFail = true;} // beim ersten Start der Uhr die WLAN-Anmeldung umgehen ! - Access Point gleich starten
  
  if (WiFiConnectFail == false)
  { 
  // vergebene IP-Adresse auf Matrix anzeigen 
  ip = "IP-Adresse:  "+(String)WiFi.localIP()[0]+"."+(String)WiFi.localIP()[1]+"."+(String)WiFi.localIP()[2]+"."+(String)WiFi.localIP()[3]+"                  "; // Ermittelte IP-Adresse
  Serial.println(ip); // Anzeige IP-Adresse
  MyWaitLoop(10); // kurze Pause

  // Webserver - Ausgabe wenn Link nicht gefunden wurde
  server.onNotFound([](){
  server.send(404, "text/plain", "Link wurde nicht gefunden!");  
  });
  
  // Webserver - Startseite Sensorwert
  server.on("/", handleRoot);
  // Einstellungen anzeigen
  server.on("/getadjustments", GetAdjustments); 
  // Einstellungen löschen - Reset
  server.on("/reset", ResetSettings); 
  server.on("/goreset", ResetYes); 

  Serial.println("UDP-Server starten ...");
  WiFiUDPStart();  // UPD Server starten
  
  // Start DNS Server  - call with "mqttsensorname.local" (Example: Plant.local)
  Serial.println("DNS- und Web-Server starten ...");
  if (!MDNS.begin(mqttsensorname)) {
  Serial.println("Error setting up MDNS responder!");
  while (1) {MyWaitLoop(1000);}}
  Serial.println("mDNS Responder gestartet - URL: "+mqttsensorname+".local");
  // Start TCP (HTTP) server
  WebServerStart();
  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);  

    // Get all information of your LittleFS
    if (littleFSInit == true)
    {
    Serial.println(F("Little Filesystem Init - done."));
    FSInfo fs_info;
    LittleFS.info(fs_info);
    Serial.println("File sistem info.");
    Serial.print("Total space:      ");
    Serial.print(fs_info.totalBytes);
    Serial.println("byte");
    Serial.print("Total space used: ");
    Serial.print(fs_info.usedBytes);
    Serial.println("byte");
    Serial.print("Block size:       ");
    Serial.print(fs_info.blockSize);
    Serial.println("byte");
    Serial.print("Page size:        ");
    Serial.print(fs_info.totalBytes);
    Serial.println("byte");
    Serial.print("Max open files:   ");
    Serial.println(fs_info.maxOpenFiles);
    Serial.print("Max path length:  ");
    Serial.println(fs_info.maxPathLength);
    Serial.println();
    // Open dir folder
    Dir dir = LittleFS.openDir("/");
    // Cycle all the content
    while (dir.next()) {
        // get filename
        Serial.print(dir.fileName());
        Serial.print(" - ");
        // If element have a size display It else write 0
        if(dir.fileSize()) {
            File f = dir.openFile("r");
            Serial.println(f.size());
            f.close();
        }else{
            Serial.println("0");
        }   
    }
   Serial.println("_______________________________");
   Serial.println("");
   }
   else
     {
     Serial.println("Little Filesystem Init - Fehler.");      
     }
     
  // Init MQTT Client
  client.setServer(mqtthost, 1883); // MQTT Host setzen
  client.setKeepAlive(MQTT_KEEPALIVE);
  client.setSocketTimeout(MQTT_TIMEOUT); 
  client.setCallback(callback); // MQTT CallBack
  
  SwitchLEDLight(false,false,true); // LED Grün
  MyWaitLoop(1000);
  SwitchLEDLight(false,false,false); // alle LED's aus

  Serial.println("Startzeit: "+FormatedDateTime());
  Serial.println("");
  Serial.println("");
  
  /*Serial.println("geladene Parameter");
  Serial.println("[Loaded-WIFI] SSID: " + (String)ssid); // Only for Debuging 
  Serial.println("[Loaded-WIFI] PASS: " + (String)password); // Only for Debuging 
  Serial.println("[Loaded- MQTT] MQTT-Host: " + (String)mqtthost); // Only for Debuging 
  Serial.println("[Loaded- MQTT] MQTT-Host Username: " + (String)mqtt_username); // Only for Debuging 
  Serial.println("[Loaded- MQTT] MQTT-Host Password: " + (String)mqtt_password); // Only for Debuging 
  Serial.println("[Loaded- MQTT] MQTT-Name: " + mqttsensorname); // Only for Debuging 
  Serial.println("[Loaded- MQTT] MQTT-Topic: " + mqtttopic);  // Only for Debuging  
  Serial.println("[Loaded-soilMoist] soilMoistLevelHigh: " + soilmoisthighlevel);  // Only for Debuging 
  Serial.println("[Loaded-soilMoist] soilMoistLevelLow: " + soilmoistlowlevel);  // Only for Debuging*/
  }
  else
  { 
  /*Serial.println("[Loaded-WIFI] SSID: " + (String)ssid); // Only for Debuging 
  Serial.println("[Loaded-WIFI] PASS: " + (String)password); // Only for Debuging 
  Serial.println("[Loaded- MQTT] MQTT-Host: " + (String)WMmqtthost); // Only for Debuging
  Serial.println("[Loaded- MQTT] MQTT-Host Username: " + (String)WMmqtthostusername); // Only for Debuging
  Serial.println("[Loaded- MQTT] MQTT-Host Password: " + (String)WMmqtthostpassword); // Only for Debuging 
  Serial.println("[Loaded- MQTT] MQTT-Name: " + (String)WMmqttsname); // Only for Debuging 
  Serial.println("[Loaded- MQTT] MQTT-Topic: " + (String)WMmqtttopic);  // Only for Debuging  
  Serial.println("[Loaded-soilMoist] soilMoistLevelHigh: " + (String)WMSMLH);  // Only for Debuging 
  Serial.println("[Loaded-soilMoist] soilMoistLevelLow: " + (String)WMSMLL);  // Only for Debuging*/
     
  if ((String)ssid != ""){ 
  Serial.println("WLAN-Verbindung beim Start des Sensors fehlgeschlagen!"); 
  SwitchLEDLight(true,false,false); // LED Rot
  MyWaitLoop(1000);
  SwitchLEDLight(false,false,false); // alle LED's aus 
  MyWaitLoop(100); // kurze Pause
  WiFiConnectLoop2 += 1;} 
  else {
  Serial.println("Starte Access Point ... [Plant-Monitor]"); 
  Serial.println("");
  SwitchLEDLight(false,true,false); // Blau
  WiFiManagerParameter WMMQTTHost("WMmqtthost", "MQTT-Host",WMmqtthost, 50);
  WiFiManagerParameter WMMQTTHostUserName("WMmqtthostusername", "MQTT-Host Username (Optional)",WMmqtthostusername, 50);
  WiFiManagerParameter WMMQTTHostPassword("WMmqtthostpasswort", "MQTT-Host Password (Optional)",WMmqtthostpassword, 50);
  WiFiManagerParameter WMMQTTName("WMmqttsname", "Sensorname (Example: Kitchenplant)",WMmqttsname, 30);
  WiFiManagerParameter WMMQTTTopic("WMmqtttopic", "MQTT-Topic (Example: /room/kitchen/planthumidity",WMmqtttopic, 80);
  WiFiManagerParameter WMMoistLevelHigh("WMSMLH", "Sensor-Value for 100% Dry (Example: 856)",WMSMLH, 4);
  WiFiManagerParameter WMMoistLevelLow("WMSMLL", "Sensor-Value for 100% Wet (Example: 382)",WMSMLL, 4);
  wifiManager.addParameter(&WMMQTTHost); // MQTT-Host
  wifiManager.addParameter(&WMMQTTHostUserName); // MQTT-Host Username
  wifiManager.addParameter(&WMMQTTHostPassword); // MQTT-Host Password
  wifiManager.addParameter(&WMMQTTName);  // Sensor-Name
  wifiManager.addParameter(&WMMQTTTopic);  // MQTT-Topic
  wifiManager.addParameter(&WMMoistLevelHigh); // soil Moist High Level
  wifiManager.addParameter(&WMMoistLevelLow); // soil Moist Low Level
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  wifiManager.autoConnect("Plant-Monitor",""); // SSID,  SSID-Passwort , MQTT-Daten
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  strcpy(WMmqtthost,WMMQTTHost.getValue()); // MQTT-Host
  strcpy(WMmqtthostusername,WMMQTTHostUserName.getValue()); // MQTT-Host Username
  strcpy(WMmqtthostpassword,WMMQTTHostPassword.getValue()); // MQTT-Host Password
  strcpy(WMmqttsname,WMMQTTName.getValue()); // Sensor-Name
  strcpy(WMmqtttopic,WMMQTTTopic.getValue());  // MQTT-Topic
  strcpy(WMSMLH,WMMoistLevelHigh.getValue()); // soil Moist High Level
  strcpy(WMSMLL,WMMoistLevelLow.getValue()); // soil Moist Low Level
  if (shouldSaveConfig && littleFSInit == true){
    
  SaveAdjustments(); // Eingaben im FileSystem speichern
  // Reset auslösen
  Serial.println("Sensor wird neu gestartet"); 
  ESP.restart();  
  }
   else
   { // Zugangsdaten wurden nicht gespeichert
  Serial.println("Zugangsdaten wurden nicht gespeichert !");   
   // Reset auslösen 
  Serial.println("Sensor wird neu gestartet"); 
  ESP.restart();}}}
  ProgramSetup = false; // Setup beendet 
  }

/*************************************(Hauptprogramm)**************************************/

void loop()
{
  // Check ob Internetverbindung noch besteht
  WiFi.mode(WIFI_STA);
  if (WiFi.status() != WL_CONNECTED) 
  {
  wificonnect(false); // WLAN-Verbindung herstellen
  if (WiFiConnectFail == true)
  { // Wenn WiFi Connection fail
  WiFiConnectLoop2 += 1; 
  if (WiFiConnectLoop2 > 25) {
  WiFiUDPStop(); // UDP Server stoppen  
  // Nach 25 durchläufen Access Point starten
  WiFiConnectLoop2 = 1;
  
  // Change Status LED's
  SwitchLEDLight(true,true,true); // Alle An
  MyWaitLoop(1500);
  SwitchLEDLight(false,false,false); // Alle Aus
  // Little-Filesystem formatieren
  ResetPlantSensor(); // Reset Sensor
  Serial.println("");
  Serial.println("Plant-Monitor wird neu gestartet");
  ESP.restart();}}} else {
  if (!client.connected()) {
  MQTTConnect();
  if (MQTTConnectFail == true){ 
  if (MQTTConnectLoop2 < 3){
  MQTTConnectLoop2 += 1;} // MQTT Verbindung   
  else {BlinkLED(ROT);}}}} // Fail
  
  // nur wenn WLAN-Verbindung besteht
  if (WiFiConnectFail == false && MQTTConnectFail == false)
  {
  if (StartFail) {Serial.println("Sensor wird neu gestartet..."); ESP.restart();} // Sensor neu starten    
  WiFiConnectLoop2 = 1; // Zurücksetzen   
  MQTTConnectLoop2 = 1; // Zurücksetzen   
  HandleWebServer();  // Webserver Abfrage
  MDNS.update(); // DNS Abfrage
  getTimeLocal(); // aktuelle Uhrzeit holen
  LoadValue += 1;
  if (LoadValue == 1 || LoadValue == MQTT_TRIGGERVALUE)
  {
  SwitchLEDLight(false,false,true); // Grün 
  client.loop(); // Client Loop init 
  int soilMoist = analogRead(soilMoistPin);
  Serial.print("Analog Value: ");
  Serial.print(soilMoist);
  // Auswertung der Bodenfeuchtigkeit in Prozent
  percentSoilMoist = map(soilMoist, soilMoistLevelHigh+1, soilMoistLevelLow-1, 0, 100);
  // percentSoilMoist = map(soilMoist, 856,323 , 0, 100); // Only for Debuging
  Serial.print("\t"); 
  LastMessureDateTime = FormatedDateTime(); // Datum / Uhrzeit - letzte Messung
  Serial.print("Feuchte: "+String(percentSoilMoist)+"%  -  "+LastMessureDateTime);
  snprintf (msg, MSG_BUFFER_SIZE, "%ld", percentSoilMoist); // Wert formatieren
  Serial.println("");
  
  Serial.println("Sending to MQTT-HOST... ");
  client.publish(mqtttopic.c_str(), msg); // Wert an MQTT-HOST schicken  (Topic Example: /room/kitchen/planthumidity)
  MyWaitLoop(500);
  digitalWrite(ROT, LOW); 
   digitalWrite(BLAU, LOW);
    digitalWrite(GRUEN, LOW); 
  LoadValue = 1;   // Zurücksetzen
  }
  } else {BlinkLED(ROT);} // rot 
MyWaitLoop(500); // 0,5 sekunden  
} // Loop - Ende
