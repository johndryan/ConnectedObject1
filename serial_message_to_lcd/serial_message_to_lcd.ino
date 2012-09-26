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

int  val = 0;
char message[61];
int messageBytesRead = 0;
int currentLetter = 0;
String trimmedMessage = "";
boolean messagePresent = false;

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
      if (messagePresent) {
        messagePresent = false;
      } else {
        lcd.clear();
        lcd.setCursor(0,0);
        messagePresent = true; 
      }
    } else {
      if (messagePresent) lcd.print(c);
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
}


// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect(serverName, 80)) {
    client.println("GET /arduino.txt HTTP/1.0");
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
