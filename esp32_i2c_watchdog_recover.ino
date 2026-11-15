#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define two separate display objects
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);   // Bus 1 (Pins 21/22)
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, -1);  // Bus 2 (Pins 32/14)

// Tracking states to handle auto-recovery when power returns
bool display1Ready = false;
bool display2Ready = false;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialize both independent hardware buses
  Wire.begin(21, 22);   // Bus 1
  Wire1.begin(32, 14);  // Bus 2

  // Initial startup attempt
  display1Ready = display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display2Ready = display2.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop() {
  // 1. Probe Bus 1 (Pins 21/22)
  Wire.beginTransmission(0x3C);
  byte bus1Error = Wire.endTransmission();

  // 2. Probe Bus 2 (Pins 32/14)
  Wire1.beginTransmission(0x3C);
  byte bus2Error = Wire1.endTransmission();

  // --- AUTO-RECOVERY & DISPLAY 1 LOGIC ---
  if (bus1Error == 0) {
    // If the bus is back to healthy but display wasn't initialized, revive it automatically!
    if (!display1Ready) {
      display1Ready = display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    }
    
    if (display1Ready) {
      display1.clearDisplay();
      display1.setTextSize(1);
      display1.setTextColor(SSD1306_WHITE);
      display1.setCursor(0, 0);
      display1.println("WATCHDOG: BUS 2");
      display1.drawLine(0, 10, 128, 10, SSD1306_WHITE);
      
      display1.setCursor(0, 16);
      display1.print("Pins 32/14: ");
      if (bus2Error == 0) {
        display1.println("ONLINE");
        display1.setCursor(0, 30);
        display1.println("Status: ACK OK");
        display1.setCursor(0, 42);
        display1.println("Partner is healthy.");
      } else {
        display1.println("FAULT!");
        display1.setCursor(0, 30);
        display1.print("Err Code: ");
        display1.println(bus2Error);
        display1.setCursor(0, 42);
        display1.println("Partner bus down!");
      }
      display1.display();
    }
  } else {
    display1Ready = false; // Mark unready if the bus throws an error
  }

  // --- AUTO-RECOVERY & DISPLAY 2 LOGIC ---
  if (bus2Error == 0) {
    // If the bus is back to healthy but display wasn't initialized, revive it automatically!
    if (!display2Ready) {
      display2Ready = display2.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    }

    if (display2Ready) {
      display2.clearDisplay();
      display2.setTextSize(1);
      display2.setTextColor(SSD1306_WHITE);
      display2.setCursor(0, 0);
      display2.println("WATCHDOG: BUS 1");
      display2.drawLine(0, 10, 128, 10, SSD1306_WHITE);
      
      display2.setCursor(0, 16);
      display2.print("Pins 21/22: ");
      if (bus1Error == 0) {
        display2.println("ONLINE");
        display2.setCursor(0, 30);
        display2.println("Status: ACK OK");
        display2.setCursor(0, 42);
        display2.println("Partner is healthy.");
      } else {
        display2.println("FAULT!");
        display2.setCursor(0, 30);
        display2.print("Err Code: ");
        display2.println(bus1Error);
        display2.setCursor(0, 42);
        display2.println("Partner bus down!");
      }
      display2.display();
    }
  } else {
    display2Ready = false; // Mark unready if the bus throws an error
  }

  // Serial Monitor diagnostic dump
  Serial.print("Bus 1 Error: ");
  Serial.print(bus1Error);
  Serial.print(" | Bus 2 Error: ");
  Serial.println(bus2Error);

  delay(1000);
}
