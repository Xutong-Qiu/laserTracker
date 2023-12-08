#include <BleAbsMouse.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BleAbsMouse bleAbsMouse;
#define BUTTON_PIN 14 
uint16_t x, y;
bool prev_click = false;
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      if (value.length() >= sizeof(uint16_t) * 2) {
        x = *reinterpret_cast<const uint16_t*>(value.data());
        y = *reinterpret_cast<const uint16_t*>(value.data() + sizeof(uint16_t));
        //Serial.print("X: "); Serial.print(x);
        //Serial.print(", Y: "); Serial.println(y);
        int buttonState = digitalRead(BUTTON_PIN);
        //With cursor
        if(buttonState == 0){
          bleAbsMouse.release();
          bleAbsMouse.click(x,y);
          delay(50);
          bleAbsMouse.move(x,y);
          prev_click = true;
        }else{
          if(prev_click){
            bleAbsMouse.release();
            bleAbsMouse.move(5000,0);
            prev_click = false;
          }
            bleAbsMouse.move(x,y);
        }
        //Without cursor
        // if(buttonState == 0){
        //   bleAbsMouse.move(x,y);
        //   prev_click = true;
        // }else{
        //   if(prev_click){
        //     bleAbsMouse.release();
        //     prev_click = false;
        //   }
        // }
      }
    }
};
BLEServer* pServer;
void setup() {
  Serial.begin(115200); 
  while (!Serial);
  Serial.flush();
  delay(1000);
  bleAbsMouse.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  delay(1000);
  pServer = bleAbsMouse.getpServer();
  BLEService *pService = pServer->createService("19b10010-e8f2-537e-4f6c-d104768a1214");
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         "19b10010-e8f2-537e-4f6c-d104768a1214",
                                         //BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE_NR 
                                       );
  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start(); 
  //delay(500);  
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();   
  //BLEDevice::getAdvertising()->start();
  //BLEDevice::startAdvertising();
  Serial.println("Waiting for connection...");
}

void loop() {
  while(pServer->getConnectedCount()!=2){
    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();   
    delay(5000);
  }
  Serial.println("Connected!");
  bleAbsMouse.move(5000,0);
  while(pServer->getConnectedCount()==2) {
    delay(5000);
  }
  Serial.println("Waiting for connection...");
}
