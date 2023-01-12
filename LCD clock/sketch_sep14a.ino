#include <LiquidCrystal.h>

unsigned int h,m,s = 00;

//LiquidCrytral(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup(){
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Arduino");
}

void loop(){
    if(h < 10)
    {
      lcd.setCursor(0,1);
      lcd.print("0");
    }
    if(m < 10)
    {
      lcd.setCursor(3,1);
      lcd.print("0");
    }
    if(s < 10)
    {
      lcd.setCursor(6,1);
      lcd.print("0");
      lcd.setCursor(7,1);
      lcd.print(s);
    }
    if(s >= 10)
    {
      lcd.setCursor(6,1);
      lcd.print(s);
    }
    if(s > 59)
    {
      m++;
      s = 0;
    }

    if(m > 59)
    {
      h++;
      m = 0;
    }
    if(h = 23)
    {
      return 0;
    }
    lcd.setCursor(1,1);
    lcd.print(h);
    lcd.print(":");

    lcd.setCursor(4,1);
    lcd.print(m);
    lcd.print(":");
    ++s;
    delay(1000);
    
}
