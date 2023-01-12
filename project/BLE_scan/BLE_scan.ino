#include <BLEDevice.h>
#include <BLEAdvertisedDevice.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

int scanTime = 2; //In seconds
BLEScan* pBLEScan;
const char* ssid     = "ccit";
const char* password = "00000000503";
char* array = new char[30];
int RS = 0;
int reading = 0;
int led = 23;
boolean ship[5];
boolean curship[5];
boolean preship[5];
boolean ship_state;
String jsondata;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      char *pHex = BLEUtils::buildHexData(nullptr, (uint8_t*)advertisedDevice.getManufacturerData().data(), advertisedDevice.getManufacturerData().length());
      array = pHex;
      RS = advertisedDevice.getRSSI();
      for(int i = 1; i <= 5; i++){
        char a = i + 49;
        if(array[0] == '4' && array[15] == a){
//          ship[i] = 1;
//          reading++;
          Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
          Serial.println(RS);
        }
        else{
//          ship[i] = 0;
        }
      }

    }

};

void initBle() {
  // Create the BLE Device
  BLEDevice::init("BLEClient");
}

void scan() {
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);            //active scan uses more power, but get results faster
  BLEScanResults foundDevices = pBLEScan->start(scanTime);

  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
}

void setup() {
  Serial.begin(115200);
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
//    Serial.print(".");
  }
  Serial.println(" CONNECTED");
}

void loop() {
  reading = 0;
  
  Serial.println("Start Scanning...");
  initBle();
  scan();

//  if(reading > 0){
//    Serial.println("Light");
//    digitalWrite(led,HIGH);
//    delay(500);
//  }
//  else {
//    digitalWrite(led,LOW);
//  }
  delay(2000);
}
