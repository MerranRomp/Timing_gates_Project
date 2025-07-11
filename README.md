# 🏃 Sprint Timer

A portable, "accurate" sprint timing system using ESP32 microcontrollers and Time-of-Flight (ToF) sensors. Designed for track athletes who want precise time measurements without expensive photo-finish equipment.

## 🚀 Project Goals

- Measure the time between start and finish with millisecond accuracy
- Use low-cost, open-source hardware (ESP32-based)
- Detect line crossing with sensors (VL53L1X or vision-based)
- Support wireless communication (ESP-NOW / BLE / Wi-Fi)
- Optional buzzer to simulate race start


## 🧰 Hardware

### Required (per gate):
- [x] ESP32-S3 (or ESP32 DevKit, S3 Mini, etc.)
- [x] VL53L1X Time-of-Flight sensor
- [x] Power source (USB power bank, LiPo, or 18650 + boost)
- [x] Jumper wires, breadboard or PCB

### Optional:
- [ ] Passive buzzer (for start signal)
- [ ] OLED/TFT display (to show times)

## 🧠 System Overview

- **Start Gate**:
  - Detects line break using VL53L1X
  - Optionally plays countdown and start beep via buzzer
  - Sends start time wirelessly to finish gate

- **Finish Gate**:
  - Detects line break using second VL53L1X
  - Receives or stores start time
  - Calculates and displays elapsed time

## 📡 Communication

- **ESP-NOW** (recommended): fast, peer-to-peer wireless messaging
- **BLE** or **UDP over Wi-Fi** also possible for syncing gates

## 🧪 Status

| Component           | Status       |
|---------------------|--------------|
| ESP32 base firmware | ✅ In progress |
| Sensor integration  | ✅ Working    |
| Wireless sync       | 🛠️ TBD         |
| Buzzer logic        | ✅ Basic beep  |
| Vision support      | 🧪 FUture idea |
| Enclosure design    | 🛠️ TBD         |

## 📦 Setup

1. Flash ESP32s with appropriate start/finish firmware
2. Wire up the VL53L1X sensor via I²C (SDA/SCL + power)
3. (Optional) Wire buzzer to GPIO (e.g., GPIO 13)
4. Power each unit via USB or battery
5. Align sensors across track lane
6. Run and record!

## 📷 Future Ideas

- Web interface to log and visualize times
- vision to record chest crossing instead of anything crossing
- Voice countdown ("on your marks, Set, pauw!")