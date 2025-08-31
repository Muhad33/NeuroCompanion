// ===================== NeuroCompanion_Main.ino =====================
// Main entry point for the NeuroCompanion project
// Only contains setup(), loop(), and mode switching logic

// ---------------- Includes ----------------
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// ---------------- Mode Enum ----------------
enum Mode { ADHD, AUTISM, MEMORY, EPILEPSY, DEAF };
Mode currentMode = ADHD;  // default mode at startup

// ---------------- Function Declarations ----------------
// (these will be defined in separate .ino files)
void runADHD();
void runAutism();
void runMemory();
void runEpilepsy();
void runDeaf();

void initSensors();
void updateGPS();

// ---------------- Setup ----------------
void setup() {
  Serial.begin(9600);
  Wire.begin();

  initSensors();  // Sensors.ino
  Serial.println("NeuroCompanion System Initialized");
}

// ---------------- Loop ----------------
void loop() {
  updateGPS();  // keep GPS updated (Sensors.ino)

  switch (currentMode) {
    case ADHD:     runADHD();     break;
    case AUTISM:   runAutism();   break;
    case MEMORY:   runMemory();   break;
    case EPILEPSY: runEpilepsy(); break;
    case DEAF:     runDeaf();     break;
  }

  delay(100);  // basic loop pacing
}
