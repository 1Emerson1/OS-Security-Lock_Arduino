// LCD library
#include <LiquidCrystal.h>
// RFID library
#include <SPI.h>
#include <MFRC522.h>

// define SDA
#define SS_PIN 48
// define RST
#define RST_PIN 49
MFRC522 mfrc522(SS_PIN, RST_PIN);

// initialize LCD interface pin with the arduino pin number
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
// initalize LED pin
const int red = 11;
const int green = 12;

// create lcd object
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

  void setup() {
    // LCD's number of columns and rows:
    lcd.begin(16, 2);
    lcd.print("Present keycard to unlock...");
  
    // begin serial communication
    Serial.begin(9600);
    SPI.begin();
  
    // initalize MFRC522
    mfrc522.PCD_Init();
  
    // serial monitor
    Serial.println("Scan device...");
  
    // initialize LEDs
    pinMode(green, OUTPUT);
    pinMode(red, OUTPUT);
  }
  
  void loop() {
  
    if(!mfrc522.PICC_IsNewCardPresent()) {
      return;
    }
  
    if(!mfrc522.PICC_ReadCardSerial()) {
      return;
    }
  
    //Show UID on serial monitor
    Serial.print("UID tag :");
    String content= "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++)  {
       Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
       Serial.print(mfrc522.uid.uidByte[i], HEX);
       content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
       content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    
    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();
    if (content.substring(1) == "12 1A D4 13") { //change here the UID of the card/cards that you want to give access
      lcd.clear();
      lcd.print("Access Granted!");
      Serial.println("Authorized access");
      Serial.println();
      digitalWrite(green, HIGH);
      delay(500);
      digitalWrite(green, LOW);
      delay(1000);
      lcd.clear();
      lcd.print("Present keycard to unlock...");
    } else {
      lcd.clear();
      lcd.print("Access Denied!");
      Serial.println(" Access denied");
      digitalWrite(red, HIGH);
      delay(500);
      digitalWrite(red, LOW);
      delay(1000);
      lcd.clear();
      lcd.print("Present keycard to unlock...");
    }
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
  }
