#include "myble.h"
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <chrono>

struct TimedBLEData {
    SimpleBLE::ByteArray data;
    std::chrono::steady_clock::time_point timestamp;
};

std::queue<TimedBLEData> writeQueue;
std::mutex queueMutex;
std::condition_variable queueCond;
bool writeThreadRunning = true;

static SimpleBLE::Service s;
static SimpleBLE::Characteristic c;
static SimpleBLE::Peripheral* p;

void writeThreadFunction() {
    while (writeThreadRunning) {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCond.wait(lock, []{ return !writeQueue.empty() || !writeThreadRunning; });

        while (!writeQueue.empty()) {
            auto timedData = writeQueue.front();
            writeQueue.pop();
            lock.unlock();

            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - timedData.timestamp).count() <= 100) {
                // Data is still fresh, send it
                p->write_request(s.uuid(), c.uuid(), timedData.data);
            } 
            lock.lock();
        }
    }
}


// Call this function to start the dedicated thread
void startWriteThread() {
    std::thread writeThread(writeThreadFunction);
    writeThread.detach();
}

// Call this function to stop the dedicated thread
void stopWriteThread() {
    writeThreadRunning = false;
    queueCond.notify_one();
}

bool initSending(SimpleBLE::Peripheral* peripheral, SimpleBLE::Service& service, SimpleBLE::Characteristic& charc){
    s = service;
    c = charc;
    p = peripheral;
    startWriteThread();
    return true;
}

bool sendLocation(uint16_t x, uint16_t y){
    std::byte byteArray[sizeof(x) + sizeof(y)];
    memcpy(byteArray, &x, sizeof(x));
    memcpy(byteArray + sizeof(x), &y, sizeof(y));
    //memcpy(byteArray + sizeof(x) + sizeof(y), &op, sizeof(op));
    SimpleBLE::ByteArray bleByteArray(reinterpret_cast<char*>(byteArray), sizeof(byteArray));
    
    TimedBLEData timedData = {
        .data = SimpleBLE::ByteArray(reinterpret_cast<char*>(byteArray), sizeof(byteArray)),
        .timestamp = std::chrono::steady_clock::now()
    };

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        writeQueue.push(timedData);
    }
    queueCond.notify_one();
    return true;
}