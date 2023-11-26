#include <BleAbsMouse.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BleAbsMouse bleAbsMouse;
bool deviceConnected = false;
bool oldDeviceConnected = false;

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      // std::string value = pCharacteristic->getValue();
      // if (value.length() > 0) {
      //   Serial.println("*********");
      //   Serial.print("New value: ");
      //   for (int i = 0; i < value.length(); i++)
      //     Serial.print(value[i]);

      //   Serial.println();
      //   Serial.println("*********");
      // }
      std::string value = pCharacteristic->getValue();
      if (value.length() >= sizeof(uint16_t) * 2) {

          // Assuming the same order: x (uint16_t), y (uint16_t), op (uint8_t)
          uint16_t x = *reinterpret_cast<const uint16_t*>(value.data());
          uint16_t y = *reinterpret_cast<const uint16_t*>(value.data() + sizeof(uint16_t));

          Serial.print("X: "); Serial.print(x);
          Serial.print(", Y: "); Serial.println(y);
          bleAbsMouse.move(x,y);
      }
    }
};

void setup() {
  Serial.begin(115200); 
  while (!Serial);
  Serial.flush();
  delay(1000);
  bleAbsMouse.begin();
  while(!bleAbsMouse.isConnected());
  BLEServer* pServer = bleAbsMouse.getpServer();
  BLEService *pService = pServer->createService("19b10010-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         "19b10010-e8f2-537e-4f6c-d104768a1214",
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();         
  BLEDevice::startAdvertising(); 
  Serial.println("Waiting...");
}

void loop() {
  if(bleAbsMouse.isConnected()) {
    // bleAbsMouse.move(10000,10000);
    // delay(5000);
    // bleAbsMouse.move(9000,9000);
    // delay(5000);
    // bleAbsMouse.move(0,0);
    // delay(5000);
  }
}
