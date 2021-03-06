/*
  WiFiEsp example: WebClient

  This sketch connects to google website using an ESP8266 module to
  perform a simple web search.

  For more details see: http://yaab-arduino.blogspot.com/p/wifiesp-example-client.html
*/

#include "WiFiEsp.h"
#include <SimpleDHT.h>

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(12, 13); // RX, TX
#endif

int pinDHT11 = 2;
SimpleDHT11 dht11;
char ssid[] = "dat_feel";            // your network SSID (name)
char pass[] = "constanzadotjpg123";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

//char server[] = "arduino.cc";
char server[] = "associate-engineer.de";

// Initialize the Ethernet client object
WiFiEspClient client;

void setup()
{
  
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");

  printWifiStatus();

  Serial.println();
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  if (client.connect(server, 80)) {
    Serial.println("Connected to server");
  }
}


void loop()
{
  //Temperaturmessung
  
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  dht11.read(pinDHT11, &temperature, &humidity, data);
  
  
  // Make a HTTP request
  // client.println("GET /asciilogo.txt HTTP/1.1");
  // client.println("Host: arduino.cc");
  if (!client.connected()) {
    client.connect(server, 80);
  }
  if (client.connected()) {
    
    client.print("GET /datacollector/storedata.php?user=philip&type=Temperatur&value=");
    client.print((int)temperature);
    client.println(" HTTP/1.1");
    client.println("Host: associate-engineer.de");

    //tempWert = (int)

    client.println("Connection: close");
    client.println();
  }
  // if there are incoming bytes available
  // from the server, read them and print them
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  delay(1000);
  // if the server's disconnected, stop the client
  /* if (!client.connected()) {
     Serial.println();
     Serial.println("Disconnecting from server...");
     client.stop();

     // do nothing forevermore
     while (true);
    } */

}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
