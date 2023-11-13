#include "myble.h"

bool sendLocation(SimpleBLE::Peripheral* peripheral, SimpleBLE::Service& service, SimpleBLE::Characteristic& charc, uint16_t x, uint16_t y){
    std::byte byteArray[sizeof(x) + sizeof(y)];

    // Copy the integers into the byte array
    memcpy(byteArray, &x, sizeof(x));
    memcpy(byteArray + sizeof(x), &y, sizeof(y));
    SimpleBLE::ByteArray bleByteArray(reinterpret_cast<char*>(byteArray), sizeof(byteArray));
    peripheral->write_request(service.uuid(), charc.uuid(), bleByteArray);
    return true;
}