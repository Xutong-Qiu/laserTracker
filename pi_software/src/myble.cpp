#include "myble.h"
#include <thread>

static SimpleBLE::Service s;
static SimpleBLE::Characteristic c;
static SimpleBLE::Peripheral* p;

bool initSending(SimpleBLE::Peripheral* peripheral, SimpleBLE::Service& service, SimpleBLE::Characteristic& charc){
    s = service;
    c = charc;
    p = peripheral;
    return true;
}

    bool sendLocation(uint16_t x, uint16_t y, uint8_t op){
        std::byte byteArray[sizeof(x) + sizeof(y)];
        memcpy(byteArray, &x, sizeof(x));
        memcpy(byteArray + sizeof(x), &y, sizeof(y));
        //memcpy(byteArray + sizeof(x) + sizeof(y), &op, sizeof(op));
        SimpleBLE::ByteArray bleByteArray(reinterpret_cast<char*>(byteArray), sizeof(byteArray));
        std::thread writeThread([=](){
            p->write_request(s.uuid(), c.uuid(), bleByteArray);
        });
        writeThread.detach();
        return true;
    }