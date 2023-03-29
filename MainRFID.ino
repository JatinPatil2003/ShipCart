#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
//  Serial.println("Approximate your card to the reader...");
//  Serial.println();
  pinMode(4,OUTPUT);
}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
//  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
//     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
//     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : "."));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
//  Serial.println();
  content.toUpperCase();
  Serial.println(content.substring(1));
  if (content.substring(1) == "A0 BE F1 28") //change here the UID of the card/cards that you want to give access
  {
//    Serial.println("Authorized access");
    digitalWrite(4,HIGH);
//    Serial.println();
    delay(100);
    digitalWrite(4,LOW);
    delay(2000);
  }
 
 else   {
//    Serial.println(" Access denied");
    digitalWrite(4,HIGH);
    delay(100);
    digitalWrite(4,LOW);
    delay(50);
    digitalWrite(4,HIGH);
    delay(100);
    digitalWrite(4,LOW);
    delay(1000);
  }
}
