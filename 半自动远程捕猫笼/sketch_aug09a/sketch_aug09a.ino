/*
 * IRrecvDemo
 * 红外控制，接收红外命令控制板载LED灯亮灭
 */

#include <IRremote.h>
#include <Servo.h>之间要有空格，否则编译时会报错。

int RECV_PIN = 11;
int LED_PIN = 13;

IRrecv irrecv(RECV_PIN);

decode_results results;

Servo myservo;//定义舵机变量名

void setup()
{
  Serial.begin(9600);
  
  myservo.attach(9);//定义舵机接口（9、10 都可以，缺点只能控制2 个）
  
  irrecv.enableIRIn(); // Start the receiver
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  
  myservo.write(90);
  delay(1000);
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    if (results.value =! 0) //开灯的值
    {
      myservo.write(150);
      delay(1000);
    }
    }
}
