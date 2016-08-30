#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIVIDER); // you can change this clock speed

#define WLAN_SSID       "<SSID>"           // cannot be longer than 32 characters!
#define WLAN_PASS       "<PASSWORD>"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define IDLE_TIMEOUT_MS  3000      // Amount of time to wait (in milliseconds) with no data 
                                   // received before closing the connection.  If you know the server
                                   // you're accessing is quick to respond, you can reduce this value.

// Port for incoming TCP connections
#define LISTEN_PORT 8888

// Server
Adafruit_CC3000_Server mServer(LISTEN_PORT);

// Timing
const int loopTime = 125;
int elapsedTime = 0;

// Client JSON String
String result;

// Relay IN2 connected to digital pin 0
int relayPin2 = 6;
int BUTTON = 7;

// Button State
int buttonState = 0;

// Binary Light Switch State
int lightSwitch = 0;

void setup(void)
{
 
  Serial.begin(115200);
  
  pinMode(relayPin2, OUTPUT);
  pinMode(BUTTON, INPUT);
  digitalWrite(relayPin2, HIGH);

  // Initialize the module
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }
  
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }

  // Wait for DHCP to complete
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }

  mServer.begin();
  
}

bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

void toggleLightSwitch() {

  if (lightSwitch == 1)
    lightSwitch = 0;
  else
    lightSwitch = 1;
  
  if (lightSwitch == 1) {
    Serial.println("SET LOW");
    digitalWrite(relayPin2, LOW);
  } else {
    Serial.println("SET HIGH");
    digitalWrite(relayPin2, HIGH);
  }
  
}

void loop(void)
{

  //displayConnectionDetails();
  
  Adafruit_CC3000_ClientRef client = mServer.available();

  Serial.println(lightSwitch);
  
  buttonState = digitalRead(BUTTON);
  
  if (buttonState == LOW)
    toggleLightSwitch();
  
  if (client) {
    
    while(client.available()) {   
      
      char c = client.read();
      result = result+c;
      
    }
    
    if (result.indexOf("switch") > 0)
      toggleLightSwitch();
    
    client.println("Success");
    delay(5);
    client.close();
  
    result = "";
  
  }
  
  delay(loopTime);

}

// SET STATIC IP ADDRESS

/*uint32_t ipAddress = cc3000.IP2U32(10, 0, 0, 13);
uint32_t netMask = cc3000.IP2U32(255, 255, 255, 0);
uint32_t defaultGateway = cc3000.IP2U32(192, 168, 1, 1);
uint32_t dns = cc3000.IP2U32(8, 8, 4, 4);
if (!cc3000.setStaticIPAddress(ipAddress, netMask, defaultGateway, dns)) {
  Serial.println(F("Failed to set static IP!"));
  while(1);
}*/
