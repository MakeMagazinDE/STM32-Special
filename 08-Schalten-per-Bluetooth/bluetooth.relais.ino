/*
  Bluetooth Kommunikation + Relais-Modul
  App: https://play.google.com/store/apps/details?id=com.tech.maxclub.bluetoothme
*/

#include <HardwareSerial.h>

const String App[] = {"btn1", "btn2", "btn3", "btn4", "btn5", "btn6"};      // https://arduinogetstarted.com/de/reference/arduino-string-object
const uint16_t Pins[] = {PB9, PB8, PB7, PB6, PB5, PB4};
const uint8_t anz = 6;
unsigned long previousMillis = 0;  // will store last time LED was updated

HardwareSerial Serial3 (PB11, PB10);    // BT-Modul

void setup() 
{
  Serial3.begin (9600);     // BT-Modul
  Serial.begin (115200);    // PC

  for (uint8_t i = 0; i < anz; i++)     // Init all Buttons with Off
  {
    pinMode (Pins[i], OUTPUT);
    digitalWrite (Pins[i], HIGH);     // Off
    Serial3.print (App[i]);Serial3.print ("/0\n");      
  }
  Serial3.print ("temp/0\n");
}

void loop() 
{
  if (Serial3.available() > 0) 
  { // Check if data is available
    String data = Serial3.readString(); // Read all available data
    Serial.println(data); // Send data back

    for (uint8_t i = 0; i < anz; i++)
    {
      if (data.startsWith(App[i]) && data.endsWith("/1\n"))     // Bsp.: "btn1/1\n" from App
         digitalWrite (Pins[i], LOW);     // Relais ON
      if (data.startsWith(App[i]) && data.endsWith("/0\n"))
         digitalWrite (Pins[i], HIGH);     // Relais OFF
    }
  }
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 2000)   // every n ms
  {
    previousMillis = currentMillis;

    uint16_t pt1000 = analogRead (PA0);     // Analogwert PT1000
    double Upt = 3.3/1024.0 * pt1000;       // Spannung PT
    double Ir1 = (3.3 - Upt) / 1000.0;        // Strom anhand R1
    double Rpt = Upt / Ir1;                   // R PT
    double Temp = (Rpt - 1000) / 3.85;              // mittlere Temperaturbeiwert über die Spanne 0 … 100 °C
    Serial3.print ("temp/"); Serial3.print (Temp, 0); Serial3.print ("\n");   // no decimal values
    Serial.print ("temp/"); Serial.print (Temp,0); Serial.print ("\n");
  }
}
