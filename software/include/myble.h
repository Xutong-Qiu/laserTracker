#include <simpleble/SimpleBLE.h>
#include <cstring>
#include <string>

bool sendLocation(SimpleBLE::Peripheral* peripheral, SimpleBLE::Service& service, SimpleBLE::Characteristic& charc, uint16_t x, uint16_t y);