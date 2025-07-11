#include <Wire.h>
#include "Adafruit_VL53L1X.h"

// Custom I2C + optional reset
#define SDA_PIN 2
#define SCL_PIN 1
#define XSHUT_PIN 3

// Distance trigger threshold in mm
#define TRIGGER_DISTANCE_MM 200

Adafruit_VL53L1X vl53 = Adafruit_VL53L1X(XSHUT_PIN);

void setup() {
  Serial.begin(115200);
  delay(500);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!vl53.begin(0x29, &Wire)) {
    Serial.println("❌ VL53L1X not found!");
    while (1) delay(10);
  }

  if (!vl53.startRanging()) {
    Serial.println("❌ Failed to start ranging!");
    while (1) delay(10);
  }

  // Fastest stable timing
  vl53.setTimingBudget(20);

  Serial.println("📏 VL53L1X ready (trigger mode)");
}

void loop() {
  if (vl53.dataReady()) {
    int16_t distance = vl53.distance();

    if (distance > 0 && distance < TRIGGER_DISTANCE_MM) {
      Serial.print("⚠️ Triggered! Distance: ");
      Serial.print(distance);
      Serial.println(" mm");
    }

    vl53.clearInterrupt(); // Always clear after reading
  }
}
