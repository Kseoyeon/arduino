int sensor = 32;
int motion;

void setup() {
  pinMode(sensor, INPUT);
  Serial.begin(115200);
}

void loop() {
  motion = digitalRead(sensor);
  if(motion == HIGH){
    Serial.println(motion);
  }
  else{
    Serial.println(motion);
  }
  delay(100);
}

//int inputPIN = 32;    
//int statusPIR = 0;    
//int valueRead = 0;  
//  
//void setup() {  
//   pinMode(inputPIN, INPUT);   
//   Serial.begin(115200);    
//}  
//  
//void loop(){  
//  valueRead = digitalRead(inputPIN);    
//  if ((valueRead == HIGH)  &&  (statusPIR == LOW)) {    
//      Serial.println("Motion detected!");    
//      statusPIR = HIGH;    
//   }  
//  else{  
//      if((valueRead == LOW) && (statusPIR == HIGH)){  
//        Serial.println("Motion ended!");    
//        statusPIR = LOW;  
//      }  
//   }  
//   delay(1000);  
//}  
