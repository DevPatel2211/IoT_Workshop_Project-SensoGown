#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
const char *ssid = "ESP8266";
const char *password = "12345678";
const int wifi = D0;
const int trigPin = D1;
const int echoPin = D2;
long duration;
int distanceCm;
void setup()
{
  Serial.begin(9600);
  pinMode(wifi, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(wifi, HIGH);
    delay(1000);
    digitalWrite(wifi, LOW);
    delay(1000);
  }
  digitalWrite(wifi, LOW);
}
void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.034 / 2;
  HTTPClient http;
  http.begin("http://techvegan.in/pdeu-project/api.php?sensor=" + (String)distanceCm);
  http.GET();
  http.end();
  Serial.println(distanceCm);
  delay(10000);
}
