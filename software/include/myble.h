#include <simpleble/SimpleBLE.h>
#include <cstring>
#include <string>

bool initSending(SimpleBLE::Peripheral* peripheral, SimpleBLE::Service& service, SimpleBLE::Characteristic& charc);
bool sendLocation( uint16_t x, uint16_t y);