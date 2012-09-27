#include <LiquidCrystal.h>               // include lcd library code
LiquidCrystal lcd(8, 9, 7, 6, 5, 4);     // initialize the library with the numbers of the interface pins

#include <SPI.h>
#include <Ethernet.h>
byte mac[] = {  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };  // Enter a MAC address for controller
char serverName[] = "ve.lifeforms.ie";
EthernetClient client;                   // Initialize the Ethernet client library

unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 10*1000;  // delay between updates, in milliseconds

unsigned long lastLCDChange = 0;          // last time you connected to the server, in milliseconds
const unsigned long lcdInterval = 2*1000;  // delay between updates, in milliseconds

//boolean serverChecked = false;

int currentLetter = 0;
int currentLine = 0;
int currentLineDisplay = 0;
int numLines = 10;
String tempString = "";

String line_0 = "----------------";   // "String 0" etc are strings to store - change to suit.
String line_1 = "WAITING.        ";
String line_2 = "WAITING .       ";
String line_3 = "WAITING  .      ";
String line_4 = "WAITING   .     ";
String line_5 = "WAITING    .    ";
String line_6 = "WAITING     .   ";
String line_7 = "WAITING      .  ";
String line_8 = "WAITING       . ";
String line_9 = "WAITING        .";
String line_10 = "WAITING         ";

boolean readingMessage = false;

String lcd_lines[] = { line_0, line_1, line_2, line_3, line_4, line_5, line_6, line_7, line_8, line_9, line_10 };

void setup() { 
  Serial.begin(9600);

  lcd.begin(16, 2);                      // set up the LCD's number of columns and rows
  lcd.setCursor(0, 0);

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    lcd.clear();
    lcd.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  
  lcd.clear();
  lcd.print("Connecting...");
}  

void loop() {
  // if there are incoming bytes available 
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
    if (c == 126) {
      if (readingMessage) {
        //-- END MESSAGE
        readingMessage = false;
        numLines = currentLine;
      } else {
        //-- START MESSAGE
        currentLetter = 0;
        currentLine = 0;
        readingMessage = true;
        lcd_lines[currentLine] = "";
      }
    } else if (readingMessage) {
      //-- READING MESSAGE
      if (currentLetter > 15) {
        //Next Line
        currentLetter = 0;
        currentLine++;
        if (currentLine > 10) {
          // Message too long, ABORT ???
          readingMessage = false;
          numLines = 10;
          currentLine = 0;
          client.stop();
          return;
        }
        lcd_lines[currentLine] = "";
      }
        if (c == '\n' || c == '\r') c = ' ';
        lcd_lines[currentLine] = lcd_lines[currentLine] + c;
        currentLetter++;
      
      //if (readingMessage) lcd.print(c);
    }
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    client.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    httpRequest();
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
  
  if((millis() - lastLCDChange > lcdInterval) && !readingMessage) {
    lcdUpdate();
  }
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect(serverName, 80)) {
    client.println("GET /arduino/data.txt HTTP/1.0");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } 
  else {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" - CONNECTION - ");
    lcd.setCursor(1,0);
    lcd.print("   - ERROR -    ");
    client.stop();
  }
}

// Update LCD
void lcdUpdate() {
  Serial.println(numLines + "LINES ----------------");
  for (int i=0; i <= numLines; i++){
    Serial.println("::" + lcd_lines[i]);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  if (currentLineDisplay < 0) {
    lcd.print("----------------");
  } else {
    lcd.print(lcd_lines[currentLineDisplay]);
  }
  lcd.setCursor(0,1);
  if (currentLineDisplay == numLines) {
    lcd.print("----------------");
  } else {
    lcd.print(lcd_lines[currentLineDisplay+1]); 
  }
  currentLineDisplay++;
  if (currentLineDisplay > numLines) currentLineDisplay = -1;
  lastLCDChange = millis();
}
