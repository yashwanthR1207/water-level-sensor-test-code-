# ESP32 Water Level Monitoring Test Rig (GPIO 34)

[![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Framework: Arduino](https://img.shields.io/badge/Framework-Arduino-00979C.svg)](https://www.arduino.cc/)
[![IDE: PlatformIO](https://img.shields.io/badge/IDE-PlatformIO-orange.svg)](https://platformio.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A clean, reliable water level monitoring test sketch for the **ESP32** microcontroller using **GPIO 34** (ADC1 Channel 6).

This repository is ready to be uploaded to your ESP32 and pushed to GitHub.

---

## 📌 Features

- **Input-Only ADC Pin (GPIO 34)**: Utilizes `ADC1_CH6`, which does **not** conflict with ESP32 Wi-Fi operations (unlike ADC2 pins).
- **Oversampling & Noise Filtering**: Averages multiple consecutive samples with settling delays to dampen water ripple jitter and electrical noise.
- **Voltage & Percentage Calculation**: Translates raw 12-bit ADC values (0–4095) into estimated sensor voltage and normalized percentage (0% – 100%).
- **State Alerts**: Dynamically categorizes current water level into `EMPTY (Dry)`, `LOW`, `NORMAL / MEDIUM`, `HIGH`, and `FULL / OVERFLOW WARNING`.
- **Non-Blocking Architecture**: Uses `millis()` intervals instead of `delay()`, making it easy to integrate Wi-Fi, MQTT, Blynk, or web dashboards later.

---

## 🔌 Hardware & Wiring

### Components
1. **ESP32 Development Board** (ESP32-WROOM-32, ESP32-NodeMCU, or equivalent)
2. **Analog Water Level Sensor** (Resistive trace sensor, capacitive water sensor, or analog submersible probe)
3. **Jumper Wires** & Breadboard

### Wiring Diagram

```
       +-----------------------+
       |   Water Level Sensor  |
       |                       |
       |  [+] VCC   [-] GND  [S] Signal
       +---+-----------+-------+---+
           |           |       |
           |           |       |
      3.3V |           | GND   | GPIO 34 (ADC1_CH6)
           |           |       |
       +---+-----------+-------+---+
       |  3V3         GND     D34  |
       |                           |
       |          ESP32            |
       +---------------------------+
```

| Sensor Pin | ESP32 Pin | Description |
| :--- | :--- | :--- |
| **VCC (+)** | **3.3V** | Power supply (*Note: Power with 3.3V to keep ADC voltage safe*) |
| **GND (-)** | **GND** | Ground reference |
| **Signal (S / OUT)** | **GPIO 34** | Analog output connected to ESP32 ADC1 Channel 6 |

> [!IMPORTANT]
> **Why GPIO 34?**
> - GPIO 34 is an **input-only** pin (`GPI`), making it ideal for dedicated analog sensing.
> - GPIO 34 belongs to **ADC1**. ESP32 **ADC2** cannot be used simultaneously with Wi-Fi, but **ADC1** works without any restrictions.
> - Keep sensor VCC connected to **3.3V** to protect the ESP32 ADC from exceeding the 3.3V safe rating.

---

## ⚙️ Calibration Guide

Water conductivity, sensor trace distance, and liquid salinity vary. To calibrate for accurate percentage readings:

1. **Dry Calibration (`SENSOR_DRY`)**:
   - Power up the ESP32 with the sensor completely dry and in ambient air.
   - Open Serial Monitor at `115200` baud.
   - Note the **Raw ADC** reading (typically `40` – `150`).
   - Update `SENSOR_DRY` in `esp32_water_level_monitor.ino`.

2. **Wet Calibration (`SENSOR_MAX_WET`)**:
   - Submerge the sensor up to your target maximum water line.
   - Note the **Raw ADC** reading (typically `2400` – `3200`).
   - Update `SENSOR_MAX_WET` in `esp32_water_level_monitor.ino`.

3. Re-flash the sketch to enjoy calibrated `0% - 100%` accuracy.

---

## 🚀 How to Run

### Option A: Arduino IDE

1. Install [Arduino IDE](https://www.arduino.cc/en/software) (version 2.x recommended).
2. Add ESP32 Board Support:
   - Go to `Settings` -> `Additional Boards Manager URLs` and add:
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to `Boards Manager`, search for `esp32` by Espressif Systems, and click **Install**.
3. Open `esp32_water_level_monitor.ino`.
4. Select your board (e.g., `ESP32 Dev Module`) and your COM/Serial port.
5. Click **Upload** and open **Serial Monitor** at **115200 baud**.

### Option B: PlatformIO (VS Code)

1. Open this repository folder in VS Code with the PlatformIO extension installed.
2. PlatformIO will automatically detect `platformio.ini`.
3. Connect your ESP32 via USB.
4. Click the **Build** (✓) and **Upload** (→) buttons on the PlatformIO bottom bar.
5. Click the **Serial Monitor** (plug icon) at `115200` baud.

---

## 📊 Sample Serial Monitor Output

```text
==================================================
     ESP32 Water Level Monitoring Test Rig        
==================================================
Sensor Pin: GPIO 34
ADC configured: 12-bit (0-4095), 11dB attenuation (~0 - 3.3V)
Status: Starting real-time monitoring...
--------------------------------------------------
[1s] Raw ADC:   75 | Voltage: 0.06V | Level:   0.0% | Status: EMPTY (Dry)
[2s] Raw ADC:  850 | Voltage: 0.68V | Level:  27.3% | Status: LOW
[3s] Raw ADC: 1620 | Voltage: 1.30V | Level:  54.6% | Status: NORMAL / MEDIUM
[4s] Raw ADC: 2450 | Voltage: 1.97V | Level:  84.0% | Status: HIGH
[5s] Raw ADC: 2950 | Voltage: 2.37V | Level: 100.0% | Status: FULL / OVERFLOW WARNING
```

---

## 🛠️ Pushing to GitHub

To push this project to a new repository on your GitHub account:

```bash
# 1. Initialize git in this directory (if not already done)
git init -b main

# 2. Stage all project files
git add .

# 3. Create initial commit
git commit -m "Initial commit: ESP32 water level monitoring test code for GPIO 34"

# 4. Link your GitHub repository (replace with your repo URL)
git remote add origin https://github.com/<your-username>/<your-repo-name>.git

# 5. Push code to main branch
git push -u origin main
```

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).

# water-level-sensor-test-code-
