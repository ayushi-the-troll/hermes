#include <Wire.h>
#include "rpr0521rs.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Sensor object
RPR0521RS sensor;

void setup() {
  Serial.begin(115200);

  // I2C pin config for XIAO RP2040
  Wire.setSDA(4);  // D4
  Wire.setSCL(5);  // D5
  Wire.begin();

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 OLED not found");
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("OLED Initialized");
  display.display();
  delay(1000);

  // Sensor init
  if(sensor.init() != 0) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("RPR init fail");
    display.display();
    while(1);
  }
}

void loop() {
  uint16_t ps, als;
  if(sensor.measure_psals(&ps, &als) == 0) {
    Serial.print("Proximity: "); Serial.print(ps);
    Serial.print("\tALS: "); Serial.println(als);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Proximity: "); display.println(ps);
    display.print("Ambient:   "); display.println(als);
    display.display();
  } else {
    Serial.println("Sensor read error");
  }

  delay(500);
}
