// Simple Web client example
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

char ssid[] = "********";            // your network SSID (name)
char pass[] = "********";            // your network password (use for WPA, or use as key for WEP)
char server1[] = "www.google.co.uk"; // name address for Google (using DNS)
char server2[] = "thingspeak.com";   // name address for Google (using DNS)

int port = 443;

// Security Certificate Authentic Fingerprints obtained from: from https://www.grc.com/fingerprints.htm
const char* fingerprint1 = "23 E2 56 C4 C6 68 06 0E AC 83 E3 2E EB 8D 0F 45 C8 BA DB 8C";  //https://www.google.co.uk 20x8=160-bit key
const char* fingerprint2 = "78 60 18 44 81 35 BF DF 77 84 D4 0A 22 0D 9B 4E 6C DC 57 2C";  //https://thingspeak.com/  20x8=160-bit key

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  Serial.println("\nConnected to wifi");
  Serial.print("connecting to ");
  Serial.println(server1);
  if (!client.connect(server1, port)) {
    Serial.println("connection failed");
    return;
  }
  if (client.verify(fingerprint1, server1)) {    // Now verify that this is the www.google.co.uk server by checking its certificate and fingerprint
    Serial.println("certificate matches...");
    Serial.println("Requesting data from server...");
    client.println("GET /search?q=123456=english& HTTP/1.1");    // Make a HTTP request:
    client.println("Host: www.google.co.uk");
    client.println("Connection: close");
    client.println();
  } else
  {
    Serial.println("certificate does not match...");
    client.stop();
    Serial.println("Demonstration complete");
    while(1);
  }
}

void loop() {
  while (client.connected()) {// if there are incoming bytes available read and print them:
    char c = client.read();
    Serial.write(c);
    if (c=='\n' || c=='\r') Serial.println();
  }
  client.stop();
  Serial.println("Demonstration complete");
  while(1);
}

/*
 * The client generates a random key that will be used for the primary symmetrical encryption algorithm. 
 * It encrypts it using an algorithm agreed upon during the Hello phase together with the server’s public key (found on its SSL certificate).
 * It sends this encrypted key to the server where it is decrypted using the server’s private key and the interesting parts of the handshake are complete.
 * The parties can now be content that they are talking to the right person and have secretly agreed on a key to symmetrically encrypt the data they
 * are about to send each other.
 * HTTP requests and responses can now be sent by forming a plaintext message and encrypting and sending it.
 * The other party is the only one who knows how to decrypt this message and so anyone monitoring the data traffic is unable to read or modify any requests 
 * to make an intercept.
 */

