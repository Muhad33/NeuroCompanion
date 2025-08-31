// ===================== Epilepsy_Mode.ino =====================
// Epilepsy Mode: Detect sudden jerks from MPU6050 and send alert

extern float accelMagnitude();
extern void sendAlert(const char* msg);

const float seizureThreshold = 2.0;  // adjust based on testing
unsigned long lastSeizureCheck = 0;

void runEpilepsy() {
  unsigned long now = millis();

  if (now - lastSeizureCheck >= 500) { // check every 0.5s
    lastSeizureCheck = now;

    float mag = accelMagnitude();

    if (mag > seizureThreshold) {
      Serial.println("⚠️ Possible Seizure Detected!");
      sendAlert("Emergency: Seizure detected! Sending location...");
    }
  }
}
