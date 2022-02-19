void setup() {
pinMode(10,INPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
delay(60);
int sensorValue = analogRead(A0);
float voltage= sensorValue * (5.0 / 1023.0);
Serial.println(voltage);
}
