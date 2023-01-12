#include <BLEDevice.h>
#include <BLEAdvertisedDevice.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#define DIST_S 20*58.2 // 200cm로 제한
#define PING_INTERVAL 30 // Milliseconds // 3개의 경우 100ms

BLEScan* pBLEScan;
const char* ssid     = "CCIT";
const char* password = "rkdtjdus";
int scanTime = 2; //In seconds

char* array = new char[30];
int reading = 0;
int LED[5] = {23,22,19,18,21};
int Sensor[5] = {36,39,34,35,32};
int trig[5]={33,26,14,17,0};
int echo[5]={25,27,12,16,2};
boolean val[5];
long dist_r[5];
int RS = 0;
int num;
int currentSensor = 0;  
unsigned long pingTimer;
unsigned long cur_time, pre_time;
unsigned long curr_time, prev_time;
unsigned long cu_time, pr_time;
unsigned long Curr_sensor, Curr_micro;
unsigned long Prev_sensor[5];
unsigned long Prev_micro[5];
unsigned long c_time;
unsigned long p_time[5];
long dist_b[5];
long dist_a[5];

int value;
int led[5];
int shipstate[5];
int shipnum[5];
int shipplace[5];
int ledval[5];
int ship[6];
int curship[6];
int preship[6];
int before[5];
int after[5];
int delaytime = 10000;
String jsondata;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      char *pHex = BLEUtils::buildHexData(nullptr, (uint8_t*)advertisedDevice.getManufacturerData().data(), advertisedDevice.getManufacturerData().length());
      array = pHex; // UUID 저장
      RS = advertisedDevice.getRSSI();
      for(uint8_t i = 0; i < 5; i++){
        char a = i + 49;
        if((array[0] == '4') && (array[15] == a) && (array[23] == a) && RS >= -65){
            ship[i] = 1;
            Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
            Serial.println(RS);
            reading++; 
        } else{
          ship[i] = 0;
        }
      }
    }
};

void setup() {
  Serial.begin(115200);
  for(uint8_t i = 0; i <= 4; i++){
    pinMode(LED[i],OUTPUT);
    pinMode(Sensor[i],INPUT);
    pinMode(trig[i],OUTPUT);
    pinMode(echo[i],INPUT);
    digitalWrite(LED[i], HIGH);
  }
  for(uint8_t i = 0; i < 5; i++){
    Prev_sensor[i] = millis();
    Prev_micro[i] = millis();
    p_time[i] = millis();
  }
  pingTimer = millis();
  pre_time = millis();
  prev_time = millis();
  pr_time = millis();
  
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    //Serial.print(".");
  }
  Serial.println(" CONNECTED");
}

void loop() {
  reading = 0;

  cur_time = millis();
  if(cur_time - pre_time > 2000){
    getjson_led("http://192.168.0.5:1000/off");
    pre_time = cur_time; 
  }
  
  if((led[0] == 1)||(led[1] == 1)||(led[2] == 1)||(led[3] == 1)||(led[4] == 1)){
    for(int i = 0; i < 5; i++){
      if(led[i] == 1){
        digitalWrite(LED[i], LOW);
      }
      else if(led[i] == 0){
        digitalWrite(LED[i], HIGH);
      }
    }
  }
  else if((led[0] == 0)&&(led[1] == 0)&&(led[2] == 0)&&(led[3] == 0)&&(led[4] == 0)){
    for(int i = 0; i < 5; i++){
      digitalWrite(LED[i], HIGH);
    }
    
    curr_time = millis();
    if(curr_time - prev_time > 5000){
      Serial.println("pre");
      for(int i = 0; i < 5; i++){
        preship[i] = ship[i];
        Serial.print(ship[i]);
      }
      Serial.println("");

      delay(100);
      Serial.println("scanning");
      
      initBle();
      scan();
//      scan();
      
      delay(100);
      
      Serial.println("cur");
      for(int i = 0; i < 5; i++){
        curship[i] = ship[i];
        Serial.print(ship[i]);
      }
      Serial.println("");
   
   if(reading > 0){ // 비콘 감지시 LED ON      
      
      Serial.println("before");
      for(int i = 0; i < 2; i++){
        ultra();
        for(int j = 0; j < 5; j++) {
          if( 1 > dist_r[j] || 13 <= dist_r[j]) {
            digitalWrite(LED[j], LOW);
            before[j] = 0;
          }
          else if( 0 < dist_r[j] && 13 > dist_r[j]){
            digitalWrite(LED[j], HIGH);
            before[j] = 1;
          }
//        Serial.print("Before ");
//        Serial.println(before[j]);
       }
       oneSensorCycle();
       delay(100);
      }
      
      //ulpe();
      delay(7000);

      Serial.println("after");
      for(int i = 0; i < 2; i++){
        ultra();
        for(int j = 0; j < 5; j++) {
          if( 1 > dist_r[j] || 13 <= dist_r[j]){
            digitalWrite(LED[j], LOW);
            after[j] = 0;
          }
          else if( 0 < dist_r[j] && 13 > dist_r[j]){
            digitalWrite(LED[j], HIGH);
            after[j] = 1;
          }
//        Serial.print("After ");
//        Serial.println(after[j]);
        }
        oneSensorCycle();
        delay(100);
      }
      
      for(int i = 0; i < 5; i++){
        if((before[i] == 0) && (after[i] == 1)){
          value = i + 1;  
          Serial.print("value ");
          Serial.println(value);  
        }
      }
      
      for(int i = 0; i < 5; i++){
        getjson("http://192.168.0.5:1000/list");
        if((preship[i] == 0) && (curship[i] == 1)){
          if(shipstate[i] == 0){
            jsonset_ship((i+1)*1000, 1, value);
            postjson("http://192.168.0.5:1000/list");
            Serial.print(value);
            Serial.write(9);
            Serial.println("IN");
          }
        }
      }
    }
    prev_time = curr_time;
   }
    Serial.println("beacon finish");
    delay(100);
    
    getjson("http://192.168.0.5:1000/list");

    delay(100);
    for(int i = 0; i < 5; i++){
      if((preship[i] == 1) && (curship[i] == 0)){
        if(shipstate[i] == 1){
          jsonset_ship((i+1)*1000, 0, 0);
          postjson("http://192.168.0.5:1000/list");
          Serial.println("OUT");
        }
      }
    }
//    delay(2000);
    //ulpe();
  }
  delay(100);
}

