#include "myble.h"
#include <thread>

static SimpleBLE::Service s;
SimpleBLE::Characteristic c;
SimpleBLE::Peripheral* p;

bool initSending(SimpleBLE::Peripheral* peripheral, SimpleBLE::Service& service, SimpleBLE::Characteristic& charc){
    s = service;
    c = charc;
    p = peripheral;
    return true;
}

bool sendLocation(uint16_t x, uint16_t y){
    std::byte byteArray[sizeof(x) + sizeof(y)];
    memcpy(byteArray, &x, sizeof(x));
    memcpy(byteArray + sizeof(x), &y, sizeof(y));
    SimpleBLE::ByteArray bleByteArray(reinterpret_cast<char*>(byteArray), sizeof(byteArray));
    std::thread writeThread([=](){
        p->write_request(s.uuid(), c.uuid(), bleByteArray);
    });

    // Detach the thread to allow it to run independently
    writeThread.detach();
    return true;
}