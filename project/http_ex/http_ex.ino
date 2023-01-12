void jsonset(){
  jsondata= "";
  //StaticJsonBuffer<200> jsonBuffer;
  StaticJsonDocument<200> jsonBuffer;
  //JsonObject& root = jsonBuffer.createObject();
  jsonBuffer["onoff"] = power; //Put Sensor value
  jsonBuffer["speed"] = motor;
  jsonBuffer["timer"] = timer; //Reads Flash Button Status
  serializeJson(jsonBuffer,jsondata);
  Serial.println(jsondata);
}

void getjson(){
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
    int onoff = jsonBuffer[0]["onoff"];
    int speeds = jsonBuffer[0]["speed"];
    int times = jsonBuffer[0]["timer"];
    //Serial.println(speeds);
    //Serial.println(onoff);
    //Serial.println(timer);
    power_server=onoff;
    Motor_server=speeds;
    timer=times;
  }
  else{
    Serial.println("Error in response");
  }
  sss.end();  //Close connection 
 
 // delay(5000);  //GET Data at every 5 seconds
} 

void postjson(){
  HTTPClient http;
  http.begin(server);        // 서버 주소
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(jsondata);
}
