#include <DHT.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
#define MQPIN 34

char auth[] = "VOTRE_TOKEN_BLYNK_SENSOR";
char ssid[] = "ReseauCapteur";
char pass[] = "motdepasseCapteur";

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(MQPIN, INPUT);
  
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int mqValue = analogRead(MQPIN);
  
  if (!isnan(h) && !isnan(t)) {
    Blynk.virtualWrite(V0, t); // Virtual Pin 0 - Temp
    Blynk.virtualWrite(V1, h); // Virtual Pin 1 - Hum
    Blynk.virtualWrite(V2, mqValue); // Virtual Pin 2 - Air Quality
  }
  
  delay(2000);
}
