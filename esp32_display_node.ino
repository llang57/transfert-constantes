#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define TFT_CS     5
#define TFT_RST    4
#define TFT_DC     2
#define TFT_SCLK   18
#define TFT_MOSI   23
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

char auth[] = "VOTRE_TOKEN_BLYNK_DISPLAY";
char ssid[] = "ReseauDisplay";
char pass[] = "motdepasseDisplay";

float currentTemp = 0;
float currentHum = 0;
int currentAirQuality = 0;

BLYNK_WRITE(V0) { currentTemp = param.asFloat(); }
BLYNK_WRITE(V1) { currentHum = param.asFloat(); }
BLYNK_WRITE(V2) { currentAirQuality = param.asInt(); }

void setup() {
  Serial.begin(115200);
  
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);
  
  Blynk.begin(auth, ssid, pass);
  showInitialScreen();
}

void loop() {
  Blynk.run();
  updateDisplay(currentTemp, currentHum, currentAirQuality);
  delay(100);
}

void showInitialScreen() {
  tft.fillScreen(ST7735_BLUE);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, 10);
  tft.println("Attente données...");
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
  
  int barWidth = map(air, 0, 4095, 0, 100);
  tft.fillRect(10, 70, barWidth, 10, ST7735_RED);
}
