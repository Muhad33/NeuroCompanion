/* NeuroCompanion_Pro.ino
   Multi-sensor professional sketch (skeleton)
   - Integrates: MPU6050 (IMU), MAX30102 (HR/SpO2), NEO-6M GPS
   - Modular: tinyML hook, SD logging, BLE/GSM sendAlert stub
   - Non-blocking, event-driven, with per-mode state machines

   Libraries required (install via Library Manager):
   - TinyGPSPlus
   - Wire
   - <MPU6050/ICM drivers e.g., "MPU6050.h" or "MPU6050_light.h">
   - Adafruit_Sensor + Adafruit_MAX30105 (or MAX30105 library)
   - SDFat / SD
   - TensorFlow Lite Micro (optional)
*/

#include <Wire.h>
#include <LiquidCrystal.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

// --- Add sensor library headers here ---
#include <MPU6050.h>           // example; pick a concrete library
#include <Adafruit_MAX30105.h> // or SparkFun MAX3010x
// #include "tensorflow/lite/micro/all_ops_resolver.h" // TFLM (if used)

#define FW_VERSION "v1.1-pro"
#define PROJECT_NAME "NeuroCompanion Pro"

// LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Pins
const uint8_t BUTTON_PIN = 7;    // INPUT_PULLUP
const uint8_t BUZZER_PIN = 8;
const uint8_t RED_PIN = 6, GREEN_PIN = 10, BLUE_PIN = 9;
const uint8_t INDICATOR_LED_PIN = 13;
const uint8_t VIB_SENSOR_PIN = A2; // analog for early prototyping
const uint8_t GPS_RX = 15, GPS_TX = 14; // SoftwareSerial pins
const uint8_t SD_CS = 4; // change as per shield

// Sensors
MPU6050 imu(Wire);
Adafruit_MAX30105 max30102;

// GPS
TinyGPSPlus gps;
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);

// Globals (timers, states)
unsigned long nowMs = 0;
enum Mode {MODE_ADHD=1, MODE_AUTISM=2, MODE_MEMORY=3, MODE_EPILEPSY=4};
Mode currentMode = MODE_ADHD;

// Debounce vars (same pattern as earlier)
const unsigned long DEBOUNCE_MS = 40;
unsigned long lastDebounce = 0;
int lastReading = HIGH;
int stable = HIGH;
unsigned long pressStart = 0;
const unsigned long LONG_PRESS_MS = 800;

// ML / detection thresholds
int EPI_THRESHOLD_ACCEL = 1200; // raw accel magnitude threshold (example)
int HR_SPIKE_DELTA = 15; // bpm delta considered abnormal

// TinyML model stub:
// - You would export a TFLite Micro model that accepts a short timeseries window
// - The code below provides a placeholder function runTinyMLInference()
bool runTinyMLInference(float *features, int n_features) {
  // Placeholder: return true if model predicts seizure/fall
  // Replace with TFLite micro inference code after training
  return false;
}

// sendAlert() - replace with BLE/GSM/HTTP integration
void sendAlert(const char* reason, bool includeGPS=true) {
  Serial.print(F("[ALERT] "));
  Serial.println(reason);
  if (includeGPS) {
    if (gps.location.isValid()) {
      Serial.print(F("GPS: "));
      Serial.print(gps.location.lat(), 6);
      Serial.print(", ");
      Serial.println(gps.location.lng(), 6);
    } else {
      Serial.println(F("GPS: No Fix"));
    }
  }
  // TODO: implement GSM / BLE / LoRa transmission here
}

// Sensor initialization helper
bool initSensors() {
  Wire.begin();
  // IMU
  if (!imu.begin()) {
    Serial.println(F("IMU init failed"));
    // some libraries return bool, others don't — adapt as needed
  } else {
    Serial.println(F("IMU OK"));
  }

  // MAX30102
  if (!max30102.begin()) {
    Serial.println(F("MAX30102 not found"));
    // non-fatal; continue
  } else {
    Serial.println(F("MAX30102 OK"));
    max30102.setup(); // default settings; tune LED current / sample rate
  }
  return true;
}

// --- Utility: compute accel magnitude ---
float accelMagnitude() {
  imu.update();
  float ax = imu.getAccX();
  float ay = imu.getAccY();
  float az = imu.getAccZ();
  return sqrt(ax*ax + ay*ay + az*az);
}

// --- GPS tick (non-blocking) ---
void gpsTick() {
  while (gpsSerial.available()) gps.encode(gpsSerial.read());
}

// --- Logging stub ---
void sdLogEvent(const char* tag, const char* msg) {
  Serial.print(F("[LOG] ")); Serial.print(tag); Serial.print(F(": ")); Serial.println(msg);
  // optional: write to SD card
}

// --- Per-mode advanced functions ---

// ADHD: adaptive reminders (learn user dismissals)
unsigned long adhd_last_time = 0;
unsigned long adhd_interval = 7000; // base
void runADHD() {
  if (millis() - adhd_last_time > adhd_interval) {
    adhd_last_time = millis();
    lcd.clear(); lcd.print("ADHD: Focus Now");
    // gentle tone + LED
    tone(BUZZER_PIN, 1200);
    setRGB(0, 0, 255);
    delay(300);
    noTone(BUZZER_PIN);
    setRGB(0,0,0);
    // log or adjust interval dynamically based on button dismissals via EEPROM
  }
}

