#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "command_parser.h"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "cbdad163-7a4a-4908-a257-219b571bb663"
#define CHARACTERISTIC_UUID "cfea1bbb-7cb3-4c95-a52a-2276b74839e5"

bool stateChanged = false;

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        String value = String(pCharacteristic->getValue().c_str());

    }
};

void setup()
{
    Serial.begin(115200); // Status message will be sent to PC at 9600 baud

    BLEDevice::init("STRIVE Reactor");
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UUID);
    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);
    pCharacteristic->setCallbacks(new MyCallbacks());
    pCharacteristic->setValue("N");
    pService->start();
    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();
    Serial.println("STRIVE Reactor boot...");
}

//+=============================================================================
// The repeating section of the code
//
void loop()
{
    delay(2000);
}