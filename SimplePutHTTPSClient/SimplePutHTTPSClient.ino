
#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <stdio.h>
#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////
char ssid[] = "KONTRAKAN ALUMNI";
char pass[] = "TMUA1443H";

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


char serverAddress[] = "storage.googleapis.com";  // server address
char pathAddress[] = "/web-xerpihan.appspot.com/noot/sendFileBismillah.wav?X-Goog-Algorithm=GOOG4-RSA-SHA256&X-Goog-Credential=web-xerpihan%40appspot.gserviceaccount.com%2F20240102%2Fauto%2Fstorage%2Fgoog4_request&X-Goog-Date=20240102T022800Z&X-Goog-Expires=900&X-Goog-SignedHeaders=host&X-Goog-Signature=97d9c7c5b085be2946face503beed8e76e62ff9a71b5a367dd0c947eca9adf18907fe25ad7c794bb8cf202e5c29352a61be0d8855ca90191ffc7031ec7fba8e18eb10b61b4d1cd1f5b9b66a00ecf453764987a55b59aecf0086dffcde8a7bd909c54bc4c54349072bcb638a3825bfeebe7ac655f8cf9e589099ee876781bf74d69d97fc17625388419aafa5d045ccc19897e246b9ad81ebf6a942a9b44bd8b74db06a9d8464884ac5165fe70c24fb6efa644f739189d402ab4b0e87bdcc11443955ab975f9f2745b89a737181d325466b1e93368ef10588200f10d096bd45589d7616119626c65ce776f2579889ad261b9188a63b0c2035bd44dd2b54b2c60a2";
int port = 443;
int httpResponseCode;
HTTPClient https;
WiFiMulti WiFiMulti;
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
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, pass);

  // wait for WiFi connection
  Serial.print("Waiting for WiFi to connect...");
  while ((WiFiMulti.run() != WL_CONNECTED)) {
    Serial.print(".");
  }
  Serial.println(" connected");

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  initLittleFS();
}

void loop() {
  Serial.println("making PUT request");

  Serial.print("[HTTPS] begin...\n");
  WiFiClientSecure *client = new WiFiClientSecure;
  if (client) {
    client -> setCACert(test_root_ca);

    {
      if (https.begin(*client, "https://storage.googleapis.com/web-xerpihan.appspot.com/noot/sendFileBismillah.wav?X-Goog-Algorithm=GOOG4-RSA-SHA256&X-Goog-Credential=web-xerpihan%40appspot.gserviceaccount.com%2F20240102%2Fauto%2Fstorage%2Fgoog4_request&X-Goog-Date=20240102T022800Z&X-Goog-Expires=900&X-Goog-SignedHeaders=host&X-Goog-Signature=97d9c7c5b085be2946face503beed8e76e62ff9a71b5a367dd0c947eca9adf18907fe25ad7c794bb8cf202e5c29352a61be0d8855ca90191ffc7031ec7fba8e18eb10b61b4d1cd1f5b9b66a00ecf453764987a55b59aecf0086dffcde8a7bd909c54bc4c54349072bcb638a3825bfeebe7ac655f8cf9e589099ee876781bf74d69d97fc17625388419aafa5d045ccc19897e246b9ad81ebf6a942a9b44bd8b74db06a9d8464884ac5165fe70c24fb6efa644f739189d402ab4b0e87bdcc11443955ab975f9f2745b89a737181d325466b1e93368ef10588200f10d096bd45589d7616119626c65ce776f2579889ad261b9188a63b0c2035bd44dd2b54b2c60a2")) {  // HTTPS
        Serial.print("[HTTPS] GET...\n");

        char *fname = "/sendFileBismillah.wav";
        FILE *fp = fopen(fname, "rb"); // read in bytes
        //get file size
        fseek(fp, 0, SEEK_END); //send file pointer to end of file
        int file_size = ftell(fp); //get end position of file
        fseek(fp, 0, SEEK_SET); //send pointer back to start

        int max_upload_size = 10; // array size, larger = less uploads but too large can cause memory issues
        int num_of_uploads = file_size / max_upload_size; // figure out how many evenly sized upload chunks we need
        int num_of_uploads_mod = file_size % max_upload_size; //find out size of remaining upload chunk if needed
        int i;
        // start connection and send HTTP header
        if (num_of_uploads > 0) {
          uint8_t  buff1[max_upload_size] = {};
          for (i = 0; i < num_of_uploads; i++) {
            fread(buff1, sizeof * buff1, sizeof buff1 / sizeof * buff1, fp);
            https.addHeader("File_name", "sendFileBismillah.wav"); //header to say what the file name is
            https.addHeader("Content-Type", "application/octet-stream");
            httpResponseCode = https.PUT(buff1, sizeof(buff1));
            //        https.put(pathAddress, "application/octet-stream", sizeof(buff1), buff1);
          }
        }

        if (num_of_uploads_mod > 0) {
          int remainder = file_size - num_of_uploads * max_upload_size;
          uint8_t  buff2[remainder] = {};
          fread(buff2, sizeof * buff2, sizeof buff2 / sizeof * buff2, fp);
          https.addHeader("File_name", "sendFileBismillah.wav"); //header to say what the file name is
          https.addHeader("Content-Type", "application/octet-stream");
          httpResponseCode = https.PUT(buff2, sizeof(buff2)); // what the file name is
          //      https.put(pathAddress, "application/octet-stream", sizeof(buff2), buff2);

        }
        // start connection and send HTTP header
//        int httpCode = https.GET();

        // httpCode will be negative on error
        if (httpResponseCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] GET... code: %d\n", httpResponseCode);

          // file found at server
          if (httpResponseCode == HTTP_CODE_OK || httpResponseCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = https.getString();
            Serial.println(payload);
          }
        } else {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpResponseCode).c_str());
        }

        https.end();
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
      }

      delete client;
    }
  } else {
    Serial.println("Unable to create client");
  }

  Serial.println();
  Serial.println("Waiting 10s before the next round...");
  delay(10000);
}
