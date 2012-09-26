#include <LiquidCrystal.h>               // include lcd library code
LiquidCrystal lcd(8, 9, 7, 6, 5, 4);     // initialize the library with the numbers of the interface pins

#include <SPI.h>
#include <Ethernet.h>
byte mac[] = {  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };  // Enter a MAC address for controller
char serverName[] = "ve.lifeforms.ie";
EthernetClient client;                   // Initialize the Ethernet client library

int  val = 0;
char message[61];
int messageBytesRead = 0;
int currentLetter = 0;
String trimmedMessage = "";
boolean messagePresent = false;

void setup() { 
  Serial.begin(9600);                    // RFID reader SOUT pin connected to Serial RX pin at 2400bps

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

  // if you get a connection, report back via serial: 
  if (client.connect(serverName, 80)) {
    lcd.clear();
    lcd.print("Connected");
    // Make a HTTP request:
    client.println("GET /arduino.txt HTTP/1.0");
    client.println();
  } 
  else {
    // if you didn't get a connection to the server:
    lcd.clear();
    lcd.println("connection failed");
  }
}  

void loop() {
  // if there are incoming bytes available 
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    if (c == '\r') {
      lcd.clear();
      lcd.setCursor(0,0);
    } else {
      if (c != '\n') lcd.print(c);
    }
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while(true);
  }
} 
