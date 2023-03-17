#include <Arduino.h>
#include <iostream>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "*********";
const char* password = "********";
const char* mqtt_server = "********"; // MQTT broker
const char* input = "/worldtime"; // nombre del topic input
const char* output = "/output";  // nombre del topic output
const char* alive = "/alive";  // nombre de topic de inicio de conexion al broker
const char* StatusRequest = "/StatusRequest";
const char* Json = "/JsonStatus";

WiFiClient espClient; 
PubSubClient client(espClient); 

void wifi_setup(){ // conexión red wifi
  WiFi.begin(ssid, password);
  Serial.print ("Connecting to AP");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect() {  // conexión con el MQTT broker
  while (!client.connected()) {
    if (client.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT broker");
      client.subscribe(input);
    
    } else {
      Serial.println("Failed with state ");
      Serial.print(client.state());
      Serial.println("Retrying...");
      delay(100);
    }
  }
}

void formato_fecha (const int day_of_week, const char* datetime){ // formato de fecha
    String dia = "";
    switch (day_of_week) {
    case 1:
      dia = "Lunes";
      break;
    case 2:
      dia = "Martes";
      break;
    case 3:
      dia = "Miércoles";
      break;
    case 4:
      dia = "Jueves";
      break;
    case 5:
      dia = "Viernes";
      break;
    case 6:
      dia = "Sábado";
      break;
    case 0:
      dia = "Domingo";
      break;
    default:
      break;
    }

    char fechaMes[1]={0x00};
    for(int i=0;i<1;i++)
      fechaMes[0]=(char)datetime[i+6];
    fechaMes[1] = 0x00;  

    String mes = "";
    switch (int(fechaMes[0])) {
    case 49:
      mes = "Enero";
      break;
    case 50:
      mes = "Febrero";
      break;
    case 51:
      mes = "Marzo";
      break;
    case 52:
      mes = "Abril";
      break;
    case 53:
      mes = "Mayo";
      break;
    case 54:
      mes = "Junio";
      break;
    case 55:
      mes = "Julio";
      break;
    case 56:
      mes = "Agosto";
      break;
    case 57:
      mes = "Septiembre";
      break;
    case 58:
      mes = "Octubre";
      break;
    case 59:
      mes = "Noviembre";
      break;
    case 60:
      mes = "Diciembre";
      break;
    default:
      break;
    }

    char fechaDia[5]={0x00};
    for(int i=0;i<2;i++)
      fechaDia[i]=(char)datetime[i+8];
    fechaDia[2]=0x00;       

    char hora[6]={0x00};
    for(int i=0;i<5;i++)
      hora[i]=(char)datetime[i+11];
    hora[5]=0x00;

    char año[5]={0x00};
    for(int i=0;i<4;i++)
      año[i]=(char)datetime[i];
    año[4]=0x00;

    String fechaprocesada = dia + ", " + fechaDia + " de " + mes + " de " + año + " -- " + hora;
    
    Serial.println(fechaprocesada);
    reconnect();
    client.publish(output,fechaprocesada.c_str());
    return;
}

void get_API (byte* payload, unsigned int length){
  // Make a GET request to the World Time API
  String Url = "http://worldtimeapi.org/api/timezone/";
  HTTPClient http; 
  char mensaje[5]={0x00};
  for(int i=0;i<length;i++)
    mensaje[i]=(char)payload[i];
  mensaje[length]=0x00;

  Url += String(mensaje);

  if(http.begin(espClient, Url)){ // conexión HTTP con el ESP8266 y la url

    int httpCode = http.GET(); // solicitud GET
    if (httpCode > 0 || httpCode == HTTP_CODE_OK) { 
      reconnect();
      client.publish(StatusRequest, "OK");
      String response = http.getString(); 
      Serial.println("Respuesta exitosa");
    
      DynamicJsonDocument doc(1024); //crea el objeto "doc" de memoria 1024 bytes
      deserializeJson(doc, response);
      client.publish(Json, "Trama procesada");

      const char* datetime = doc["datetime"];
      const int day_of_week = doc["day_of_week"];
    
      formato_fecha(day_of_week, datetime);
      return;
      
    }else {
      const String error = http.errorToString(httpCode);
      Serial.println("Error making API call");
      Serial.printf("[HTTP] GET... failed, error: %s", error.c_str());
      Serial.println("");
    }  
  }
  http.end();
  delay(1000);
}

void callback (char* topic, byte* payload, unsigned int length) { 
  Serial.print("Incomming message from: ");
  Serial.println(topic);
  get_API(payload,length);
}

void setup() {
  Serial.begin(115200); 
  wifi_setup();
  client.setServer(mqtt_server, 1883); 
  client.setCallback(callback);
  
  reconnect();
  client.subscribe(input);
  client.publish(Json, "Trama procesada"); 
  client.publish(alive, "Conexión Exitosa al Broker");
}

void loop() {
  reconnect();
  client.loop();
}