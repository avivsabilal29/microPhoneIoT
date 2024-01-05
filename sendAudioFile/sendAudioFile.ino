#include "LittleFS.h"
#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "KONTRAKAN ALUMNI";
const char* password = "TMUA1443H";
String serverName = "storage.googleapis.com/web-xerpihan.appspot.com";   // GANTI DENGAN ALAMAT IP Raspberry Pi ANDA
//String serverPath = "/audio.php";     // Path server seharusnya sesuai dengan tempat file diunggah
String serverPath = "/noot/esp32AudioFIle.wav?X-Goog-Algorithm=GOOG4-RSA-SHA256&X-Goog-Credential=web-xerpihan%40appspot.gserviceaccount.com%2F20231230%2Fauto%2Fstorage%2Fgoog4_request&X-Goog-Date=20231230T175048Z&X-Goog-Expires=900&X-Goog-SignedHeaders=host&X-Goog-Signature=86d3e97e03dc1d72922bbb82dbf7d623ecbaacec71e8be7a2b933fe5266ed494ce61068dfbffeeb301f8d2804533df31d11a6f5dc54d6e01f27d66a0a232ea4f677d5ca93605c09795ed3a38c68b652f923ee36cc19ee046fb43eb36962ba1ee69eee9bb1e82a0319eb675c6be2996a6d87cbfece33b51c4524ce675020fb7f0a6018df2e9f06a39675d6141e4b85cb5a7387b80f1387fc969b738757db8f40e679ebe802a4bb1452321d576955fbefed4c0bc8d09b63c87c421be5ee06f0e67848d2ea94e07428524fb8fe19da491a3813135328a204882f5c30270d4932693abd6377198155b74b96531886ae3a9a299555d8404ce53c9e8013d392f073670";     // Path server langsung ke folder penyimpanan
const int serverPort = 80;

WiFiClient client;
File myfile;
void initLittleFS() {
  if (!LittleFS.begin(true)) {
    Serial.println("An Error has occurred while mounting LittleFS");
    ESP.restart();
  }
  else {
    delay(500);
    Serial.println("LittleFS mounted successfully");
  }
}
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
  initLittleFS();
}

void loop() {
  myfile = LittleFS.open("/esp32AudioFIle.wav");
  // Close the file
  myfile.close();
  Serial.println("Connecting to server: " + serverName);
  size_t audioFileSize = myfile.size();

  if (client.connect(serverName.c_str(), serverPort)) {
    Serial.println("Connection successful!");
    // Send HTTP POST request
    client.println("PUT " + serverPath + " HTTP/1.1");
    client.println("Host: " + serverName);
    client.println("Content-Disposition: binary; name=\"audioFile\"; filename=\"esp32AudioFIle.wav\"\r\nContent-Type: audio/wav");
    //    client.println("Content-Type: audio/wav");
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
    // Disconnect from server
    client.stop();
  }

  delay(10000); // Wait for 10 seconds before next attempt
}