// Autism mode: soothing breathing, BLE-controlled pattern
unsigned long autism_last = 0;
int autism_brightness = 0;
int autism_step = 4;
void runAutism() {
  if (millis() - autism_last > 30) {
    autism_last = millis();
    autism_brightness += autism_step;
    if (autism_brightness <= 0 || autism_brightness >= 255) autism_step = -autism_step;
    analogWrite(BLUE_PIN, autism_brightness);
    analogWrite(INDICATOR_LED_PIN, 255 - autism_brightness);
  }
  lcd.setCursor(0,0); lcd.print("Autism: Calm     ");
  gpsTick(); // show GPS status on second line optionally
}

// Memory mode: geofence + scheduled reminders
double safe_lat = 0.0, safe_lon = 0.0;
float safe_radius_m = 200.0; // caregiver-set radius
unsigned long memory_state_start = 0;
void runMemory() {
  // example: show where am I
  lcd.setCursor(0,0); lcd.print("Memory Mode      ");
  if (gps.location.isValid()) {
    lcd.setCursor(0,1);
    lcd.print("GPS:");
    lcd.print(gps.location.lat(),2); lcd.print(",");
    lcd.print(gps.location.lng(),2);
    // check geofence
    // compute haversine dist (implement simple function)
    // if outside safe_radius_m -> sendAlert("Geofence breach")
  } else {
    lcd.setCursor(0,1); lcd.print("No GPS Fix       ");
  }
}

// Epilepsy mode: multimodal event detection (IMU + HR + TinyML inference)
#define ACCEL_WINDOW  64
float accel_window[ACCEL_WINDOW];
int accel_idx = 0;
unsigned long lastAccelSample = 0;
void sampleAccelWindow() {
  if (millis() - lastAccelSample < 20) return; // 50Hz
  lastAccelSample = millis();
  accel_window[accel_idx++] = accelMagnitude();
  if (accel_idx >= ACCEL_WINDOW) accel_idx = 0;
}
bool detectSeizureCandidate() {
  // simple threshold detector
  for (int i=0;i<ACCEL_WINDOW;i++) {
    if (accel_window[i] > (float)EPI_THRESHOLD_ACCEL) return true;
  }
  // or build features (mean,std,max) and call runTinyMLInference()
  float features[4] = {0}; // placeholder
  // if (runTinyMLInference(features,4)) return true;
  return false;
}

unsigned long last_epilepsy_alert = 0;
void runEpilepsy() {
  sampleAccelWindow();
  // read HR quickly from MAX30102 (use library functions)
  // int hr = readHeartRate(); // stub

  if (detectSeizureCandidate()) {
    if (millis() - last_epilepsy_alert > 15000) { // cooldown
      last_epilepsy_alert = millis();
      lcd.clear(); lcd.print("Seizure Detected");
      sendAlert("Seizure Detected", true);
      sdLogEvent("SEIZURE","detected");
      // loud pattern
      for (int i=0;i<3;i++){ tone(BUZZER_PIN,1000); delay(300); noTone(BUZZER_PIN); delay(100); }
    }
  }
}

// helper: set RGB
void setRGB(uint8_t r, uint8_t g, uint8_t b){
  analogWrite(RED_PIN, r); analogWrite(GREEN_PIN, g); analogWrite(BLUE_PIN, b);
}

// --- Button polling (non-blocking, debounced) ---
bool lastStable = HIGH;
bool reading = HIGH;
unsigned long lastDebounceTime = 0;
bool longConsumed = false;
unsigned long singlePressTime = 0;
enum BtnEvent {NONE, SHORT, LONG};
BtnEvent pollButton() {
  int raw = digitalRead(BUTTON_PIN);
  if (raw != reading) { reading = raw; lastDebounceTime = millis(); }
  if (millis() - lastDebounceTime > DEBOUNCE_MS) {
    if (reading != lastStable) {
      lastStable = reading;
      if (lastStable == LOW) { // pressed
        singlePressTime = millis(); longConsumed = false;
      } else { // released
        unsigned long dur = millis() - singlePressTime;
        if (dur < LONG_PRESS_MS) return SHORT;
        else return LONG;
      }
    } else {
      if (lastStable == LOW && !longConsumed && (millis() - singlePressTime >= LONG_PRESS_MS)) {
        longConsumed = true;
        return LONG;
      }
    }
  }
  return NONE;
}

// --- Setup / Loop ---
void setup() {
  Serial.begin(115200);
  lcd.begin(16,2);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(INDICATOR_LED_PIN, OUTPUT);
  initSensors();
  gpsSerial.begin(9600);
  // optional SD begin
  lcd.print(PROJECT_NAME);
  delay(800);
}

void loop() {
  nowMs = millis();
  gpsTick();

  // handle button events
  BtnEvent be = pollButton();
  if (be == SHORT) {
    // next mode
    currentMode = (Mode)((((int)currentMode)%4)+1);
    sdLogEvent("MODE", String((int)currentMode).c_str());
  } else if (be == LONG) {
    // long press: show GPS or SOS
    sendAlert("Manual SOS", true);
  }

  // Per-mode
  switch (currentMode) {
    case MODE_ADHD: runADHD(); break;
    case MODE_AUTISM: runAutism(); break;
    case MODE_MEMORY: runMemory(); break;
    case MODE_EPILEPSY: runEpilepsy(); break;
  }

  // light sleep hint or sensor low-power switches (not implemented)
  delay(10);
}
