/**
 * This example turns the ESP32 into a Bluetooth LE Absolute Mouse that clicks
 * the center of the screen every 2 seconds.
 */
#include <BleAbsMouse.h>

BleAbsMouse bleAbsMouse;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleAbsMouse.begin();
}

void loop() {
  if(bleAbsMouse.isConnected()) {
    Serial.println("Click");
    bleAbsMouse.click(5000, 5000);
  }
  delay(2000);
}
