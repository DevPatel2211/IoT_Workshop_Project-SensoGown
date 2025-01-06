#include <ESP8266WebServer.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define DHTTYPE DHT11
#define DHTPIN 14 // D5 pin= GPIO pin 14
#define DS18B20 2 // D4 pin= GPIO pin 2
#define REPORTING_PERIOD_MS 1000

float temperature, humidity, BPM, SpO2, bodytemperature;

// Wi-Fi credentials
const char *ssid = "Rachi";         // Enter SSID here
const char *password = "rachivyas"; // Enter Password here

DHT dht(DHTPIN, DHTTYPE);
; //--> Initialize DHT sensor, DHT dht(Pin_used, Type_of_DHT_Sensor);
PulseOximeter pox;
uint32_t tsLastReport = 0;
OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);

ESP8266WebServer server(80);

void setup()
{
  Serial.begin(115200);
  pinMode(16, OUTPUT);
  delay(100);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  Serial.println("Connecting to ");
  Serial.println(ssid);

  // Connect to your local Wi-Fi network
  WiFi.begin(ssid, password);

  // Check Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

  Serial.print("Initializing pulse oximeter..");

  if (!pox.begin())
  {
    Serial.println("FAILED");
    for (;;)
      ;
  }
  else
  {
    Serial.println("SUCCESS");
  }
}

void loop()
{
  server.handleClient();
  pox.update();
  sensors.requestTemperatures();

  if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    bodytemperature = sensors.getTempCByIndex(0);

    temperature = t;
    humidity = h;
    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();

    // Print to Serial Monitor
    Serial.print("Room Temperature: ");
    Serial.print(t);
    Serial.println("°C");

    Serial.print("Room Humidity: ");
    Serial.print(h);
    Serial.println("%");

    Serial.print("BPM: ");
    Serial.println(BPM);

    Serial.print("SpO2: ");
    Serial.print(SpO2);
    Serial.println("%");

    Serial.print("Body Temperature: ");
    Serial.print(bodytemperature);
    Serial.println("°C");

    // Send data to the web server
    sendDataToServer(temperature, humidity, BPM, SpO2, bodytemperature);

    Serial.println("*");
    Serial.println();
    tsLastReport = millis();
  }
}

void sendDataToServer(float temperature, float humidity, float BPM, float SpO2, float bodytemperature)
{
  HTTPClient http;

  // Your API URL (change it to your server's URL)
  String url = "http://devkpatel.com/pdeu-project/api.php?temperature=" + String(temperature) +
               "&humidity=" + String(humidity) +
               "&bpm=" + String(BPM) +
               "&spo2=" + String(SpO2) +
               "&bodytemperature=" + String(bodytemperature);

  // Make the GET request to the server
  http.begin(url);
  int httpCode = http.GET();

  // Check the response from the server
  if (httpCode > 0)
  {
    Serial.println("Data sent successfully: " + String(httpCode));
  }
  else
  {
    Serial.println("Error sending data: " + String(httpCode));
  }

  http.end(); // End HTTP request
}

void handle_OnConnect()
{
  String jsonResponse = "{\"temperature\": " + String(temperature) +
                        ", \"humidity\": " + String(humidity) +
                        ", \"BPM\": " + String(BPM) +
                        ", \"SpO2\": " + String(SpO2) +
                        ", \"bodytemperature\": " + String(bodytemperature) + "}";
  server.send(200, "application/json", jsonResponse);
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}