/*
Sensor 1 : 0x28, 0x52, 0x4D, 0x75, 0xD0, 0x01, 0x3C, 0xB2
Sensor 2 : 0x28, 0x65, 0x02, 0x75, 0xD0, 0x01, 0x3C, 0x5C
Sensor 3 : 0x28, 0x15, 0xA2, 0x75, 0xD0, 0x01, 0x3C, 0xF7
*/

#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D3

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

float tempSensor1, tempSensor2, tempSensor3;

uint8_t sensor1[8] = { 0x28, 0x52, 0x4D, 0x75, 0xD0, 0x01, 0x3C, 0xB2  };
uint8_t sensor2[8] = { 0x28, 0x65, 0x02, 0x75, 0xD0, 0x01, 0x3C, 0x5C  };
uint8_t sensor3[8] = { 0x28, 0x15, 0xA2, 0x75, 0xD0, 0x01, 0x3C, 0xF7  };

const char* ssid = "Home Wi-fi 2.4G";   // SSID
const char* password = "Home13579";  // пароль

ESP8266WebServer server(80);             
 
void setup() 
{
  Serial.begin(115200);
  delay(100);
  
  sensors.begin();              

  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() 
{
  server.handleClient();
}

void handle_OnConnect() 
{
  sensors.requestTemperatures();
  tempSensor1 = sensors.getTempC(sensor1); // Получить значение температуры
  tempSensor2 = sensors.getTempC(sensor2); // Получить значение температуры
  tempSensor3 = sensors.getTempC(sensor3); // Получить значение температуры
  server.send(200, "text/html", SendHTML(tempSensor1,tempSensor2,tempSensor3)); 
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float tempSensor1,float tempSensor2,float tempSensor3)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP8266 Temperature Monitor</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>Temperature Monitoring: </h1>\n";
  ptr +="<p>Sensor 1: ";
  ptr +=tempSensor1;
  ptr +="&deg;C</p>";
  ptr +="<p>Sensor 2: ";
  ptr +=tempSensor2;
  ptr +="&deg;C</p>";
  ptr +="<p>Sensor 3: ";
  ptr +=tempSensor3;
  ptr +="&deg;C</p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
