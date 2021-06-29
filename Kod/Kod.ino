#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.







String uids[] = {"29 C2 07 5E", "31 C2 07 5E"};







int rele = 2;
int led = 3;
int bzuciak = 4;

//
int button = A5;
//

void setup() 
{
  pinMode(rele, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(bzuciak, OUTPUT);

  //
  pinMode(button, INPUT);
  //
  
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();

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
  Serial.print("UID tag :");
  String content= "";
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
  /*content = " 29 C2 07 5E";*/
  content.toUpperCase();
  if (/*digitalRead(button) == HIGH*/isInAuthorized(content)) 
  {
    Serial.println("Authorized access");
    Serial.println();

    
    signalizacia(true);
    otvoritStroj();
  }
  
  else   {
    Serial.println(" Access denied");
    
    digitalWrite(rele, LOW);
    signalizacia(false);
    
    
  }
  
  delay(3000);
} 

bool isInAuthorized(String value) {
  for (int uidIndex = 0; uidIndex < sizeof(uids)/sizeof(uids[0]); uidIndex++) {
    if (value.substring(1) == uids[uidIndex]){ // Make sure you change this with your own UID number 
      return true;
    }
  }
  return false;
}

void signalizacia(bool overenie) {
  if (overenie) {
    digitalWrite(led, HIGH);
    for (int i = 0; i < 25; i++) {
      digitalWrite(bzuciak, HIGH);
      delay(2);
    
      digitalWrite(bzuciak, LOW);
      delay(1);
    }
    digitalWrite(led, LOW);
    delay(25);
    digitalWrite(led, HIGH);
    for (int i = 0; i < 50; i++) {
      digitalWrite(bzuciak, HIGH);
      delay(1);
    
      digitalWrite(bzuciak, LOW);
      delay(1);
    }
    digitalWrite(led, LOW);
    return;
  }

  digitalWrite(led, HIGH);
  for (int i = 0; i < 50; i++) {
    digitalWrite(bzuciak, HIGH);
    delay(2);
    
    digitalWrite(bzuciak, LOW);
    delay(1);
  }
  digitalWrite(led, LOW);
  delay(50);
  digitalWrite(led, HIGH);
  for (int i = 0; i < 50; i++) {
    digitalWrite(bzuciak, HIGH);
    delay(2);
    
    digitalWrite(bzuciak, LOW);
    delay(1);
  }
  digitalWrite(led, LOW);
}

void blikat(bool overenie) {
  if (overenie) {
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(250);
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    return;
  }
  digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    delay(400);
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
}

void otvoritStroj() {
  digitalWrite(rele, HIGH);
  delay(500);
  digitalWrite(rele, LOW);
}
