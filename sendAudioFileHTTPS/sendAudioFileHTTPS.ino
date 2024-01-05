#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <stdio.h>
#include <SPIFFS.h>
#include "FS.h"
const char* ssid = "KONTRAKAN ALUMNI";
const char* password = "TMUA1443H";
String serverName = "storage.googleapis.com";
String serverPath = "/web-xerpihan.appspot.com/noot/sendFileBismillah.wav?X-Goog-Algorithm=GOOG4-RSA-SHA256&X-Goog-Credential=web-xerpihan%40appspot.gserviceaccount.com%2F20240103%2Fauto%2Fstorage%2Fgoog4_request&X-Goog-Date=20240103T141238Z&X-Goog-Expires=900&X-Goog-SignedHeaders=host&X-Goog-Signature=710710e65890706c5fd9989d62a6123b42a7054b536d4d3f570ca3fb1d6d49eb7462fa3723a7b9927af3b0d21fc58b2c4e8185933e9243330700b224a613ba6e03846ea2fdd6e95d2f8490534265316a03b5b07e5039737a797d2a5fd110a098daebb0312813935aca50f0a772d58de25cf869a6657818ab06e7e58776a290c88289ce7988ebf9c3b69294bf021b2048073ea02697a33fbc6a95fee817ebcb36b58c1235ffb485d7e3d0ebbdf1830cec3ae062241596d611cc93bca8a6773b3f7b7e059f9137f906afc325a3b2fb1a5da6e01d55b519ff1b16fdf347c90e41bd5c87a6393482322cff8bfa261c637d0237b612fd65d41505508f9d05c310b4fb";     // Path server langsung ke folder penyimpanan
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
File myfile;
const char filename[] = "/sendFileBismillah.wav";
void SPIFFSInit() {
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield();
  }
}
void setup() {
  Serial.begin(115200);
  SPIFFSInit();
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
  client.setCACert(test_root_ca);
}

void loop() {
  myfile = SPIFFS.open(filename);
  if (!myfile) {
    Serial.println("File is not available!");
  }
  Serial.println("Connecting to server: " + serverName);
  size_t audioFileSize = myfile.size();

  if (client.connect(serverName.c_str(), serverPort)) {
    const size_t bufferSize = 512;
    uint8_t buffer[bufferSize];
    size_t bytesRead = myfile.readBytes((char*)buffer, bufferSize);

    // Send file data in chunks
    const size_t max_upload_size = 10; // Ukuran chunk yang diunggah
    size_t bytesSent = 0;
    size_t remainingBytes = bytesRead;
    if (bytesSent < bytesRead) {
      size_t bytesToUpload = min(max_upload_size, remainingBytes);
      //    client.println("Content-Disposition: binary; name=\"audioFile\"; filename=\"sendFileBismillah.wav\"\r\nContent-Type: application/octet-stream");
      Serial.println("Connection successful!");
      // Send HTTP POST request
      client.println("PUT " + serverPath + " HTTP/1.1");
      client.println("Host: " + serverName);
      client.println("Connection: keep-alive");
      client.println("File_name: sendFileBismillah.wav");
      client.println("Content-Type: application/octet-stream");
      client.write((const uint8_t*)buffer + bytesSent, bytesToUpload);
      bytesSent += bytesToUpload;
      remainingBytes -= bytesToUpload;
    }
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
  myfile.close();
  client.stop();
  delay(10000);
}
//    client.println("Content-Type: audio/wav");
//    client.print("Content-Length: ");
//    client.println(audioFileSize );
//    client.println();

//  FILE *fp = fopen("/sendFileBismillah.wav", "rb"); // read in bytes
//    const size_t bufferSize = 512;
//    uint8_t buffer[bufferSize];
//    size_t bytesRead = myfile.readBytes((char*)buffer, bufferSize);

// Send file data in chunks
//    const size_t max_upload_size = 10; // Ukuran chunk yang diunggah
//    size_t bytesSent = 0;
//    size_t remainingBytes = bytesRead;
//
//    while (bytesSent < bytesRead) {
//      size_t bytesToUpload = min(max_upload_size, remainingBytes);
//      client.println("File_name: sendFileBismillah.wav");
//      client.println("Content-Type: application/octet-stream");
//      client.write((const uint8_t*)buffer + bytesSent, bytesToUpload);
//      bytesSent += bytesToUpload;
//      remainingBytes -= bytesToUpload;
//
//
//    if (num_of_uploads > 0)
//    {
//      uint8_t buff1[max_upload_size] = {};
//      for (i = 0; i < num_of_uploads; i++)
//      {
//        fread(buff1, sizeof * buff1, sizeof buff1 / sizeof * buff1, fp);
//        client.write(buffer, len);
//        client.write(myfile.read());
//        client.println("File_name: sendFileBismillah.wav");
//        client.println("Content-Type: application/octet-stream");
//        //        client.write((char*)buff1, sizeof(buff1));
//        client.println((char*)buff1);
//      }
//    }
//
//    if (num_of_uploads_mod > 0)
//    {
//      int remainder = file_size - num_of_uploads * max_upload_size;
//      uint8_t buff2[remainder] = {};
//      fread(buff2, sizeof * buff2, sizeof buff2 / sizeof * buff2, fp);
//      client.println("File_name: sendFileBismillah.wav");
//      client.println("Content-Type: application/octet-stream");
//      //      client.write((char*)buff2, sizeof(buff2));
//      client.println((char*)buff2);
//    }
//



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
