//
// connect to eduroam wifi (no certificate)
// connect to dedicated TCP server (ohm, for now) on local port
// send commands to server periodically to configure sign
// via 9600 baud serial (UART 2)
//
// commands to server:
//
// TIM                       get date/time as "Tue Jul 16 11:10:30 2024"
// DAT                       read data file, return "RECS n" where n is record count
// REC n                     retrieve record n which should be one of the commands below
//
// commands to sign:
//
// M <marquee text>          update the top-line scrolled message (80 char max)
// O n hh:mm                 update opening time for day n (0-7)
// C n hh:mm                 update closing time for day n (0-7)
// I n                       set intensity to n (0-10)
//


#include <WiFi.h> //Wifi library
#include "esp_wpa2.h" //wpa2 library for connections to Enterprise networks

//Identity for user with password related to his realm (organization)
//Available option of anonymous identity for federation of RADIUS servers or 1st Domain RADIUS servers
#define EAP_ANONYMOUS_IDENTITY "" //anonymous@example.com, or you can use also nickname@example.com
#define EAP_IDENTITY "hazen@bu.edu" //nickname@example.com, at some organizations should work nickname only without realm, but it is not recommended

ERROR - replace with valid WiFi credentials

#define EAP_PASSWORD "notThePassword" //password for eduroam account
#define EAP_USERNAME "notTheUsername@bu.edu" // the Username is the same as the Identity in most eduroam networks.

//SSID NAME
const char* ssid = "eduroam"; // eduroam SSID

char resp[100];

void setup() {
  Serial.begin(115200);
  
  Serial2.begin( 9600);
  delay(1000);
  Serial2.print( "M CONNECTING\n\r");
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
  Serial2.print("M CONNECTED\n\r");
  delay(500);
}



void loop(){

  static int nrecs;		// number of data records available
  static int crec;		// current record number

  // coordinates of server
  const uint16_t port = 5432;
  const char * host = "128.197.43.46"; // ip or dns

  Serial.print("Connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("Connection failed.");
    Serial.println("Waiting 5 seconds before retrying...");
    delay(5000);
    return;
  }

  // send a request
  if( !nrecs || crec >= nrecs-1) { // no data available, request it
    client.print("DAT");
  } else {
    if( crec < nrecs-1) {
      sprintf( resp, "REC %d", crec);
      client.print( resp);
      ++crec;
    }
  }

  int maxloops = 0;

  //wait for the server's reply to become available
  while (!client.available() && maxloops < 1000) {
    maxloops++;
    delay(1); //delay 1 msec
  }

  if (client.available() > 0) {

    //read back one line from the server
    String line = client.readStringUntil('\r');
    Serial.println(line);

    const char* s = line.c_str();	// we're old-fashioned!

    // check for record count
    if( !strncmp( s, "RECS ", 5)) {
      crec = 0;
      nrecs = atoi( s+5);
    } else if( !strncmp( s, "ERR", 3)) {
      // error return ignore it
    } else {
      // must be a command
      if( *s == 'M' || *s == 'O' || *s == 'C' || *s == 'I') {
	sprintf( resp, "%s\n\r", s); // ensure there is \n
	Serial2.print( resp);
      }
    }

  } else {
    delay(1000);
    Serial.println("client.available() timed out ");
  }


  Serial.println("Closing connection.");
  client.stop();
  Serial.println("Waiting 1 seconds before restarting...");
  delay(1000);
}
