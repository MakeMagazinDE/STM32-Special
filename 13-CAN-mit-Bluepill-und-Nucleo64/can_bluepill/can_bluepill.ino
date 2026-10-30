/* 
  CAN
  This code is for the Bluepill
  Build with STM32duino core
 */

#include <arduino.h>
#include <eXoCAN.h> 

#define bluePillLED PC13

uint16_t txMsgID = 0x0A0;
uint8_t txData[] = {0x00, 0x00, 0x11, 0x22, 0xab, 0xcd, 0xef, 0xff};
uint8_t txDataLen = 8;
int id, fltIdx;
uint8_t rxbytes[8];
eXoCAN can;

uint32_t last = millis();
uint8_t counter = 0;

void setup()
{
  pinMode(bluePillLED, OUTPUT);
  digitalWrite(bluePillLED, HIGH);      // LED off
  can.begin(STD_ID_LEN, BR125K, PORTA_11_12_XCVR); // 11b IDs, 250k bit rate, transceiver chip, portA pins 11,12
}

void loop()
{
  
  if ((millis() - 50) >= last)             // tx every x ms
  {
    last = millis();
    txData[0] = counter;    // replace first byte with counting value 0..255
    can.transmit(txMsgID, txData, txDataLen);
    counter++;
  }

  if (can.receive(id, fltIdx, rxbytes) > -1) // poll for rx
  {
    if ((id == 0x010 || id == 0x0B0) && rxbytes[0] == 1)    // check for 2 IDs
      digitalWrite(bluePillLED, LOW);       // LED on
    if ((id == 0x010 || id == 0x0B0) && rxbytes[0] == 0)
      digitalWrite(bluePillLED, HIGH);      // LED off
  }
}