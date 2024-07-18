//
// connect to eduroam wifi (no certificate)
// connect to sign (or other device) on 9600 baud serial (UART 2)
//
//

// on-board ESP32 LED
#define LED_PIN 2

#include <WiFi.h> //Wifi library
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks
#include "arduino_percent.hpp"  // URL encode and decode

//Identity for user with password related to his realm (organization)
//Available option of anonymous identity for federation of RADIUS servers or 1st Domain RADIUS servers
#define EAP_ANONYMOUS_IDENTITY "" //anonymous@example.com, or you can use also nickname@example.com
#define EAP_IDENTITY "hazen@bu.edu" //nickname@example.com, at some organizations should work nickname only without realm, but it is not recommended
#define EAP_PASSWORD "U1traLin3arKT88" //password for eduroam account
#define EAP_USERNAME "hazen@bu.edu" // the Username is the same as the Identity in most eduroam networks.

//SSID NAME
const char* ssid = "eduroam"; // eduroam SSID

char resp[100];

WiFiServer server(80);

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}

void setup() {
  pinMode( LED_PIN, OUTPUT);

  Serial.begin(115200);
  Serial2.begin( 9600);
  delay(1000);
  Serial2.print( "M CONNECTING\n");
  delay(5000);
  Serial.print(F("Connecting to network: "));
  Serial.println(ssid);
  WiFi.disconnect(true);  //disconnect from WiFi to set new WiFi connection
 
  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD); // without CERTIFICATE, RADIUS server EXCEPTION "for old devices" required
  
  Serial.print("MAC: ");
  Serial.println( WiFi.macAddress());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println("");
  Serial.println(F("WiFi is connected!"));
  Serial.println(F("IP address set: "));
  Serial.println(WiFi.localIP()); //print LAN IP

  // convert IP address and display
  String ips = IpAddress2String( WiFi.localIP());
  snprintf( resp, sizeof(resp), "M %s\n", ips.c_str() );
  Serial2.print( resp);

  server.begin();
  delay(500);
}

//
// taken largely from SimpleWiFiServer example
//
void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // process line and erase it
	    // look for queries headed to us
	    const char *qry = currentLine.c_str();
	    if( !strncmp( qry, "GET /CMD=", 9)) {
	      // unencode the command and send to arduino
	      percent::decode( qry+9, resp);
	      // check for and strip HTTP thing
	      char *p = strrchr( resp, ' ');   // look for space at end
	      if( p)			       // overwrite with null
		*p = '\0';
	      strcat( resp, "\n");
	      Serial2.print( resp);
	    }
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }



        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_PIN, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_PIN, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
  
}
