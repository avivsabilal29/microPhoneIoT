

#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <HTTPClient.h>
const char* ssid = "KONTRAKAN ALUMNI";
const char* password = "TMUA1443H";
String serverName = "storage.googleapis.com";
String serverPath = "/web-xerpihan.appspot.com/noot/sendFileBismillah.wav?X-Goog-Algorithm=GOOG4-RSA-SHA256&X-Goog-Credential=web-xerpihan%40appspot.gserviceaccount.com%2F20240103%2Fauto%2Fstorage%2Fgoog4_request&X-Goog-Date=20240103T141238Z&X-Goog-Expires=900&X-Goog-SignedHeaders=host&X-Goog-Signature=710710e65890706c5fd9989d62a6123b42a7054b536d4d3f570ca3fb1d6d49eb7462fa3723a7b9927af3b0d21fc58b2c4e8185933e9243330700b224a613ba6e03846ea2fdd6e95d2f8490534265316a03b5b07e5039737a797d2a5fd110a098daebb0312813935aca50f0a772d58de25cf869a6657818ab06e7e58776a290c88289ce7988ebf9c3b69294bf021b2048073ea02697a33fbc6a95fee817ebcb36b58c1235ffb485d7e3d0ebbdf1830cec3ae062241596d611cc93bca8a6773b3f7b7e059f9137f906afc325a3b2fb1a5da6e01d55b519ff1b16fdf347c90e41bd5c87a6393482322cff8bfa261c637d0237b612fd65d41505508f9d05c310b4fb";     // Path server langsung ke folder penyimpanan
const int serverPort = 443;
const char filename[] = "/sendFileBismillah2.wav";
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

HTTPClient http;
File file;
void setup() {
  Serial.begin(115200);
  initLittleFS();
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
  sendFileChunks();
}

void loop() {
}


void sendFileChunks() {
  File file = LittleFS.open(filename, "r");
  if (!file) {
    Serial.println("Failed to open file");
    return;
  }
  http.begin("https://storage.googleapis.com/web-xerpihan.appspot.com/noot/sendFileBismillah2.wav?X-Goog-Algorithm=GOOG4-RSA-SHA256&X-Goog-Credential=web-xerpihan%40appspot.gserviceaccount.com%2F20240103%2Fauto%2Fstorage%2Fgoog4_request&X-Goog-Date=20240103T214648Z&X-Goog-Expires=900&X-Goog-SignedHeaders=host&X-Goog-Signature=4167127f60079e7a9414ec33cb78ea3130e1c2eda196b6b724bf9300e4ab864e8d775ac56bd3e27f00ea2384cc0f25b43e506c62293242e4d419664fe0f9be5021f75b6938ad0e8e5bc3b767008a50cc046bb51f7bbbe668724bb262756e6223f9fe3a01abb742ef08d04ae72f506cf053958ee8d5144cab1fc3c1fcfdcc6ea6097d2132a9a738c28fa52b40ab856c69149230b9505f35cdf6e062e27cdb134905c2cb10107ca0f5214b03cfd5f3ec1e3ad602f237ff2fb62020c0f72339634c59d5b1dd1d0351be7e2ffb3edba4af720a51aabd42499e0d6d3fe70ef5e4aa00deef168720a9b81a6bc2a519cb1ee9de6291000885b951a5e52dc273a92c6fc3", test_root_ca);
  const int chunkSize = 1024;
  long fileSize = file.size();
  int numUploads = fileSize / chunkSize;
  int remainder = fileSize % chunkSize;
  uint8_t buffer[chunkSize];
  for (int i = 0; i < numUploads; i++) {
    file.read(buffer, chunkSize);
    char contentRange[50];
    snprintf(contentRange, sizeof(contentRange), "bytes %d-%d/%d", i * chunkSize, (i * chunkSize) + (chunkSize - 1), fileSize);
    http.addHeader("Content-Range", contentRange);
    http.addHeader("File_name", filename); // Sesuaikan dengan nama file
    http.addHeader("Content-Type", "application/octet-stream");
    int httpResponseCode = http.PUT(buffer, chunkSize);
    if (httpResponseCode > 0) {
      Serial.printf("Sent %d bytes. Response code: %d\n", chunkSize, httpResponseCode);
    } else {
      Serial.println("Error sending data");
      break;
    }

    delay(100); // Sesuaikan jeda jika diperlukan
  }

  if (remainder > 0) {
    file.read(buffer, remainder);
    char contentRange[50];
    snprintf(contentRange, sizeof(contentRange), "bytes %d-%d/%d", numUploads * chunkSize, (numUploads * chunkSize) + remainder - 1, fileSize);
    http.addHeader("Content-Range", contentRange);
    http.addHeader("File_name", filename); // Sesuaikan dengan nama file
    http.addHeader("Content-Type", "application/octet-stream");

    int httpResponseCode = http.PUT(buffer, remainder);
    if (httpResponseCode > 0) {
      Serial.printf("Sent %d bytes. Response code: %d\n", remainder, httpResponseCode);
    } else {
      Serial.println("Error sending data");
    }
  }

  http.end();
  file.close();
}
