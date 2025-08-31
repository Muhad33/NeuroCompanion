# 🛠 NeuroCompanion Pro – Pin Mapping

This document lists the pin assignments for all sensors, actuators, and peripherals used in the project.  
Reference board: **Arduino UNO / Nano** (ATmega328P)

---

## 📌 Core Peripherals
- **LCD Display (LiquidCrystal 16x2)**  
  - RS → D12  
  - E  → D11  
  - D4 → D5  
  - D5 → D4  
  - D6 → D3  
  - D7 → D2  

- **Indicator LED**  
  - Built-in LED → D13  

- **Mode Button**  
  - Push Button → D7 (configured as `INPUT_PULLUP`)  

---

## 📌 Sensors
- **MPU6050 (IMU – Accelerometer & Gyroscope)**  
  - SDA → A4  
  - SCL → A5  

- **MAX30102 (Pulse Oximeter / HR Sensor)**  
  - SDA → A4  
  - SCL → A5  
  *(shares I²C bus with MPU6050)*  

- **Vibration Sensor (prototype analog input)**  
  - Signal → A2  

- **GPS Module (NEO-6M via SoftwareSerial)**  
  - GPS TX → D15 (Arduino RX)  
  - GPS RX → D14 (Arduino TX)  

---

## 📌 Outputs & Feedback
- **Buzzer**  
  - Signal → D8  

- **RGB LED (common cathode assumed)**  
  - Red   → D6  
  - Green → D10  
  - Blue  → D9  

---

## 📌 Storage / Expansion
- **SD Card Module**  
  - CS → D4  
  - MISO/MOSI/SCK → Hardware SPI (D11/D12/D13)  

---

## ⚡ Notes
- I²C bus (A4/A5) is shared between MPU6050 and MAX30102.  
- SPI pins depend on Arduino board (on UNO: D10–D13).  
- GPS uses SoftwareSerial; you may remap RX/TX to free hardware Serial for debugging.  
- Ensure common ground (GND) across **all modules**.  
- Power:  
  - Sensors: 3.3V/5V depending on module breakout.  
  - GPS and MAX30102 typically need **3.3V regulated**.  
