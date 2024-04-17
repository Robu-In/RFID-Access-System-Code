#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h> // Include Servo library

#define SS_PIN 10
#define RST_PIN 9

byte readCard[4];
String MasterTag = "Add Your Tag ID Here";
String tagID = "";

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myServo; // Create a servo object

void setup() 
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();

  Serial.println("--------------------------");
  Serial.println(" Access Control ");
  Serial.println("Scan Your Card>>");

  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);

  myServo.attach(2); // Attach servo to pin 2
}


void loop() 
{
  while (getID()) {
    if (tagID == MasterTag){
      Serial.println(" Access Granted!");
      Serial.println("--------------------------");
      digitalWrite(3,HIGH);
      controlServo(); // Call function to control servo
      digitalWrite(4,LOW);
      delay(2000);
      digitalWrite(3,LOW);
    } else{
      Serial.println(" Access Denied!");
      Serial.println("--------------------------");
      digitalWrite(3,LOW);
      digitalWrite(4,HIGH);
      delay(2000);
      digitalWrite(4,LOW);

    }
    delay(2000);
    Serial.println(" Access Control ");
    Serial.println("Scan Your Card>>");
  }
}

boolean getID() 
{
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return false;
  }
  tagID = "";
  for (uint8_t i = 0; i < 4; i++) {
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}

void controlServo() {
  // Control servo movement here
  myServo.write(0); 
  delay(2000); // Wait for 2 second
  myServo.write(90);
}
