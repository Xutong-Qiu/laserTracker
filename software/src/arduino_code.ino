////////#include "PluggableUSBHID.h"
//#include "USBMouse.h"
#include <ArduinoBLE.h>
#include <HIDMouse.h>

HIDMouse bleMouse;
//USBMouse Mouse;

BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214");  // create service

// create switch characteristic and allow remote device to read and write
BLECharacteristic ledCharacteristic("UUID", BLERead | BLEWrite, 20);  // 20 bytes max

void setup() {
  //ble mouse code
  pinMode(LED_BUILTIN, OUTPUT);
  bleMouse.setDeviceName("Device");
  bleMouse.setManufacturerName("Manufacturer");
  bleMouse.setBatteryLevel(75);
  bleMouse.begin();
  //////
  // put your setup code here, to run once:
  Serial.begin(9600);  // initialize serial communication
  while (!Serial)
    ;

  pinMode(LED_BUILTIN, OUTPUT);  // initialize the built-in LED pin to indicate when a central is connected

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1)
      ;
  }

  // set the local name peripheral advertises
  BLE.setLocalName("ButtonLED");
  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(ledService);

  // add the characteristics to the service
  ledService.addCharacteristic(ledCharacteristic);

  // add the service
  BLE.addService(ledService);

  BLE.advertise();

  Serial.println("Bluetooth® device active, waiting for connections...");
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's BT address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:
    digitalWrite(LED_BUILTIN, HIGH);

    // while the central is connected:
    while (central.connected()) {
      long currentMillis = millis();
      if (ledCharacteristic.written()) {
        const uint8_t* data = ledCharacteristic.value();
        int dataLength = ledCharacteristic.valueLength();

        if (dataLength == sizeof(uint16_t) * 2) {  // Expecting two uint16_t integers
          uint16_t receivedInt1, receivedInt2;

          // Unpack the integers
          memcpy(&receivedInt1, data, sizeof(receivedInt1));
          memcpy(&receivedInt2, data + sizeof(receivedInt1), sizeof(receivedInt2));

          // Use the received integer values
          Serial.print("x: ");
          Serial.print(receivedInt1);
          Serial.print("y: ");
          Serial.println(receivedInt2);
          if(bleMouse.isConnected()) {
            moveTo(receivedInt1, receivedInt2);
          }
          //moveTo(receivedInt1, receivedInt2);
        }
      }
    }
    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void moveTo(int x, int y){
    
  bleMouse.move(-1000,-1000);
  bleMouse.move(x,y);
}

