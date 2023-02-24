#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
// incluyendo las librerías necesarias para el código
// ==================================================
// Replace with your network credentials
const char* ssid = "XXXXXX";
const char* password = "XXXXXX";

// WebServer Instantiation
ESP8266WebServer server(80);

void handleRoot() {
  // Muestra una página HTML básica con un botón para encender y apagar un LED
  String html = "<html><body>";
  html += "<center"; //Centra lo que se va a ver en la pagina
  html += "<h1 class = titulo>Control de LED</h1>";
  //Dar nombre a los botones y adicionar clases para asi modificarlos.
  html += "<p><a href=\"/led/on1\"><button class = button1>Encender led 1</button></a></p>";
  html += "<p><a href=\"/led/off1\"><button class = button>Apagar led 1</button></a></p>";
  html += "<p><a href=\"/led/on2\"><button class = button1>Encender led 2</button></a></p>";
  html += "<p><a href=\"/led/off2\"><button class = button>Apagar led 2</button></a></p>";
  html += "<style>"; 
  //Poner el diseño del boton que se va usar, color entre cuadro, letra, negrilla, grande de letra 
  html += ".button{"; 
  html += "background-color: red;";
  html += "padding: 15px 25px;";
  html += "font-family: arial black;";
  html += "font-weight: bold;"; 
  html += "font-size: 30px;"; 
  html += "}";
  //Poner el diseño del boton que se va usar, color entre cuadro, letra, negrilla, grande de letra 
  html += ".button1{";
  html += "background-color: green;";
  html += "padding: 15px 25px;";
  html += "font-family: arial black;";  
  html += "font-weight: bold;"; 
  html += "font-size: 30px;"; 
  html += "}";
  //Poner el diseño del boton que se va usar, color entre cuadro, letra, negrilla, grande de letra 
  html += ".titulo {";
  html += "text-align: center;";
  html += "font-family: arial black;";
  html += "font-weight: bold;";
  html += "font-size: 30px;";
  html += "color:black;";
  html += "}";
  html += "</style>";
  html += "</center>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleLedOn1() {
  // Enciende el LED
  digitalWrite(D1, HIGH);
  server.send(200, "text/plain", "LED encendido");
}

void handleLedOff1() {
  // Apaga el LED
  digitalWrite(D1, LOW);
  server.send(200, "text/plain", "LED apagado");
}

void handleLedOn2() {
  // Enciende el LED
  digitalWrite(D2, HIGH);
  server.send(200, "text/plain", "LED encendido");
}

void handleLedOff2() {
  // Apaga el LED
  digitalWrite(D2, LOW);
  server.send(200, "text/plain", "LED apagado");
}

void setup() {

  pinMode(D1, OUTPUT);

  // Configura las rutas del servidor web
  server.on("/", handleRoot);
  server.on("/led/on1", handleLedOn1);
  server.on("/led/off1", handleLedOff1);
  

  pinMode(D2, OUTPUT);

  // Configura las rutas del servidor web
  server.on("/", handleRoot);
  server.on("/led/on2", handleLedOn2);
  server.on("/led/off2", handleLedOff2);

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

//Fragmento que ejecuta el server
//================================================================================================
  server.handleClient();
//================================================================================================
}
