# NeuroCompanion 🧠💡

An assistive wearable device built with Arduino for supporting individuals with ADHD, Autism, Memory Loss, Epilepsy, and Hearing Disabilities.  
It also includes health monitoring features like Sleep Tracking, Stress Detection, and Fall Detection.  

---

## 🌟 Features & Modes

1. **ADHD Mode** → Focus reminders (LED + Vibration + Buzzer) + Data Logging.  
2. **Autism Mode** → Calming LEDs + Mini-game + Stress auto-detection.  
3. **Memory Loss Mode** → Medication reminders + GPS safety alerts.  
4. **Epilepsy Mode** → Seizure detection (MPU6050) + SOS hospital alert + Health stats.  
5. **Deaf Mode** → Morse code communication + vibration-based alerts.  
6. **Sleep Mode** → Pulse + SpO₂ + motion-based sleep analysis.  
7. **Stress Mode** → Detects stress via HRV & skin temp → activates relaxation cues.  
8. **Fall Detection** → Detects elderly falls and auto sends alert.  

---

## 🔧 Tech Stack
- Arduino UNO/Nano
- MPU6050 Accelerometer
- Pulse + SpO₂ Sensor
- Temperature Sensor
- GPS Module
- Bluetooth/Wi-Fi Module
- SD Card Module
- Vibration Motor, LEDs, Buzzer

---

## 📂 Project Structure
- `code/` → All Arduino sketches (Main + Modes)  
- `hardware/` → Circuit diagrams, datasheets  
- `docs/` → Documentation, project overview  
- `data/` → Sensor logs (CSV/JSON)  

---

## 🚀 Getting Started
1. Clone repo:
   ```bash
   git clone https://github.com/YourUsername/NeuroCompanion.git
   cd NeuroCompanion/code/NeuroCompanion_Main

