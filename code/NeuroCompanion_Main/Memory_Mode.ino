// ===================== Memory_Mode.ino =====================
// Memory Mode: Reminders + logging

extern void sendAlert(const char* msg);
extern void sdLogEvent(const char* msg);

unsigned long lastMemoryReminder = 0;
const unsigned long memoryInterval = 10000; // 10s test interval

void runMemory() {
  unsigned long now = millis();

  if (now - lastMemoryReminder >= memoryInterval) {
    lastMemoryReminder = now;

    Serial.println("Memory Mode: Time for scheduled task!");
    sendAlert("Memory Mode: Reminder triggered!");
    sdLogEvent("Reminder issued in Memory Mode");
  }
}