void ultra() {
  if(millis() >= pingTimer) { 
    pingTimer += PING_INTERVAL; 
    dist_r[num] = trig_ultra(trig[num],echo[num]);
    num++;      
  }
  if(num>4) {num=0;}
}

void ulpe() { //거리센서로 움직임 감지
  c_time = millis();
  ultra();
  for(int i = 0; i < 5; i++){
    dist_b[i] = dist_r[i];
  }
  ultra();
  for(int i = 0; i < 5; i++){
    dist_a[i] = dist_r[i];
    if(dist_b[i] != dist_a[i]){
      digitalWrite(LED[i], LOW); 
    }
  } 
  for(int i = 0; i < 5; i++){
    if(c_time - p_time[i] > 3000){
      digitalWrite(LED[i], HIGH);
      p_time[i] = c_time;    
    }
  }

}

long trig_ultra(uint8_t TRIG,uint8_t ECHO){
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

void person() {  //움직임 감지센서
  Curr_sensor = millis();
  for(uint8_t i = 0; i <= 4; i++){
    val[i] = digitalRead(Sensor[i]);
  }
  for(uint8_t i = 0; i <= 4; i++){
    if(val[i] == HIGH){
      digitalWrite(LED[i], LOW);
    }
    else {digitalWrite(LED[i], HIGH);}
    ledval[i] = digitalRead(LED[i]);
  }
  for(uint8_t i = 0; i < 5; i++){
    if(ledval[i] == LOW){
      if(Curr_sensor - Prev_sensor[i] > 5000){
        digitalWrite(LED[i], HIGH);
        Prev_sensor[i] = Curr_sensor;
      }
    }
  }
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
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
}

void getjson(String host){
  int state_server[5];
  int shipnum_server[5];
  int shipplace_server[5];
  HTTPClient sss;    //Declare object of class HTTPClient
  sss.begin(host);     //Specify request destination
  int httpCode = sss.GET();            //Send the request
  String payload = sss.getString();    //Get the response payload from server
  if(httpCode == 200){ 
    const size_t capacity = JSON_ARRAY_SIZE(5) + 5*JSON_OBJECT_SIZE(3) + 140;
    DynamicJsonDocument jsonBuffer(capacity);
    deserializeJson(jsonBuffer, payload);
    for(int i = 0; i < 5; i++){
      state_server[i] = jsonBuffer[i]["state"];
      shipnum_server[i] = jsonBuffer[i]["port_num"];
      shipplace_server[i] = jsonBuffer[i]["stop_num"];
      shipplace[i] = shipplace_server[i];
      shipstate[i] = state_server[i];
      shipnum[i] = shipnum_server[i];
    }
  }
  else{Serial.println("Error in response1");}
  sss.end();  //Close connection 
} 

void getjson_led(String host){
  int led_server[5];
  HTTPClient sss;    //Declare object of class HTTPClient
  sss.begin(host);     //Specify request destination
  int httpCode = sss.GET();            //Send the request
  String payload = sss.getString();    //Get the response payload from server
  if(httpCode == 200){ 
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(5) + 40;
    DynamicJsonDocument jsonBuffer(capacity);
    deserializeJson(jsonBuffer, payload);
    led_server[0] = jsonBuffer[0]["sea1"];
    led_server[1] = jsonBuffer[0]["sea2"];
    led_server[2] = jsonBuffer[0]["sea3"];
    led_server[3] = jsonBuffer[0]["sea4"];
    led_server[4] = jsonBuffer[0]["sea5"];
    led[0] = led_server[0];
    led[1] = led_server[1];
    led[2] = led_server[2];
    led[3] = led_server[3];
    led[4] = led_server[4];
  }
  else{Serial.println("Error in response2");}
  sss.end();  //Close connection 
} 

void jsonset_ship(int port, boolean state, uint8_t site){
  jsondata= "";
  StaticJsonDocument<200> jsonBuffer;
  jsonBuffer["port_num"] = port; //배 번호
  jsonBuffer["state"] = state; //입출항 상태
  jsonBuffer["stop_num"] = site; //자리 번호
  serializeJson(jsonBuffer,jsondata);
}

void postjson(String url){
  HTTPClient http;
  http.begin(url);        // 서버 주소
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(jsondata);
}
