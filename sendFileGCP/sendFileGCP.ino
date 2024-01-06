/**
   Created by K. Suwatchai (Mobizt)

   Email: k_suwatchai@hotmail.com

   Github: https://github.com/mobizt/Firebase-ESP-Client

   Copyright (c) 2023 mobizt

*/

// This example shows how to upload file to Firebase storage bucket via the Google Cloud Storage JSON API.
// The file media.mp4 in the data folder should be uploaded to the device flash memory before test.
// The Google Cloud Storage JSON API function required OAuth2.0 authen.

// If SD Card used for storage, assign SD card type and FS used in src/FirebaseFS.h and
// change the config for that card interfaces in src/addons/SDHelper.h

#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the SD card interfaces setting and mounting
#include <addons/SDHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "KONTRAKAN ALUMNI"
#define WIFI_PASSWORD "TMUA1443H"

/* 2. Define the Firebase storage bucket ID e.g bucket-name.appspot.com or Google Cloud Storage bucket name */
#define STORAGE_BUCKET_ID "microphone-iot-demo-bucket1"

/* 3 The following Service Account credentials required for OAuth2.0 authen in Google Cloud Storage JSON API upload */
#define FIREBASE_PROJECT_ID "microphoneiot"
#define FIREBASE_CLIENT_EMAIL "avivsabilalm@microphoneiot.iam.gserviceaccount.com"
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQCgf9SmwKV2pGqC\ncY83qALnPtMmySvFqE9TKi1gRHogYM+IqxdJZzCFmBdWIslayMED9xY+6U6T7F1V\nnu/Xb4pKoh9PAQE5dvhs+efIMwf/+KHAyhlP4OCbCWGBjrL1QahsRaRfrsg0TJrt\n14LKM552pui0WP2dxq0NfnXXSgGhLCp0XiWKOcLUm5ISzcNzPlxQuWJ+BIauc+ZM\nfpAwXuEiuukB8ogsk/svdkakqUD9wQuserojkONR8XqHfc/qZy5LvFw5yJZ4QPPb\nbO+kqZn5vRJ5ToQYyOBrNtwRDOH9LBr9KRke2NomgQxnOU0g1tudSqme24SihVLh\nI0x7q1C9AgMBAAECggEANXYfGWR2OSzPJaA5+moGfYyZvQOphu9nbOZRsw1cOMjG\nmFmVKigrK3Tb4R85Pvo8ANwfFtmUCQXkmQ6WCYbn47BZCXHmPIP6gCJGKJOHCKaD\nCrrG76qSZlIbnpIv3i3+sgIInK6VlYYm3S66LCxBjnhObzRtVk7FegczrYDDRjHu\nT8zQVtciu3h0XfMRRKzYTroRqEP7qJaL6tQW8kuQVy+UouhL2mZh90s15fVPOLel\nh8S2w3jd48kjx6cWdsb4I6UbOvKmARec+GZkj4gqEmp/hQ74Y7cKIcR3j2y0/ELd\nhkGxP1871b0KfgAxl6L8yEgBj4WmIgPBGVnXzxCDqQKBgQDfMYEh6dh5ajcNfCmP\nP2qeje9fhpUgtWeIa/Mr2/9mEV6MnJ82+RIM9N5zGJ5jo10kfoJSGcjNOM6oskfj\n16b8GIi4Shm6qLD7npY2fjwR4IM/kHOsNKSOsu8CbuRFAA2atPuEy5HnQHri8sGe\n+wMa2T8hdV3oXwDR24s560j7pwKBgQC4FznL4WaLpHlk07jgQ4Q4hiH7nxybUuvm\nhvlGv+dFj39DM1WJUYCc5PLoVkWUXTHNbxlgXdhM7QfOSDH7A9c3llO3EvWPqKgg\nZAOsWFpT1hgNCKEVKxjIGyHQ0OY1062Ant+ByMCdI7KdwD083fsalGCZ7BjzNiZw\nx1wQTUFM+wKBgQCpSFfkDARHWps03CZ9YqEQbyDOeDolm9FR9GZ0rqDQnn3y8lwE\nCvd44kTSUSrY8bAUnOtlL+9QUDTWWJoUIp2heU5BUNqg64+I2OifnjEbcgi6Lmq9\ngxQIs4h6dPcnqnmnDHv0ZZ5dIlfSSEM4vLPHKl0qmGVIeL4bbYP0wxMQ4wKBgGDg\nGBIqc4RY8WkV4dxiq1nzfD6l5K/4WxvdO6D3rAT3FsmOYhz5Pu1RrmKFw+fngyXQ\nqVIFEKaqtcjgRS0LQF6IRVW6LG+FGbSx7RztAdvMdqulNt+k5k2nuYvh4sPAuTVO\nZeAufO40svJBD/jP+CxsmMlnbVnPzTrcLp6FiS+xAoGAI0VWfLDYoen400zXOqU9\nMl8ZA5ASTkVpfHi8er3aUROAa16MdWnKCXH6lh0Dk9jeXbG/sgY2T8pYVPf0FLUY\neB9i65NRW9pMwdRclkQ85fsi8flGhrOxP/IX+ODhmQtWOyZ80J47hwW7WswcJqCq\n8SoXLu+KRi7mHipWpI84cts=\n-----END PRIVATE KEY-----\n";
#define FILE_AUDIO_PATH "/BISMILLAH_GCP.wav"
#define BUCKET_AUDIO "/AUDIO/BISMILLAH_GCP.wav"
// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool taskCompleted = false;

