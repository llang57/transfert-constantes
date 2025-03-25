#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>

// Configuration TFT
#define TFT_CS     5
#define TFT_RST    4
#define TFT_DC     2
#define TFT_SCLK   18
#define TFT_MOSI   23
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Configuration DHT22
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Configuration MQ-135
#define MQPIN 34

// Configuration Blynk
char auth[] = "YourAuthToken";
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

void setup() {
  Serial.begin(115200);
  
  // Initialiser TFT
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);
  
  // Initialiser capteurs
  dht.begin();
  pinMode(MQPIN, INPUT);
  
  // Connexion Blynk
  Blynk.begin(auth, ssid, pass);
  
  // Afficher écran initial
  showInitialScreen();
}

void loop() {
  Blynk.run();
  
  // Lire capteurs
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int mqValue = analogRead(MQPIN);
  
  // Vérifier les lectures
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // Envoyer données à Blynk
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, mqValue);
  
  // Afficher sur TFT
  updateDisplay(t, h, mqValue);
  
  delay(2000); // Attendre 2s entre les lectures
}

void showInitialScreen() {
  tft.fillScreen(ST7735_BLUE);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, 10);
  tft.println("Initialisation...");
}

void updateDisplay(float temp, float hum, int air) {
  tft.fillScreen(ST7735_BLACK);
  
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(1);
  tft.setCursor(10, 10);
  tft.print("Temp: ");
  tft.print(temp);
  tft.println(" C");
  
  tft.setCursor(10, 30);
  tft.print("Hum: ");
  tft.print(hum);
  tft.println(" %");
  
  tft.setCursor(10, 50);
  tft.print("Air Q: ");
  tft.println(air);
  
  // Barre de qualité d'air
  int barWidth = map(air, 0, 4095, 0, 100);
  tft.fillRect(10, 70, barWidth, 10, ST7735_RED);
}
