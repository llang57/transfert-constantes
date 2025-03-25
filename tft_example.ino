#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// Définir les broches
#define TFT_CS     5
#define TFT_RST    4
#define TFT_DC     2
#define TFT_SCLK   18
#define TFT_MOSI   23

// Initialiser l'écran
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

unsigned long startTime;
bool radarActive = false;
int radarAngle = 0;

void setup() {
  // Initialiser l'écran
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  
  // Enregistrer le temps de démarrage
  startTime = millis();
  
  // Afficher l'écran initial
  showInitialScreen();
}

void loop() {
  // Vérifier si 30 secondes se sont écoulées
  if (!radarActive && millis() - startTime > 30000) {
    tft.fillScreen(ST7735_BLACK);
    radarActive = true;
  }
  
  // Si le radar est actif, afficher l'animation
  if (radarActive) {
    drawRadar();
    delay(50);
  }
}

void showInitialScreen() {
  tft.fillScreen(ST7735_BLUE);
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("Bonjour!");
  
  tft.drawRect(20, 50, 80, 60, ST7735_RED);
  tft.drawCircle(100, 100, 30, ST7735_GREEN);
  tft.drawTriangle(40, 120, 80, 120, 60, 150, ST7735_WHITE);
  tft.fillRect(30, 60, 60, 40, ST7735_MAGENTA);
  tft.fillCircle(120, 80, 20, ST7735_CYAN);
  tft.drawLine(10, 140, 110, 40, ST7735_ORANGE);
}

void drawRadar() {
  // Effacer l'ancienne ligne
  tft.fillCircle(64, 80, 60, ST7735_BLACK);
  
  // Dessiner le cercle du radar
  tft.drawCircle(64, 80, 60, ST7735_GREEN);
  
  // Calculer les coordonnées de la ligne
  int x2 = 64 + 60 * cos(radarAngle * PI / 180);
  int y2 = 80 + 60 * sin(radarAngle * PI / 180);
  
  // Dessiner la ligne du radar
  tft.drawLine(64, 80, x2, y2, ST7735_GREEN);
  
  // Mettre à jour l'angle
  radarAngle = (radarAngle + 2) % 360;
}
