# ESP32 BLE Abs Mouse library

This library allows you to make the ESP32 act as a Bluetooth Absolute Mouse and control what it does. E.g. click, move, release and etc.

You might also be interested in:
- [ESP32-BLE-Mouse](https://github.com/T-vK/ESP32-BLE-Mouse)
- [ESP32-BLE-Keyboard](https://github.com/T-vK/ESP32-BLE-Keyboard)
- [ESP32-BLE-Gamepad](https://github.com/lemmingDev/ESP32-BLE-Gamepad)

## Features

 - [x] Click
 - [x] Move
 - [x] Release
 - [x] Report optional battery level to host (basically works, but it doesn't show up in Android's status bar)
 - [x] Customize Bluetooth device name/manufacturer
 - [x] Compatible with Android
 - [ ] Compatible with Windows
 - [ ] Compatible with Linux
 - [ ] Compatible with MacOS X
 - [ ] Compatible with iOS
 
## Installation
- (Make sure you can use the ESP32 with the Arduino IDE. [Instructions can be found here.](https://github.com/espressif/arduino-esp32#installation-instructions))
- [Download the latest release of this library from the release page.](https://github.com/sobrinho/ESP32-BLE-Abs-Mouse/releases)
- In the Arduino IDE go to "Sketch" -> "Include Library" -> "Add .ZIP Library..." and select the file you just downloaded.
- You can now go to "File" -> "Examples" -> "ESP32 BLE Abs Mouse" and select any of the examples to get started.

## Examples

```
#include <BleAbsMouse.h>

BleAbsMouse bleAbsMouse;

void setup() {
  bleAbsMouse.begin();
}

void loop() {
  if(bleAbsMouse.isConnected()) {
    bleAbsMouse.click(5000, 5000);
  }
  delay(2000);
}
```

Check [examples](examples) for concrete examples.

## API docs

```
// Use 0 up to 10000 to represent:
//   x: from left to right
//   y: from top to bottom
bleAbsMouse.click(x,y);

// If you want to click and stay clicked, use move instead:
bleAbsMouse.move(x,y);

// Since move keeps the finger as pressed, you need to release it when convenient
bleAbsMouse.release();
```

There is also Bluetooth specific information that you can use (optional):

Instead of `BleAbsMouse bleAbsMouse;` you can do `BleAbsMouse bleAbsMouse("Bluetooth Device Name", "Bluetooth Device Manufacturer", 100);`.

The third parameter is the initial battery level of your device. To adjust the battery level later on you can simply call e.g. `bleAbsMouse.setBatteryLevel(50)` (set battery level to 50%).
By default the battery level will be set to 100%, the device name will be `ESP32 Bluetooth Abs Mouse` and the manufacturer will be `Espressif`.

## Credits

Credits to [T-vK](https://github.com/T-vK) as this library is based on [ESP32-Ble-Mouse](https://github.com/T-vK/ESP32-BLE-Mouse) that he created.
