

void setup()
{
pinMode(3, OUTPUT);//LED +

pinMode(4, OUTPUT);//LED GND 
digitalWrite(4,LOW); 

pinMode(7, OUTPUT);//GND
digitalWrite(7,LOW); 

pinMode(8, OUTPUT);//VCC 
digitalWrite(8,HIGH);

pinMode(9, INPUT); //OUT
}

void loop()
{

  if (digitalRead(9)==HIGH)
  {
    digitalWrite(3,HIGH);
  } else {
    digitalWrite(3,LOW);
  }
}
