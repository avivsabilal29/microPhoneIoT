#include "Arduino.h"
#include <WiFiClientSecure.h>
#include <LittleFS.h>
#include <FS.h>
#include "Wav.h"
#include "I2S.h"

//Replace with your network credentials
const char* ssid = "KONTRAKAN ALUMNI";
const char* password = "TMUA1443H";

String serverName = "storage.googleapis.com";
String serverPath = "/web-xerpihan.appspot.com/noot/esp32AudioFIle4.wav?X-Goog-Algorithm=GOOG4-RSA-SHA256&X-Goog-Credential=web-xerpihan%40appspot.gserviceaccount.com%2F20240101%2Fauto%2Fstorage%2Fgoog4_request&X-Goog-Date=20240101T104713Z&X-Goog-Expires=900&X-Goog-SignedHeaders=host&X-Goog-Signature=87291c693bd89374aaabaf3b6149653cf6fb4a7f9e940c8bf3b8158b9174dab068c7b684ffc084048144a43e85661d8a12eaa03cd902e02a9968c588b26016663f9235ca48fcdb46ffdd2dba6c0a2e382659e917449b6d4acfa32ee70c37962428a58681d9c870c722cb48d61cc2f87c572a6e70bdfe2037695a840fe5d0bb4aa168945ce0cb7d867d9fe1316b8d2974ab2348e7b88a929628de34f3705ee224d00b0035f016e7f1aa7e853371de0b5c33a0638f02b9d91a36d557b281d56c14140211fcd6f1e027686d67532c5ea2e4d08647b2442ac0c8948e37777ef42cf2457412d8a3969d3be399a159f3ed265ce3d6c5f639f0a448e2f325d242ddd7a3";     // Path server langsung ke folder penyimpanan
const int serverPort = 443;

const char* test_root_ca = \
                           "-----BEGIN CERTIFICATE-----\n" \
                           "MIIFljCCA36gAwIBAgINAgO8U1lrNMcY9QFQZjANBgkqhkiG9w0BAQsFADBHMQsw\n" \
                           "CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n" \
                           "MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMjAwODEzMDAwMDQyWhcNMjcwOTMwMDAw\n" \
                           "MDQyWjBGMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp\n" \
                           "Y2VzIExMQzETMBEGA1UEAxMKR1RTIENBIDFDMzCCASIwDQYJKoZIhvcNAQEBBQAD\n" \
                           "ggEPADCCAQoCggEBAPWI3+dijB43+DdCkH9sh9D7ZYIl/ejLa6T/belaI+KZ9hzp\n" \
                           "kgOZE3wJCor6QtZeViSqejOEH9Hpabu5dOxXTGZok3c3VVP+ORBNtzS7XyV3NzsX\n" \
                           "lOo85Z3VvMO0Q+sup0fvsEQRY9i0QYXdQTBIkxu/t/bgRQIh4JZCF8/ZK2VWNAcm\n" \
                           "BA2o/X3KLu/qSHw3TT8An4Pf73WELnlXXPxXbhqW//yMmqaZviXZf5YsBvcRKgKA\n" \
                           "gOtjGDxQSYflispfGStZloEAoPtR28p3CwvJlk/vcEnHXG0g/Zm0tOLKLnf9LdwL\n" \
                           "tmsTDIwZKxeWmLnwi/agJ7u2441Rj72ux5uxiZ0CAwEAAaOCAYAwggF8MA4GA1Ud\n" \
                           "DwEB/wQEAwIBhjAdBgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwEgYDVR0T\n" \
                           "AQH/BAgwBgEB/wIBADAdBgNVHQ4EFgQUinR/r4XN7pXNPZzQ4kYU83E1HScwHwYD\n" \
                           "VR0jBBgwFoAU5K8rJnEaK0gnhS9SZizv8IkTcT4waAYIKwYBBQUHAQEEXDBaMCYG\n" \
                           "CCsGAQUFBzABhhpodHRwOi8vb2NzcC5wa2kuZ29vZy9ndHNyMTAwBggrBgEFBQcw\n" \
                           "AoYkaHR0cDovL3BraS5nb29nL3JlcG8vY2VydHMvZ3RzcjEuZGVyMDQGA1UdHwQt\n" \
                           "MCswKaAnoCWGI2h0dHA6Ly9jcmwucGtpLmdvb2cvZ3RzcjEvZ3RzcjEuY3JsMFcG\n" \
                           "A1UdIARQME4wOAYKKwYBBAHWeQIFAzAqMCgGCCsGAQUFBwIBFhxodHRwczovL3Br\n" \
                           "aS5nb29nL3JlcG9zaXRvcnkvMAgGBmeBDAECATAIBgZngQwBAgIwDQYJKoZIhvcN\n" \
                           "AQELBQADggIBAIl9rCBcDDy+mqhXlRu0rvqrpXJxtDaV/d9AEQNMwkYUuxQkq/BQ\n" \
                           "cSLbrcRuf8/xam/IgxvYzolfh2yHuKkMo5uhYpSTld9brmYZCwKWnvy15xBpPnrL\n" \
                           "RklfRuFBsdeYTWU0AIAaP0+fbH9JAIFTQaSSIYKCGvGjRFsqUBITTcFTNvNCCK9U\n" \
                           "+o53UxtkOCcXCb1YyRt8OS1b887U7ZfbFAO/CVMkH8IMBHmYJvJh8VNS/UKMG2Yr\n" \
                           "PxWhu//2m+OBmgEGcYk1KCTd4b3rGS3hSMs9WYNRtHTGnXzGsYZbr8w0xNPM1IER\n" \
                           "lQCh9BIiAfq0g3GvjLeMcySsN1PCAJA/Ef5c7TaUEDu9Ka7ixzpiO2xj2YC/WXGs\n" \
                           "Yye5TBeg2vZzFb8q3o/zpWwygTMD0IZRcZk0upONXbVRWPeyk+gB9lm+cZv9TSjO\n" \
                           "z23HFtz30dZGm6fKa+l3D/2gthsjgx0QGtkJAITgRNOidSOzNIb2ILCkXhAd4FJG\n" \
                           "AJ2xDx8hcFH1mt0G/FX0Kw4zd8NLQsLxdxP8c4CU6x+7Nz/OAipmsHMdMqUybDKw\n" \
                           "juDEI/9bfU1lcKwrmz3O2+BtjjKAvpafkmO8l7tdufThcV4q5O8DIrGKZTqPwJNl\n" \
                           "1IXNDw9bg1kWRxYtnCQ6yICmJhSFm/Y3m6xv+cXDBlHz4n/FsRC6UfTd\n" \
                           "-----END CERTIFICATE-----\n";


