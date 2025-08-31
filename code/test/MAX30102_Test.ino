#include <Wire.h>
#include "MAX30105.h"

MAX30105 particleSensor;

void setup() {
  Serial.begin(9600);
  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("MAX30102 not found. Check wiring.");
    while (1);
  }
  Serial.println("MAX30102 ready!");
}

void loop() {
  long irValue = particleSensor.getIR();
  Serial.print("IR: "); Serial.println(irValue);
  delay(500);
}
