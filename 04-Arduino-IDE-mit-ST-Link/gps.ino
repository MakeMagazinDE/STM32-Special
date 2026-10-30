#include <HardwareSerial.h>
#include <Adafruit_GPS.h>       // https://github.com/adafruit/Adafruit_GPS

// Serial or Serial1: (PA10, PA9);    // Rx, Tx
HardwareSerial Serial2 (PA3, PA2);
// HardwareSerial Serial3 (PB11, PB10);

Adafruit_GPS GPS(&Serial2);     // Connect to the GPS on the hardware port
uint32_t timer = millis();

void setup() {
  Serial.begin (57600);   // PC
  GPS.begin(9600);        // GPS
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  
  pinMode (PC13, OUTPUT);      // LED
}

void loop() 
{
  char c = GPS.read();
  digitalWrite(PC13, HIGH);   // off

  if (GPS.newNMEAreceived())
  {
    Serial.print(GPS.lastNMEA()); 
    if (GPS.parse(GPS.lastNMEA()))
    {
      if (millis() - timer > 2000)      // every 2000 ms 
      {
        timer = millis(); // reset the timer
        digitalWrite(PC13, LOW);   // on

        if (GPS.fix)    // data is valid: print some info
        {
          Serial.print("\nLocation: ");
          Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
          Serial.print(", ");
          Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
          Serial.print("Speed (knots): "); Serial.println(GPS.speed);
          Serial.print("Angle: "); Serial.println(GPS.angle);
          Serial.print("Altitude: "); Serial.println(GPS.altitude);
          Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
          Serial.print("Antenna status: "); Serial.println((int)GPS.antenna); Serial.println();
          digitalWrite(PC13, HIGH);   // off
        }
        else  // no fix data
          Serial.println ("\n### NO FIX DATA ###\n\n");
      }
    }
  }
}
