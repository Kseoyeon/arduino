// Demonstrates the use of Laser obstacle detection.

// The Arduino circuit connection for 7219:
// MAX7219 DIN pin to Arduino digital pin 4
// MAX7219 LOAD pin to Arduino digital pin 3
// MAX7219 CLOCK pin to Arduino digital pin 2

// Laser Module digital output to Arduino digital pin 5
 
// Name:- M.Pugazhendi
// Date:-  24thAug2016
// Version:- V0.1
// e-mail:- muthuswamy.pugazhendi@gmail.com


unsigned int count = 0x00;
unsigned int count_one = 0x00;
unsigned int previous = 0x00;

int detect_obstacle=8;

void setup()
{
  pinMode(detect_obstacle,INPUT);
 
    Serial.begin(9600);
    Serial.println("Laser Obstacle detection / counter");
    // 2 Second delay
    delay(2000);
  
}
void loop()
{
  if(digitalRead(detect_obstacle)==LOW)
  {  
            Serial.println("Obstruct");
          
                     
  }
  else
  {
      
        Serial.println("Free");
        //print the counted value
        Serial.print("Count =");
        
      
  }  
  delay(100);
}

/**************************************************************************/
/*!
 * \brief Coverts hex into BCD
 *
 * This function will coverts hex into BCD
 *
 * \param[in] byte
 * \param[out] byte
 * \return Nill
 * 
 */
/**************************************************************************/
unsigned int HexToBCD(unsigned int number)
{
    unsigned char i=0;
    unsigned int k = 0;

    while(number)
    {
        k = ( k ) | ((number%10) << i*4);
        number = number / 10;
        i++;
    }

    return(k);
}
