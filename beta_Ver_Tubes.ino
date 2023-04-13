#include <Wire.h>
#include "SPI.h"
#include "MFRC522.h"
#include <Servo.h>

Servo myservo;

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

int attempts = 0; // counter for successful attempts

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  myservo.attach(6);
  Serial.println("I am waiting for card...");
  
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i != 3 ? ":" : "");
  }
  strID.toUpperCase();
  Serial.print("Tap card key: ");
  Serial.println(strID);
  delay(1000);

  if (strID.indexOf("C6:B6:D8:29") >= 0) {  // put your own tap card key
    myservo.write(0);
    delay(1);
    Serial.println("********************");
    Serial.println("**Authorised access**");
    Serial.println("********************");

    delay(1500);

    attempts++;
    if (attempts % 2 == 0) {  // every even attempt
      myservo.write(0);
    } else {  // every odd attempt
      myservo.write(0);
    }
  } else {
    Serial.println("****************");
    Serial.println("**Access denied**");
    Serial.println("****************");

    delay(1500);
  }
}
