#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

const char* ssid     = "ccit";
const char* password = "00000000503";

uint8_t LED[5] = {23,22,19,18,21};

//int led = 0;
int ship_state[2];
int shipnum[2];
int led[5];
unsigned long cur_time, pre_time;

String jsondata;

void setup() {
  Serial.begin(115200);
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
  pre_time = millis();
}

void loop() {
//  char a = Serial.read();
//  if(a == 'a'){
//    getjson("http://54cead1e.ngrok.io/list/1");
//    Serial.print(shipnum[0]);
//    Serial.print(" : ");
//    Serial.print(ship_state[0]);
//    Serial.write(9);
//    Serial.print(shipnum[1]);
//    Serial.print(" : ");
//    Serial.println(ship_state[1]);
//  }
//  else if(a == 'b'){
//    jsonset_ship(0005, 0, 5);
//    postjson("");
//  }
  cur_time = millis();
  
  if(cur_time - pre_time > 3000){
    getjson_led("http://192.168.0.29:3000/led");
    pre_time = cur_time;    
  }
  if(led[0] == 1){
    digitalWrite(LED[0], LOW);
    Serial.println("LED1 ON");
  } else if(led[0] == 0){
    digitalWrite(LED[0], HIGH);
  }
  if(led[1] == 1){
    digitalWrite(LED[1], LOW);
    Serial.println("LED2 ON");
  } else if(led[1] == 0){
    digitalWrite(LED[1], HIGH);
  }
  if(led[2] == 1){
    digitalWrite(LED[2], LOW);
    Serial.println("LED3 ON");
  } else if(led[2] == 0){
    digitalWrite(LED[2], HIGH);
  }
  if(led[3] == 1){
    digitalWrite(LED[3], LOW);
    Serial.println("LED4 ON");
  } else if(led[3] == 0){
    digitalWrite(LED[3], HIGH);
  }
  if(led[4] == 1){
    digitalWrite(LED[4], LOW);
    Serial.println("LED5 ON");
  } else if(led[4] == 0){
    digitalWrite(LED[4], HIGH);
  }
}

void getjson(String host){
  int state_server[2];
  int shipnum_server[2];
  HTTPClient sss;    //Declare object of class HTTPClient
  sss.begin(host);     //Specify request destination
  int httpCode = sss.GET();            //Send the request
  String payload = sss.getString();    //Get the response payload from server
  if(httpCode == 200){ 
    const size_t capacity = JSON_ARRAY_SIZE(2) + 2*JSON_OBJECT_SIZE(4) + 140;
    DynamicJsonDocument jsonBuffer(capacity);
    deserializeJson(jsonBuffer, payload);
    Serial.println(payload);
    state_server[0] = jsonBuffer[0]["state"];
    shipnum_server[0] = jsonBuffer[0]["port_num"];
    shipnum[0] = shipnum_server[0];
    ship_state[0] = state_server[0];
    state_server[1] = jsonBuffer[1]["state"];
    shipnum_server[1] = jsonBuffer[1]["port_num"];
    shipnum[1] = shipnum_server[1];
    ship_state[1] = state_server[1];
  }
  else{Serial.println("Error in response");}
  sss.end();  //Close connection 
} 

void getjson_led(String host){
  int led_server[5];
  HTTPClient sss;    //Declare object of class HTTPClient
  sss.begin(host);     //Specify request destination
  int httpCode = sss.GET();            //Send the request
  String payload = sss.getString();    //Get the response payload from server
  if(httpCode == 200){ 
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(5) + 30;
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
  else{Serial.println("Error in response");}
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
