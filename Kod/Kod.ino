#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.



/*
    Pre jednoduchšie pridávanie viacerých čipov pred každý riadok, ktorý na konci obsahuje //|*|*|// dajte znak: //
    príklad:  Serial.print("\nMessage : "); //|*|*|//  ->   //Serial.print("\nMessage : "); //|*|*|//
    Pre zrušenie zase // z daných riadkov odstránte


     Vkladanie nových čipov:
      1. Získajte kópiu tohto kódu (GitHub kopia - https://github.com/MatoXsvk/Arduino_rfid), ďalej 'UID' kódy, ktoré chcete aby boli povolené (aj staré - spravte krok 2, potom po privedení 5V na pin A0 vám to na okno vypíše staré uid kódy, ktoré môžete použiť)
      2. Pripojte dosku arduino k počítaču pomocou kábla USB-B
      3. Otvorte kópiu kódu v editore alebo vytvorte novy projekt v arduino editore (vytvorenie projektu: tlačidlo v hornej lište s dokumentom)
      4. Na hormnom paneli rozkliknite "Nástroje" a za "Dosku" zadajte "Arduino UNO", a v "Port" vyberte možnosť, za ktorou je (Arduino UNO)
      5. Ak je zapajané prvý krát(ak nie sú priložene knižnice), na paneli rozkliknite "Projekt" > "Zahrnúť knižnice" > "Pridať .zip knižnicu" a vyberte .zip súbor priložený k projektu (rfid-master.zip)
      6. Na arduino doske vypojte kábel z 8. pinu!
      7. V ľavom hornom rohu editora Arduino stlačte tlačidlo lupy
      8. Stlačte tlačidlo šípky aby sa kod nahral na dosku Arduino
      9. Čip, ktorý chcete pridať, priložte k čítačke
      10. Skopírujte informáciu za vetou 'UID tag:' medzi úvodzovkami a vložte ju za ostatné kódy vo forme "XX XX XX XX" a oddeľujte ich čiarkou. Ak doposiaľ nie je uložený žiadny kód, tak ju vložte medzi kučeravé zátvorky: {}
      11. Uložte, nahrajte kód tlačidlom s šípkou vľavo hore v editore arduino.
      12. Znovu zapojte 8. pin

*/

// |
// V
String uids[] = {
  "B7 35 2E D6", "89 A2 1D 1F", "9C A1 99 2F", "C7 EE 2C D6", "2C 63 9A 2F",
  "27 EB CF D6", "C7 DD C8 D6", "BC C8 B1 2F", "FC E1 AF 2F", "D7 E7 C7 D6",
  "8C B3 B9 2F", "57 69 32 D6", "17 42 CC D6", "97 BC C7 D6", "F7 0E 36 D7",
  "47 69 66 D6", "C7 D7 4E D7", "A7 E7 CF D6", "DC F5 FF 30", "57 8A CE D6",
  "B7 7F C4 D6", "E7 5A D4 D6", "3C C2 0D 2F", "57 D6 C6 D6", "47 BD CF D6",
  "37 29 60 D7", "47 2E D3 D6", "4C 6D FD 30", "47 98 64 D6", "6C 5F B2 2F",
  "C7 1B D5 D6", "2C 28 AE 2F", "67 AA 34 D6", "3C 48 F7 30", "27 A4 3B D6",
  "EC 27 AE 2F", "C7 85 44 D6", "D7 62 CD D6", "67 DF C9 D6", "F7 14 CA D6",
  "37 E2 35 D6",
};






int rele = 2;
int beeper = 3;
int ledSpravne = 4;
int ledZle = 5;

int resetPin = 8;

int codesBTN = A0;


int refreshArduinoTime = 30;



void setup()
{
  digitalWrite(resetPin, HIGH);
  pinMode(resetPin, OUTPUT);
  pinMode(rele, OUTPUT);
  pinMode(ledSpravne, OUTPUT);
  pinMode(ledZle, OUTPUT);
  pinMode(beeper, OUTPUT);

  pinMode(codesBTN, INPUT);

  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();

  signalizaciaZapnutia();
}

void loop()
{
  if (millis() > refreshArduinoTime * 60000) restartArduino();
  if (digitalRead(codesBTN) == HIGH) printUIDS();

  // Look for new cards and select one of the cards
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

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
    otvoritStroj(1000);
    return;
  }
  else
  {
    Serial.println(" Access denied"); //*|*//
    Serial.println(); //*|*//

    digitalWrite(rele, LOW);
    signalizacia(false);
  }
  delay(100);
}

void beep(int len, int d1, int d2) {
  for (int i = 0; i < len; i++)
  {
    digitalWrite(beeper, HIGH);
    delay(d1);

    digitalWrite(beeper, LOW);
    delay(d2);
  }
}

bool isInAuthorized(String value)
{
  for (int uidIndex = 0; uidIndex < sizeof(uids) / sizeof(uids[0]); uidIndex++)
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
        digitalWrite(beeper, HIGH);
        delay(i);

        digitalWrite(beeper, LOW);
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
    digitalWrite(beeper, HIGH);
    delay(1);

    digitalWrite(beeper, LOW);
    delay(1);
  }
  delay(50);
  for (int i = 0; i < 100; i++)
  {
    digitalWrite(beeper, HIGH);
    delay(3);

    digitalWrite(beeper, LOW);
    delay(1);
  }
  digitalWrite(ledZle, LOW);

}

void signalizaciaZapnutia()
{
  delay(500);
  for (int i = 0; i < 50; i++)
  {
    digitalWrite(beeper, HIGH);
    delay(3);

    digitalWrite(beeper, LOW);
    delay(3);
  }
  delay(50);
  for (int i = 0; i < 50; i++)
  {
    digitalWrite(beeper, HIGH);
    delay(1);

    digitalWrite(beeper, LOW);
    delay(1);
  }
  delay(50);
}

void signalizaciaRestartu()
{
  for (int i = 0; i < 50; i++)
  {
    digitalWrite(beeper, HIGH);
    delay(4);

    digitalWrite(beeper, LOW);
    delay(4);
  }
  delay(50);
  for (int i = 0; i < 50; i++)
  {
    digitalWrite(beeper, HIGH);
    delay(2);

    digitalWrite(beeper, LOW);
    delay(2);
  }
  delay(50);
  for (int i = 0; i < 50; i++)
  {
    digitalWrite(beeper, HIGH);
    delay(3);

    digitalWrite(beeper, LOW);
    delay(3);
  }
  delay(50);
  for (int i = 0; i < 50; i++)
  {
    digitalWrite(beeper, HIGH);
    delay(1);

    digitalWrite(beeper, LOW);
    delay(1);
  }
  delay(50);
}

void otvoritStroj(int timeOpened)
{
  digitalWrite(rele, HIGH);
  delay(timeOpened);
  digitalWrite(rele, LOW);
}

void printUIDS()
{
  Serial.println("Stare uid kody: ");

  for (int uidIndex = 0; uidIndex < sizeof(uids) / sizeof(uids[0]); uidIndex++)
  {
    Serial.print("\"");
    Serial.print(uids[uidIndex]);
    Serial.print("\", ");
  }
  Serial.println("\n");
  delay(2000);
}

void restartArduino() {
  signalizaciaRestartu();
  digitalWrite(resetPin, LOW);
}
