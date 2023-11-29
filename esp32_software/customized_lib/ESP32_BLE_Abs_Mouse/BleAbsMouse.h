#ifndef ESP32_BLE_ABS_MOUSE_H
#define ESP32_BLE_ABS_MOUSE_H
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "BleConnectionStatus.h"
#include "BLEHIDDevice.h"
#include "BLECharacteristic.h"

class BleAbsMouse {
private:
  BleConnectionStatus* connectionStatus;
  BLEHIDDevice* hid;
  BLECharacteristic* inputAbsMouse;
  BLEServer* server;
  void rawAction(uint8_t msg[], char msgSize);
  static void taskServer(void* pvParameter);
public:
  BleAbsMouse(std::string deviceName = "ESP32 Bluetooth Abs Mouse", std::string deviceManufacturer = "Espressif", uint8_t batteryLevel = 100);
  void begin(void);
  void end(void);
  void click(int16_t x, int16_t y);
  void move(int16_t x, int16_t y);
  void release(void);
  void send(int state, int16_t x, int16_t y);
  bool isConnected(void);
  bool isConnectedController(void);
  void setBatteryLevel(uint8_t level);
  BLEServer* getpServer();
  uint8_t batteryLevel;
  std::string deviceManufacturer;
  std::string deviceName;
  bool isPressed;
protected:
  virtual void onStarted(BLEServer *pServer) { };
};

#endif // CONFIG_BT_ENABLED
#endif // ESP32_BLE_ABS_MOUSE_H
