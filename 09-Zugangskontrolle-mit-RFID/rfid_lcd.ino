/*
  RFID MFRC522 tag reader with HD44780 LCD 
*/

#include <LiquidCrystal_I2C.h>    // https://github.com/arduino-libraries/LiquidCrystal
#include "MFRC522.h"              // https://github.com/miguelbalboa/rfid

const uint8_t cols = 20;
const uint8_t rows = 4;

LiquidCrystal_I2C lcd(0x27, cols, rows);  // set the LCD address to 0x27 (or 0x3F) 
MFRC522 mfrc522(PA4, -1);   // Konstruktor, CS-Pin, kein Reset-Pin

const char titel[] = "== RFID-Demo ==";
const uint8_t anz = 3;   // Anzahl IDs
const uint32_t TagsOK[anz] = {0xE11E7A21, 0xA1FF98C7, 0x527C8DA0};
const char* User[anz] ={"Peter", "Hanna", "Toni"};
uint8_t i = 0;

void setup()
{
  lcd.init();   // initialize the LCD 
  lcd.backlight();  // activate LED
  lcd.clear(); delay (10);  // wait for clear
  lcd.setCursor(2,0); lcd.print(titel);
  lcd.setCursor(0,2); lcd.print("Tag vorhalten");

  SPI.begin();
  mfrc522.PCD_Init();   // initialize RFID
}

void loop()
{
  if (!mfrc522.PICC_IsNewCardPresent())   // no card
    return;
  if (!mfrc522.PICC_ReadCardSerial())   // Select one of the cards if more than 1
    return;

  lcd.clear(); delay (10);
  lcd.setCursor(2,0); lcd.print(titel);
  lcd.setCursor(0,1); lcd.print("UID: ");

  uint32_t uid = 0;
  for (i = 0; i < mfrc522.uid.size; i++)   
    uid |= mfrc522.uid.uidByte[i] << (24 - 8 * i);  // build UID from Bytes

  lcd.print(uid < 0x10000000 ? "0" : "");    // leading zero
  lcd.print(uid, HEX);

  for (i = 0; i < anz; i++)
  {
    if (uid == TagsOK[i])
    {
      lcd.setCursor(0, 3);
      lcd.print ("Name: ");
      lcd.print (User[i]);
      break;
    }
    else
    {
      lcd.setCursor(2, 3);
      lcd.print ("** Unbekannt **");
      break;
    }
  }
  delay(1000);
}



