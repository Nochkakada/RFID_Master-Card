#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#define SS_PIN 10
#define RST_PIN 9
#define LED_R 4 //define red LED
#define LED_B 5 //define red LED
#define RELAY A0 //relay pin
#define BUZZER 2 //buzzer pin
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000
Servo door;
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
void setup() {
  Serial.begin(9600); // Initiate a serial communication
  SPI.begin(); // Initiate SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522
  pinMode(LED_R, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  pinMode(13,OUTPUT);
  digitalWrite(RELAY, LOW);
  digitalWrite(A1, LOW);
  Serial.println("Put your card to the reader...");
  door.attach(6);
  door.write(0);
}
void loop() {
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
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  
  if (content.substring(1) == "63 AA EF 11") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    digitalWrite(LED_B, HIGH);
    digitalWrite(RELAY, HIGH);
    digitalWrite(A1, LOW);
    delay(ACCESS_DELAY);
    digitalWrite(RELAY, LOW);
    digitalWrite(LED_B, 0);
    digitalWrite(A1, LOW);
    delay(3000);
    door.write(90);
    delay(2000);
    door.write(0);
  }
  else {
    Serial.println(" Access denied");
    digitalWrite(A1, LOW);
    digitalWrite(RELAY, LOW);
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    delay(DENIED_DELAY);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);  door.write(0);
  }

}