/**
 * This example turns the ESP32 into a Bluetooth LE Absolute Mouse that presses,
 * moves and releases (drags n' drops) something from the left to the right of
 * the screen every 2 seconds.
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
    Serial.println("Press");
    bleAbsMouse.move(2500, 2500);
		delay(1000);

    Serial.println("Move");
    bleAbsMouse.move(5000, 5000);
		delay(1000);

    Serial.println("Move");
    bleAbsMouse.move(7500, 7500);
		delay(1000);

    Serial.println("Release");
    bleAbsMouse.release();
  }

  delay(2000);
}
