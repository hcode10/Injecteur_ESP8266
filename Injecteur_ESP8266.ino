#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// DÃ©finition des constantes
#ifndef STASSID
#define STASSID "Freebox-5ABA8B"
#define STAPSK  "ipod95110"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const int led = LED_BUILTIN;
const int inj1 = D1;
const int inj2 = D2;
const int inj3 = D3;
const int inj4 = D4;
const double tempsInjection = 0.003;

// Le serveur Web "ecoute" sur le port 80 (port HTTP par dÃ©faut)
ESP8266WebServer server(80);


/****************************************************************************/
void handleRoot() {
  int val;

  // Lecture de l'Ã©tat du relais
  val = 0;//digitalRead(relayPin);

  // En-tÃªte et titre de la page
  String s = "<!DOCTYPE HTML>\r\n<html><head><title>Relais ";
  s += (val)?"ON":"OFF";  // Affiche ON ou OFF selon l'Ã©tat du relais
  s += "</title></head><body style=\"";
  s += "margin:auto; background-color:white; width:100%;\">\r\n";
  s += "<h1 style=\"text-align:center; font-size:300%;\">ESP8266 D1 Mini</h1>\r\n";

  // Bouton ON / OFF
  s += "<br/><br/><div><a style=\"display:block; height:70px; background-color:";
  s += (val)?"red":"DarkRed";   // La couleur du bouton change selon l'Ã©tat du relais
  s += "; color:white; text-align:center; padding:20px; margin:20px; font-size:300%; text-decoration: none;\" href=\"http://";
  s += WiFi.localIP().toString();
  s += "/setgpio/";
  s += (val)?"0":"1";
  s += "\">M&eacute;thode 1</a></div>\r\n";

  s += "<br/><br/><div><a style=\"display:block; height:70px; background-color:";
  s += (val)?"red":"DarkRed";   // La couleur du bouton change selon l'Ã©tat du relais
  s += "; color:white; text-align:center; padding:20px; margin:20px; font-size:300%; text-decoration: none;\" href=\"http://";
  s += WiFi.localIP().toString();
  s += "/setgpio/";
  s += (val)?"0":"1";
  s += "\">M&eacute;thode 2</a></div>\r\n";

  s += "<br/><br/><div><a style=\"display:block; height:70px; background-color:";
  s += (val)?"red":"DarkRed";   // La couleur du bouton change selon l'Ã©tat du relais
  s += "; color:white; text-align:center; padding:20px; margin:20px; font-size:300%; text-decoration: none;\" href=\"http://";
  s += WiFi.localIP().toString();
  s += "/setgpio/";
  s += (val)?"0":"1";
  s += "\">M&eacute;thode 3</a></div>\r\n";

  // Informations de connexion (SSID et force du signal)
  s += "<p style=\"margin:20px;\">Connect&eacute; &agrave; : ";
  s += ssid;
  s += " - Force du signal : ";
  long rssi = WiFi.RSSI();
  s += rssi; 
  s += " dBm</p>";

  s += "</body></html>\n";

  // Envoie de la page (Le code HTTP 200 indique que la requÃªte a Ã©tÃ© traitÃ©e avec succÃ¨s
  server.send(200, "text/html", s);
}
/****************************************************************************/
void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}
/****************************************************************************/
void setup(void) {
  // Initialisation GPIO led et relais en sorties
  pinMode(led, OUTPUT);
  pinMode(inj1, OUTPUT);
  pinMode(inj2, OUTPUT);
  pinMode(inj3, OUTPUT);
  pinMode(inj4, OUTPUT);
  
  digitalWrite(led, HIGH);
  digitalWrite(inj1, LOW);
  digitalWrite(inj2, LOW);
  digitalWrite(inj3, LOW);
  digitalWrite(inj4, LOW);

  // Initialisation port sÃ©rie Ã  115 200 bauds
  Serial.begin(115200);

  // Connection au point d'accÃ¨s WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Affichage des paramÃ¨tres de connexion (point d'accÃ¨s et IP obtenue par DHCP)
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Quand un client se connecte sur l'URL / on appelle la fonction handleRoot()
  server.on("/", handleRoot);

  // Quand un client se connecte sur l'URL /setgpio/1
  server.on("/setgpio/1", []() {
    //digitalWrite(relayPin, HIGH); // Activation de relais
    handleRoot();                 // appel de la fonction handleRoot()
  });

  // Quand un client se connecte sur l'URL /setgpio/0
  server.on("/setgpio/0", []() {
    //digitalWrite(relayPin, LOW);  // DÃ©sactivation de relais
    handleRoot();                 // appel de la fonction handleRoot()
  });

  // Si le client se connecte sur une URL non programmÃ©e
  server.onNotFound(handleNotFound);  // appel de la fonction handleNotFound()

  // DÃ©marrage du serveur
  server.begin();
  Serial.println("HTTP server started");
}
/****************************************************************************/
void loop(void) {
  server.handleClient();  // Attente de la connexion d'un client
}



void test1(){
  int run = 3;
  int repos = 17;

  unsigned long starttime = millis();
  unsigned long endtime = starttime;

  while ((endtime - starttime) <=10000) // do this loop for up to 10000mS
  {
   // code here

   digitalWrite(inj1, HIGH);
   digitalWrite(inj2, HIGH);
   digitalWrite(inj3, HIGH);
   digitalWrite(inj4, HIGH);
   delay(tempsInjection*1000); 
   digitalWrite(inj1, LOW);
   digitalWrite(inj2, LOW);
   digitalWrite(inj3, LOW);
   digitalWrite(inj4, LOW);
   delay(repos);

   endtime = millis();
  }

}


int calculRepos(int toursMN){
  int cycle = toursMN/60;
  Serial.print("Cycle = ");
  Serial.print(cycle);
  Serial.print(" par secondes  ");
   
  int tempsTotalInjection = (tempsInjection*cycle)*1000;//*1000 pour convertir en millisecondes 
  Serial.print("Temps total injection par seconde = ");
  Serial.print(tempsTotalInjection);
  Serial.println("ms");

  int reposTotal = 1000-tempsTotalInjection;//1000 = 1secondes
  Serial.print("Temps repos total = ");
  Serial.print(reposTotal);
  Serial.println("ms");


  int reposparCycle = reposTotal/cycle;
  Serial.print("Temps repos par cycle = ");
  Serial.print(reposparCycle);
  Serial.println("ms");

  return reposparCycle;
}
