#include "LittleFS.h"
#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "KONTRAKAN ALUMNI";
const char* password = "TMUA1443H";
String serverName = "infakpro.com";   // GANTI DENGAN ALAMAT IP Raspberry Pi ANDA
//String serverPath = "/audio.php";     // Path server seharusnya sesuai dengan tempat file diunggah
String serverPath = "/audioFile";     // Path server langsung ke folder penyimpanan
const int serverPort = 80;

WiFiClient client;
File myfile;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  myfile = LittleFS.open("/testingSend.wav");
  if (!myfile) {
    Serial.println("Failed to open file for reading");
    return;
  }
}

void loop() {
  Serial.println("Connecting to server: " + serverName);
  size_t audioFileSize = myfile.size();

  if (client.connect(serverName.c_str(), serverPort)) {
    Serial.println("Connection successful!");
    // Send HTTP POST request
    client.println("POST " + serverPath + " HTTP/1.1");
    client.println("Host: " + serverName);
    client.println("Content-Disposition: form-data; name=\"audioFile\"; filename=\"testingSend.wav\"\r\nContent-Type: audio/wav");
    client.println("Content-Type: audio/wav");
    client.print("Content-Length: ");
    client.println(audioFileSize );
    client.println();
    //    client.println("Content-Type: audio/wav");
    //    client.print("Content-Length: ");
    //    client.println(myfile.size());
    //    client.println();
    //    client.println(myfile);


    while (myfile.available()) {
      uint8_t buffer[128];
      size_t len = myfile.readBytes((char*)buffer, sizeof(buffer));
      client.write(buffer, len);
    }

    // Wait for server response
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }

    // Print server response
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
    // Close the file
    myfile.close();
    // Disconnect from server
    client.stop();
  }

  delay(10000); // Wait for 10 seconds before next attempt
}
