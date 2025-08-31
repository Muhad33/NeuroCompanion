# NeuroCompanion 🧠💡

An assistive wearable device built with Arduino for supporting individuals with ADHD, Autism, Memory Loss, Epilepsy, and Hearing Disabilities.  
It also includes health monitoring features like Sleep Tracking, Stress Detection, and Fall Detection.  

---

## 🌟 Features & Modes
- ADHD Mode → Adaptive reminders (LED + buzzer) with EEPROM-based interval tuning.
- Autism Mode → Soothing “breathing light” pattern (RGB fade) with calm-inducing feedback.
- Memory Mode → GPS-based geofencing + caregiver reminders.
- Epilepsy Mode → Seizure candidate detection using MPU6050 + MAX30102 with hooks for TinyML inference.
- SOS Button → Long press sends emergency alert (GPS coordinates included).
 
---

## 🔧 Tech Stack
- Arduino (Uno / Mega / Nano – depending on project)
- MPU6050 Accelerometer + Gyroscope (seizure detection)
- MAX30102 (Heart rate & SpO₂ sensor)
- NEO-6M GPS Module (location + geofence)
- 16x2 LCD Display (I2C or parallel)
- RGB LED (PWM pins)
- Buzzer + Vibration Motor
- Push button (mode navigation & SOS)
- SD Card Module (optional logging)
---

## 📂 Project Structure
```bash
NeuroCompanion/
│
├── code/
│   └── NeuroCompanion_Main/
│       └── NeuroCompanion_Pro.ino   # Main Arduino program
│
├── hardware/
│   └── pinout.md                    # Pin mapping & wiring notes
│
├── LICENSE
└── README.md                        # Project documentation
```

---
## ⚡ Installation & Setup

1. Clone the repo:
```bash
git clone https://github.com/Muhad33/NeuroCompanion.git
cd NeuroCompanion/code/NeuroCompanion_main
```

2. Open NeuroCompanion_Pro.ino in the Arduino IDE.
3. Install required libraries via Arduino IDE → Library Manager:
  - Wire
  - LiquidCrystal (or LiquidCrystal_I2C if using I2C LCD)
  - TinyGPSPlus
  - MPU6050 (or MPU6050_light)
  - Adafruit MAX30105 (for MAX30102/30105)
  - SD or SDFat (optional logging)
  - (optional) TensorFlow Lite for Microcontrollers (TinyML inference)
4. Select your Arduino board (Tools → Board).
5. Upload the sketch to your board.

## 🎮 Usage

- On startup → defaults to ADHD Mode.
- Short press button → Switch to next mode:
ADHD → Autism → Memory → Epilepsy → back to ADHD.
- Long press button → Send SOS alert with GPS coordinates.
- LCD displays the current mode status.
- Buzzer + LEDs + vibration provide multimodal feedback.
 ---
## 📖 Documentation

- [Hardware wiring details](hardware/pinout.md)  
- [Code modular design](code/NeuroCompanion_Pro.ino)



