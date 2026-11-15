#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define two separate display objects
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);   // Bus 1 (Pins 21/22)
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, -1);  // Bus 2 (Pins 32/14)

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialize both independent hardware buses
  Wire.begin(21, 22);   // Bus 1, sda, scl
  Wire1.begin(32, 14);  // Bus 2, sda, scl

  // Initialize both displays
  display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display2.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop() {
  // 1. Probe Bus 1 (Pins 21/22)
  Wire.beginTransmission(0x3C);
  byte bus1Error = Wire.endTransmission();

  // 2. Probe Bus 2 (Pins 32/14)
  Wire1.beginTransmission(0x3C);
  byte bus2Error = Wire1.endTransmission();

  // 3. Update Display 1 (Bus 1) to show the health of BUS 2
  // (Note: If Bus 1 is dead, this screen won't update, which is our physical indicator!)
  if (bus1Error == 0) {
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

  // 4. Update Display 2 (Bus 2) to show the health of BUS 1
  if (bus2Error == 0) {
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

  // Serial Monitor diagnostic dump
  Serial.print("Bus 1 Error: ");
  Serial.print(bus1Error);
  Serial.print(" | Bus 2 Error: ");
  Serial.println(bus2Error);

  delay(1000);
}
