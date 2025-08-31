// ===================== Deaf_Mode.ino =====================
// Deaf Mode: Morse code communication via LED

extern void setRGB(uint8_t r, uint8_t g, uint8_t b);

void morseDot() {
  setRGB(255, 255, 255);
  delay(200);
  setRGB(0, 0, 0);
  delay(200);
}

void morseDash() {
  setRGB(255, 255, 255);
  delay(600);
  setRGB(0, 0, 0);
  delay(200);
}

void runDeaf() {
  // Example: send "SOS" (... --- ...)
  morseDot(); morseDot(); morseDot(); // S
  delay(400);
  morseDash(); morseDash(); morseDash(); // O
  delay(400);
  morseDot(); morseDot(); morseDot(); // S
  delay(1000);
}
