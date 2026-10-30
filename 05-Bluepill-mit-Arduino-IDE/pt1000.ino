/*
  PT1000 an PA0
*/

void setup() 
{
  Serial.begin (115200);
}

void loop() 
{
  uint16_t pt1000 = analogRead (PA0);     // Analogwert PT1000
  double Upt = 3.3/1024.0 * pt1000;       // Spannung PT
  double Ir1 = (3.3 - Upt) / 1000.0;        // Strom anhand R1
  double Rpt = Upt / Ir1;                   // R PT
  double Temp = (Rpt - 1000) / 3.85;              // mittlere Temperaturbeiwert über die Spanne 0 … 100 °C

  Serial.print ("PT1000: ");
  Serial.print (pt1000);
  Serial.print ("\t Upt: ");
  Serial.print (Upt, 3);
  Serial.print (" V\t I: ");
  Serial.print (Ir1 * 1000.0, 3);
  Serial.print (" mA\t Rpt: ");
  Serial.print (Rpt);
  Serial.print (" Ohm\t Temp: ");
  Serial.print (Temp);
  Serial.print (" *C");

  Serial.println();
  delay(1000);
}
