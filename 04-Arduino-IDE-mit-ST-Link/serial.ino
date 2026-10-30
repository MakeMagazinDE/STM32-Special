#include <HardwareSerial.h>

HardwareSerial Serial (PB11, PB10); 

void setup() {
  Serial.begin (115200);   // PC
}

void loop() 
{
  int8_t i = -128;
  while (1)
  {
    Serial.print("var i: "); Serial.println(i);
    delay(250);
    i++;
  }
}
