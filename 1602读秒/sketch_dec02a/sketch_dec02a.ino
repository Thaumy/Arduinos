#include <LiquidCrystal.h>
#include <stdio.h>
#include <string.h>
//设置io
LiquidCrystal lcd(12, 11, 10, 9, 8, 13);

int t1,t2,t3,t4,st;

void setup()
{
  lcd.begin(16,2);
  lcd.clear(); //清屏
}
void timmer(){
  st=millis()/1000;
  
  t1=st%10;
  t2=st/10%10;
  if(t2==6){t2=0;}
  t3=st/60%10;
  if(t3==6){t3=0;}
  t4=st/3600%10;
}
void loop ()
{  
  timmer();
     lcd.setCursor(0, 0) ;
     lcd.print(t4);
     lcd.print(t3);
     lcd.print(t2);
     lcd.print(t1);
     lcd.setCursor(0, 1) ;
     lcd.print("");
}

