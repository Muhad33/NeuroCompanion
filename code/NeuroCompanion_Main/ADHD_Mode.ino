// ===================== ADHD_Mode.ino =====================
// ADHD Mode: Visual + vibration reminders to stay focused

#include <Adafruit_NeoPixel.h>

// External declarations (already in Main & Utils)
extern Adafruit_NeoPixel strip;
extern void setRGB(uint8_t r, uint8_t g, uint8_t b);

unsigned long lastReminder = 0;
const unsigned long reminderInterval = 5000; // 5 seconds for testing

void runADHD() {
  unsigned long now = millis();

  if (now - lastReminder >= reminderInterval) {
    lastReminder = now;

    Serial.println("ADHD Reminder: Stay focused!");

    // Visual reminder
    setRGB(255, 255, 0); // yellow flash
    delay(300);
    setRGB(0, 0, 0);

    // Vibration (if motor is connected on pin 6)
    digitalWrite(6, HIGH);
    delay(200);
    digitalWrite(6, LOW);
  }
}