WiFiClientSecure client;

//comment the first line and uncomment the second if you use MAX9814
//#define I2S_MODE I2S_MODE_RX
#define I2S_MODE I2S_MODE_ADC_BUILT_IN

const int record_time = 10;  // second

const int headerSize = 44;
const int waveDataSize = record_time * 88000;
const int numCommunicationData = 8000;
const int numPartWavData = numCommunicationData / 4;
byte header[headerSize];
char communicationData[numCommunicationData];
char partWavData[numPartWavData];
File file;

void initWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}


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
  initWiFi();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  client.setCACert(test_root_ca);
  initLittleFS();
}

void loop() {
  // Close the file
  Serial.println("Connecting to server: " + serverName);
  size_t audioFileSize = file.size();

  if (client.connect(serverName.c_str(), serverPort)) {
    Serial.println("Connection successful!");
    // Send HTTP POST request
    client.println("PUT " + serverPath + " HTTP/1.1");
    client.println("Host: " + serverName);
    //    client.println("Content-Disposition: binary; name=\"audioFile\"; filename=\"esp32AudioFIle.wav\"\r\nContent-Type: audio/wav");
    client.println("File_name: esp32AudioFIle4.wav");
    client.println("Content-Type: application/octet-stream");
//    client.print("Content-Length: ");
//    client.println(audioFileSize );
//    client.println();
    //    client.println("Content-Type: audio/wav");
    //    client.print("Content-Length: ");
    //    client.println(myfile.size());
    //    client.println();
    //    client.println(myfile);

    //
    //    while (myfile.available()) {
    //      uint8_t buffer[128];
    //      size_t len = myfile.readBytes((char*)buffer, sizeof(buffer));
    //      client.write(buffer, len);
    //      client.write(myfile.read());
    //    }

    // Send file data
    //    const size_t bufferSize = 1024;
    //    uint8_t buffer[bufferSize];
    //    size_t bytesSent = 0;
    //
    //    while (bytesSent < myfile.size()) {
    //      size_t bytesToRead = min(bufferSize, myfile.size() - bytesSent);
    //      myfile.readBytes((char*)buffer, bytesToRead);
    //      client.write(buffer, bytesToRead);
    //      bytesSent += bytesToRead;
    //    }


    // Send file data in chunks
    //    const size_t bufferSize = 1024; // Ukuran buffer untuk setiap chunk
    //    uint8_t buffer[bufferSize];
    //    size_t bytesSent = 0;
    //
    //    while (bytesSent < myfile.size()) {
    //      size_t bytesToRead = min(bufferSize, myfile.size() - bytesSent);
    //      size_t bytesRead = myfile.read(buffer, bytesToRead);
    //
    //      if (bytesRead > 0) {
    //        client.write(buffer, bytesRead);
    //        bytesSent += bytesRead;
    //      } else {
    //        break; // Keluar dari loop jika tidak ada byte yang terbaca
    //      }
    //    }


    CreateWavHeader(header, waveDataSize);
    LittleFS.remove("/esp32AudioFIle4.wav");
    file = LittleFS.open("/esp32AudioFIle4.wav", FILE_WRITE);
    if (!file) return;
    file.write(header, headerSize);
    client.write(header, headerSize);
    I2S_Init(I2S_MODE, I2S_BITS_PER_SAMPLE_32BIT);
    for (int j = 0; j < waveDataSize / numPartWavData; ++j) {
      I2S_Read(communicationData, numCommunicationData);
      for (int i = 0; i < numCommunicationData / 8; ++i) {
        partWavData[2 * i] = communicationData[8 * i + 2];
        partWavData[2 * i + 1] = communicationData[8 * i + 3];
      }
      file.write((const byte*)partWavData, numPartWavData);
      client.write((const byte*)partWavData, numPartWavData);
    }
    file.close();
    Serial.println("finish");

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
