#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// Pre jednoduchšie pridavanie vacerých čipov dajte pred každý riadok čo má na konci //*|*// znak // a pre zrušenie ho zas

// 1. Získajte kópiu tohto kodu (github kopia - https://github.com/MatoXsvk/Arduino_rfid), dalej uids, ktore chcete aby boli povolene (aj staré - spravte krok 2, potom po privedeni 5V na pin A0 vám to na okno vypíše staré uid kódy, ktoré môžete použiť)
// 2. V ľavom hornom rohu otvorte lupu
// 3. Priložte čip ktorý chcete pridať
// 5. Skopírujte informáciu za vetou 'UID tag:' od prvého " do " do riadku kde ukazuje šípka pred } a dajte čiarku a medzeru :  String uids[] = {"B7 35 2E D6", ... , "novy uid", };
// 6. Uložte a nahrajte šípkou vľavo hore

// |
// V
String uids[] = {};






int rele = 2;
int bzuciak = 3;
int ledSpravne = 4;
int ledZle = 5;

int codesBTN = A0;

void setup() 
{
  pinMode(rele, OUTPUT);
  pinMode(ledSpravne, OUTPUT);
  pinMode(ledZle, OUTPUT);
  pinMode(bzuciak, OUTPUT);

  pinMode(codesBTN, INPUT);
  
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();

}
void loop() 
{
  if (digitalRead(codesBTN) == HIGH) printUIDS();
  
  // Look for new cards and select one of the cards
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  //Show UID on serial monitor
  Serial.print("UID tag : "); //*|*//
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  content.toUpperCase();
  
  Serial.print("\"");
  Serial.print(content.substring(1));
  Serial.print("\", ");
  
  Serial.println(); //*|*//
  Serial.print("Message : "); //*|*//
  
  if (isInAuthorized(content)) 
  {
    Serial.println("Authorized access"); //*|*//
    Serial.println(); //*|*//
    
    signalizacia(true);
    otvoritStroj();
  } 
  else 
  {
    Serial.println(" Access denied"); //*|*//
    Serial.println(); //*|*//
    
    digitalWrite(rele, LOW);
    signalizacia(false);    
  }
  delay(3000);
} 

bool isInAuthorized(String value) 
{
  for (int uidIndex = 0; uidIndex < sizeof(uids)/sizeof(uids[0]); uidIndex++) 
  {
    if (value.substring(1) == uids[uidIndex]) return true; // Make sure you change this with your own UID number 
  }
  return false;
}

void signalizacia(bool overenie) 
{
  if (overenie) 
  {
    digitalWrite(ledSpravne, HIGH);
    
    for (int i = 3; i >= 1; i--) 
    {
      int timeDuration = 25;
      if (i == 1)timeDuration = 45;
      for (int j = 0; j < timeDuration; j++) {
        digitalWrite(bzuciak, HIGH);
        delay(i);
      
        digitalWrite(bzuciak, LOW);
        delay(1);
      }
      delay(25);
    }
    
    digitalWrite(ledSpravne, LOW);
    return;
  }

  digitalWrite(ledZle, HIGH);
  for (int i = 0; i < 50; i++) 
  {
    digitalWrite(bzuciak, HIGH);
    delay(1);
    
    digitalWrite(bzuciak, LOW);
    delay(1);
  }
  delay(50);
  for (int i = 0; i < 100; i++) 
  {
    digitalWrite(bzuciak, HIGH);
    delay(3);
    
    digitalWrite(bzuciak, LOW);
    delay(1);
  }
  digitalWrite(ledZle, LOW);
  
}

void otvoritStroj() 
{
  digitalWrite(rele, HIGH);
  delay(500);
  digitalWrite(rele, LOW);
}

void printUIDS() 
{
  Serial.println("Stare uid kody: ");
  
  for (int uidIndex = 0; uidIndex < sizeof(uids)/sizeof(uids[0]); uidIndex++)
  {
    Serial.print("\"");
    Serial.print(uids[uidIndex]);
    Serial.print("\", ");
  }
  Serial.println();
  delay(2000);
}
