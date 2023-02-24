#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
const char* ssid = "*********";
const char* password = "*******";

// WebServer Instantiation
ESP8266WebServer server(80);

void handleRoot() {
  // Muestra una página HTML básica con dos botónes para encender y apagar dos LED
  
  String html = "<!DOCTYPE html><html><body>";
  html += "<style>button {";
  html += "background-color: blue;";
  html += "border-color: white;";
  html += "border-width: 5px;";
  html += "border-height: 5px;";
  html += "border-radius: 50px;";
  html += "color: white;";
  html += "padding: 15px 35px;";
  html += "text-align: center;";
  html += "font-size: 18px;";
  html += "margin: 8px 4px;";
  html += "cursor: pointer;";
  html += "}</style>";
  html += "<center>";
  html += "<h1>Practica 1 IoT Gabriel Cano 2023</h1>";
  html += "<h1>LED 1</h1>";
  html += "<a href=\"/led1/on\"><button>ON</button></a>";
  html += "<a href=\"/led1/off\"><button>OFF</button></a>";
  html += "<h1>LED 2</h1>";
  html += "<a href=\"/led2/on\"><button>ON</button></a>";
  html += "<a href=\"/led2/off\"><button>OFF</button></a>";
  html += "</center>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleLed1On() {
  // Enciende el LED1
  digitalWrite(D1, HIGH);
  server.send(200, "text/html", "<center><h1>LED 1 Encendido</h1></center>");
}

void handleLed1Off() {
  // Apaga el LED1
  digitalWrite(D1, LOW);
  server.send(200, "text/html", "<center><h1>LED 1 Apagado</h1></center>");
}

void handleLed2On() {
  // Enciende el LED2
  digitalWrite(D2, HIGH);
  server.send(200, "text/html", "<center><h1>LED 2 Encendido</h1></center>");
}

void handledLed2Off() {
  // Apaga el LED2
  digitalWrite(D2, LOW);
  server.send(200, "text/html", "<center><h1>LED 2 Apagado</h1></center>");
}

void setup() {
  // configura los puertos como salidas
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  // Configura las rutas del servidor web
  server.on("/", handleRoot);
  server.on("/led1/on", handleLed1On);
  server.on("/led1/off", handleLed1Off);
  server.on("/led2/on", handleLed2On);
  server.on("/led2/off", handledLed2Off);

  // Inicia el servidor
  server.begin();
  Serial.println("Servidor iniciado");

  //Serial Comunnication and Wifi Connection kick off
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println(""); 

  // Inicializa la conexión WiFi
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
   //Handle client requests to web Server using .handleClient() method from ESP8266WebServer class
  server.handleClient();
}

