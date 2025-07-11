// VL53L1X Fast Trigger Timing with Register-Level Access
// ESP32-S3 (SDA: 2, SCL: 1, XSHUT: 10)
// This code is designed to work with the VL53L1X distance sensor using I2C communication.
// It initializes the sensor, reads distance measurements, and detects two trigger events with timing.



#include <Wire.h>

#define SDA_PIN 2
#define SCL_PIN 1
#define XSHUT_PIN 10
#define SENSOR_ADDR 0x29

#define TRIGGER_THRESHOLD_MM 800
#define COOLDOWN_MS 1000

unsigned long firstTriggerTime = 0;
unsigned long lastTriggerTime = 0;
bool firstTriggered = false;
bool cooldown = false;

void writeReg(uint16_t reg, uint8_t val) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg >> 8);
  Wire.write(reg & 0xFF);
  Wire.write(val);
  Wire.endTransmission();
}

uint8_t readReg(uint16_t reg) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg >> 8);
  Wire.write(reg & 0xFF);
  Wire.endTransmission(false);
  Wire.requestFrom(SENSOR_ADDR, 1);
  return Wire.read();
}

uint16_t readReg16(uint16_t reg) {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(reg >> 8);
  Wire.write(reg & 0xFF);
  Wire.endTransmission(false);
  Wire.requestFrom(SENSOR_ADDR, 2);
  uint8_t msb = Wire.read();
  uint8_t lsb = Wire.read();
  return (msb << 8) | lsb;
}

void xshutReset() {
  pinMode(XSHUT_PIN, OUTPUT);
  digitalWrite(XSHUT_PIN, LOW);
  delay(10);
  digitalWrite(XSHUT_PIN, HIGH);
  delay(10);
}

void loadDefaultConfig() {
  writeReg(0x2D, 0x00);  // ... (other register setups omitted for brevity)
  writeReg(0x86, 0x01);  // clear interrupt
  writeReg(0x87, 0x40);  // start ranging
}

void setup() {
  Serial.begin(115200);
  delay(100);

  Wire.begin(SDA_PIN, SCL_PIN);
  xshutReset();

  uint16_t modelID = readReg16(0x010F);
  Serial.print("Model ID: 0x");
  Serial.println(modelID, HEX);
  if (modelID != 0xEACC) {
    Serial.println("❌ Invalid model ID");
    while (1) delay(1);
  }

  loadDefaultConfig();
  Serial.println("Ready to detect two trigger events with timing...");
}

void loop() {
  if ((readReg(0x0089) & 0x01) != 0) {
    uint16_t dist = readReg16(0x0096);
    unsigned long now = micros();
    writeReg(0x0086, 0x01); // clear interrupt

    if (dist < TRIGGER_THRESHOLD_MM && dist > 0) {
      if (!firstTriggered && !cooldown) {
        firstTriggerTime = now;
        firstTriggered = true;
        Serial.println("🟡 First trigger!");
      } else if (firstTriggered && !cooldown) {
        unsigned long secondTriggerTime = now;
        unsigned long duration = secondTriggerTime - firstTriggerTime;
        Serial.print("🔁 Second trigger! Δt = ");
        Serial.print(duration / 1000.0);
        Serial.println(" ms");

        cooldown = true;
        lastTriggerTime = millis();
        firstTriggered = false;
      }
    }
  }

  if (cooldown && millis() - lastTriggerTime > COOLDOWN_MS) {
    cooldown = false;
    Serial.println("✅ Cooldown over");
  }
}