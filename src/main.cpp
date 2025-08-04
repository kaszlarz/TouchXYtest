#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Waveshare_ILI9486.h>

// =======================
// Kolory 16-bit (RGB565)
// =======================
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

unsigned long lastRotationChange = 0;
uint8_t rot = 0;



// =======================
// LCD - alias GFX
// =======================
namespace {
  Waveshare_ILI9486 Waveshield; // domyślne piny SPI z biblioteki
  Adafruit_GFX &tft = Waveshield; // alias
}

void mapTouchToRotation(int16_t &x, int16_t &y) {
    uint8_t rot = tft.getRotation();
    int16_t tempX = x;
    int16_t tempY = y;
    int16_t w = tft.width();
    int16_t h = tft.height();

    switch (rot) {
        case 0: // pionowo
            // bez zmian
            break;

        case 1: // poziomo
            x = w - tempX;
            y = h - tempY;
            break;

        case 2: // pionowo odwrotnie
            x = tempX;
            y = tempY;
            break;

        case 3: // poziomo odwrotnie
            x = w - tempX;
            y = h - tempY;
            break;
    }
}


void setup() {
   
    // ----- LCD -----
    Waveshield.begin();
    tft.fillScreen(CYAN); // czyść ekran przy każdej zmianie
}

void loop() {
   


    unsigned long now = millis();

    // co 5 sekund zmieniaj rotację
    if (now - lastRotationChange >= 5000) {
        rot = (rot + 1) % 4;  // 0,1,2,3
        tft.setRotation(rot);
        tft.fillScreen(BLACK); // czyść ekran przy każdej zmianie
        tft.setCursor(0, 0);
        tft.setTextColor(CYAN, BLACK);
        tft.setTextSize(3);
        tft.print("Rotation: ");
        tft.println(rot);

        lastRotationChange = now;
    }

    // obsługa dotyku i rysowanie
    TSPoint p = Waveshield.getPoint();
    if (p.z > 100) {
      Waveshield.normalizeTsPoint(p);
        int16_t px = p.x;
        int16_t py = p.y;
        mapTouchToRotation(p.x,p.y); // korekta wg rotacji
        tft.fillRect(p.x, p.y, 5, 5, YELLOW);
    }
}



