// ===================== Autism_Mode.ino =====================
// Autism Mode: Calming LED + optional "Game Mode"

extern void setRGB(uint8_t r, uint8_t g, uint8_t b);

bool gameMode = false;  // toggle with a button later

void runAutism() {
  if (!gameMode) {
    // Calming effect - slow fade blue
    for (int i = 0; i < 255; i++) {
      setRGB(0, 0, i);
      delay(5);
    }
    for (int i = 255; i > 0; i--) {
      setRGB(0, 0, i);
      delay(5);
    }
  } else {
    // Game Mode: blink rainbow quickly
    setRGB(255, 0, 0); delay(200);
    setRGB(0, 255, 0); delay(200);
    setRGB(0, 0, 255); delay(200);
  }
}