// The Google Cloud Storage upload callback function
void gcsUploadCallback(UploadStatusInfo info);


void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
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
void setup()
{

  Serial.begin(115200);
  initWiFi();
  initLittleFS();
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the Service Account credentials for OAuth2.0 authen */
  config.service_account.data.client_email = FIREBASE_CLIENT_EMAIL;
  config.service_account.data.project_id = FIREBASE_PROJECT_ID;
  config.service_account.data.private_key = PRIVATE_KEY;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  /* Assign upload buffer size in byte */
  // Data to be uploaded will send as multiple chunks with this size, to compromise between speed and memory used for buffering.
  // The memory from external SRAM/PSRAM will not use in the TCP client internal tx buffer.
  config.gcs.upload_buffer_size = 2048;

  // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.reconnectNetwork(true);

  // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
  // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

  Firebase.begin(&config, &auth);
}

// The Google Cloud Storage upload callback function
void gcsUploadCallback(UploadStatusInfo info)
{
  if (info.status == firebase_gcs_upload_status_init)
  {
    Serial.printf("Uploading file %s (%d) to %s\n", info.localFileName.c_str(), info.fileSize, info.remoteFileName.c_str());
  }
  else if (info.status == firebase_gcs_upload_status_upload)
  {
    Serial.printf("Uploaded %d%s, Elapsed time %d ms\n", (int)info.progress, "%", info.elapsedTime);
  }
  else if (info.status == firebase_gcs_upload_status_complete)
  {
    Serial.println("Upload completed\n");
    FileMetaInfo meta = fbdo.metaData();
    Serial.printf("Name: %s\n", meta.name.c_str());
    Serial.printf("Bucket: %s\n", meta.bucket.c_str());
    Serial.printf("contentType: %s\n", meta.contentType.c_str());
    Serial.printf("Size: %d\n", meta.size);
    Serial.printf("Generation: %lu\n", meta.generation);
    Serial.printf("ETag: %s\n", meta.etag.c_str());
    Serial.printf("CRC32: %s\n", meta.crc32.c_str());
    Serial.printf("Tokens: %s\n", meta.downloadTokens.c_str());      // only gcs_upload_type_multipart and gcs_upload_type_resumable upload types.
    Serial.printf("Download URL: %s\n", fbdo.downloadURL().c_str()); // only gcs_upload_type_multipart and gcs_upload_type_resumable upload types.
  }
  else if (info.status == firebase_gcs_upload_status_error)
  {
    Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
  }
}

void loop()
{
  File file = LittleFS.open(FILE_AUDIO_PATH);
  file.close();

  // Firebase.ready() should be called repeatedly to handle authentication tasks.

  if (Firebase.ready() && !taskCompleted)
  {
    taskCompleted = true;

    Serial.println("\nUpload file via Google Cloud Storage JSON API...\n");

    /**
       The following function uses Google Cloud Storage JSON API to upload the file (object).
       The Google Cloud Storage functions required OAuth2.0 authentication.
       The upload types of methods can be selectable.

       The gcs_upload_type_simple upload type is used for small file upload in a single request without metadata.
       gcs_upload_type_multipart upload type is for small file upload in a single reques with metadata.
       gcs_upload_type_resumable upload type is for medium or large file (larger than or equal to 256 256 KiB) upload with metadata and can be resumable.

       The upload with metadata supports allows the library to add the metadata internally for Firebase to request the download access token in Firebase Storage bucket.
       User also can add custom metadata for the uploading file (object).
    */

    // For query parameters description of UploadOptions, see https://cloud.google.com/storage/docs/json_api/v1/objects/insert#optional-parameters
    // For request payload properties description of Requestproperties, see https://cloud.google.com/storage/docs/json_api/v1/objects/insert#optional-properties
    // The file systems for flash and SD/SDMMC can be changed in FirebaseFS.h.
    Firebase.GCStorage.upload(&fbdo, STORAGE_BUCKET_ID /* Firebase or Google Cloud Storage bucket id */, FILE_AUDIO_PATH /* path to local file */, mem_storage_type_flash /* memory storage type, mem_storage_type_flash and mem_storage_type_sd */, gcs_upload_type_resumable /* upload type */, BUCKET_AUDIO /* path of remote file stored in the bucket */, "audio/wav" /* mime type */, nullptr /* UploadOptions data */, nullptr /* Requestproperties data */, nullptr /* UploadStatusInfo data to get the status */, gcsUploadCallback /* callback function */);
  }
}
