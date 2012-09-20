#include <LiquidCrystal.h>               // include lcd library code
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);   // initialize the library with the numbers of the interface pins

int  val = 0;
char message[61];
int messageBytesRead = 0;
int currentLetter = 0;
String trimmedMessage = "";

void setup() { 
  Serial.begin(9600);                    // RFID reader SOUT pin connected to Serial RX pin at 2400bps

  lcd.begin(16, 2);                      // set up the LCD's number of columns and rows
  lcd.setCursor(0, 0);
  lcd.print("   S Y S T E M  ");
  lcd.setCursor(0, 1);
  lcd.print("   R E A D Y !  ");

  Serial.println("Ready for communication.");
}  

void loop() {
  //Check Hardware Serial for comms 
  if(Serial.available() > 0) {
    messageBytesRead = 0;
    val = 0; 
    char tempMessage[61];
    while(val != 10) {                   // read 140 digit code 
      if( Serial.available() > 0) { 
        val = Serial.read(); 
        if((val == 10)||(val == 13)) {   // if header or stop bytes before the 10 digit reading 
          break;                         // stop reading 
        } 
        message[messageBytesRead] = val; // add the digit           
        messageBytesRead++;              // ready to read next digit  
      } 
    }
    if(messageBytesRead > 1) {           // if 10 digit read is complete
      trimmedMessage = "";
      for (int thisChar = 0; thisChar < messageBytesRead; thisChar++) {
        trimmedMessage = trimmedMessage + message[thisChar];
      }
      lcd.clear();
      currentLetter = 0;
      lcd.setCursor(1, 0);
      lcd.print(trimmedMessage);
    }
  }
} 

