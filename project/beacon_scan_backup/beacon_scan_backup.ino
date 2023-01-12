#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#define DIST_S 20*58.2 // 200cm로 제한
#define PING_INTERVAL 20 // Milliseconds // 3개의 경우 100ms

BLEScan* pBLEScan;

const char* ssid     = "CCIT";
const char* password = "rkdtjdus";

int scanTime = 2; //In seconds

char* array = new char[60];
int reading = 0;
uint8_t LED[5] = {23,22,19,18,21};
uint8_t Sensor[5] = {36,39,34,35,32};
uint8_t trig[5]={33,26,14,17,0};
uint8_t echo[5]={25,27,12,16,2};
int val[5];
long dist_r[5];
int RS = 0;
uint8_t currentSensor = 0;  
unsigned long pingTimer[5];
unsigned long Curr_sensor, Curr_micro;
unsigned long Prev_sensor[4];
unsigned long Prev_micro[4];

int count;
boolean led, ship_state;
boolean ledval[5];
boolean ship[5];
boolean curship[5];
boolean preship[5];
int delaytime = 10000;

String jsondata;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      char *pHex = BLEUtils::buildHexData(nullptr, (uint8_t*)advertisedDevice.getManufacturerData().data(), advertisedDevice.getManufacturerData().length());
      array = pHex; // UUID 저장
      Serial.println(pHex); 
      Serial.println(array[0]);
      Serial.println(advertisedDevice.getRSSI());
      RS = advertisedDevice.getRSSI();
      for(int i = 1; i <= 5; i++){
        if(array[0] == '4' && array[15] == char(i) && RS >= -65){
          Serial.println("Y");
          ship[i] = 1;
          reading++;
        }
        else{
          ship[i] = 0;
        }
      }
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    }
};

void setup() {
  Serial.begin(115200);
  for(int i = 0; i <= 4; i++){
    pinMode(LED[i],OUTPUT);
    pinMode(Sensor[i],INPUT);
    pinMode(trig[i],OUTPUT);
    pinMode(echo[i],INPUT);
    digitalWrite(LED[i], HIGH);
  }
  
  for(int i = 0; i < 5; i++){
    Prev_sensor[i] = millis();
    Prev_micro[i] = millis();
  }
  
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println(" CONNECTED");
  
  for (uint8_t i = 1; i < 5; i++)
  pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
}

void loop() {
  reading = 0;
  getjson("fdfd");
  if(led == 1){
    for(int i = 0; i < 5; i++){
      digitalWrite(LED[i], LOW);
    }
  }
  else if(led == 0){
    for(int i = 0; i <= 5; i++){
      preship[i] = ship[i];
    }
    
    Serial.println("Start scanning...");
      initBle();
      scan();
      
    for(int i = 0; i <= 5; i++){
      curship[i] = ship[i];
    }
    
    for(int i = 0; i <= 5; i++){
      if((curship[i] == 0) && (preship[i] != curship[i])){
        getjson("host");
        if(ship_state == 1){
          jsonset_ship(i, 0);
          postjson("host");
        }
      }
      if((curship[i] == 1) && (preship[i] != curship[i])){
        getjson("host");
        if(ship_state == 0){
          jsonset_ship(i, 1);
          postjson("host");
        }
      }
    }
    
    if(reading > 0){ // 비콘 감지시 LED ON
          Curr_micro = millis();    
          for(int i=0; i<=4; i++){   //초음파센서
            if (millis() >= pingTimer[i]) {
              pingTimer[i] += PING_INTERVAL * 5;
                if (i == 0 && currentSensor == 4)        
                   oneSensorCycle();      
                currentSensor = i;           
                dist_r[i] = trig_ultra(trig[i],echo[i]);
            }
          }
          for(int i = 0; i <= 4; i++) {
            if ( 1 > dist_r[i] || 10 <= dist_r[i]) {
              digitalWrite(LED[i], LOW);
            }
            else if ( 0 < dist_r[i] && 10 > dist_r[i]) {
              digitalWrite(LED[i], HIGH);
              for(int j = 0; j <= 4; j++){ // 인체감지센서 값 저장
                val[j] = digitalRead(Sensor[j]);
              }
              for(int i = 0; i <= 4; i++){
                Curr_sensor = millis();
                if(val[i] == HIGH){
                  digitalWrite(LED[i], LOW);
                }
                ledval[i] = digitalRead(LED[i]);
              }
              for(int i = 0; i < 5; i++){
                if(ledval[i] == LOW){
                  if(Curr_sensor - Prev_sensor[i] > 3000) {
                    digitalWrite(LED[i], HIGH);
                    Prev_sensor[i] = Curr_sensor;
                  }
                }
              }
            }
          }
    }
    else {
      // 인체감지센서  
      for(int j = 0; j <= 4; j++){ // 인체감지센서 값 저장
        val[j] = digitalRead(Sensor[j]);
      }
      for(int i = 0; i <= 4; i++){
        Curr_sensor = millis();
        if(val[i] == HIGH){
          digitalWrite(LED[i], LOW);
        }
        ledval[i] = digitalRead(LED[i]);
      }
      for(int i = 0; i < 5; i++){
        if(ledval[i] == LOW){
          if(Curr_sensor - Prev_sensor[i] > 3000) {
            digitalWrite(LED[i], HIGH);
            Prev_sensor[i] = Curr_sensor;
          }
        }
      }
    }
  }
}

long trig_ultra(int TRIG,int ECHO){
  long dist;
  digitalWrite(TRIG, LOW); 
  delayMicroseconds(2); 
  digitalWrite(TRIG, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  dist = pulseIn(ECHO, HIGH,DIST_S)/58.2;
  return(dist);
}

void oneSensorCycle() { 
  for (uint8_t i = 0; i < 5; i++) {
      Serial.print(dist_r[i]);
      Serial.print("cm");
      Serial.write(9); //tab
  }
  Serial.println();
}

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

void getjson(String host){
  HTTPClient sss;    //Declare object of class HTTPClient
  Serial.print("Request Link:");
  Serial.println(host);
  sss.begin(host);     //Specify request destination
  int httpCode = sss.GET();            //Send the request
  String payload = sss.getString();    //Get the response payload from server
  Serial.print("Response Code:"); //200 is OK
  Serial.println(httpCode);   //Print HTTP return code
  Serial.print("Returned data from Server:");
  Serial.println(payload);    //Print request response payload
  if(httpCode == 200){ 
    //const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(1) + 10;
    const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(3)+ 81;
    DynamicJsonDocument jsonBuffer(capacity);
    deserializeJson(jsonBuffer, payload);
    int led_server = jsonBuffer[0]["led"];
    int state_server = jsonBuffer[0]["state"];
    led_server=led;
    state_server=ship_state;
  }
  else{
    Serial.println("Error in response");
  }
  sss.end();  //Close connection 
} 

void jsonset_ship(int num, int state){
  jsondata= "";
  StaticJsonDocument<200> jsonBuffer;
  jsonBuffer["port_num"] = num; //배 번호
  jsonBuffer["state"] = state; //입출항 상태
  serializeJson(jsonBuffer,jsondata);
  Serial.println(jsondata);
}

void postjson(String url){
  HTTPClient http;
  http.begin(url);        // 서버 주소
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(jsondata);
}
