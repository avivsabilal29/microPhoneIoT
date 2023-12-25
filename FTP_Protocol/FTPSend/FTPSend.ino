/**
   @file ftp-client.ino
   @author Phil Schatzmann
   @brief Receiving audio via FTP and writing to I2S of the AudioKit.
   Replace the userids, passwords and ip adresses with your own!
   And don't forget to read the Wiki of the imported projects
   @version 0.1
   @date 2023-11-09

   @copyright Copyright (c) 2023
*/
#include "LittleFS.h"
#include "WiFi.h"
#include "ArduinoFTPClient.h" // install https://github.com/pschatzmann/TinyFTPClient
#include "AudioTools.h" // https://github.com/pschatzmann/arduino-audio-tools
#include "AudioCodecs/CodecMP3Helix.h" // https://github.com/pschatzmann/arduino-libhelix
#include "AudioLibs/AudioKit.h" // https://github.com/pschatzmann/arduino-audiokit
const char* ssid = "KONTRAKAN ALUMNI";
const char* password = "TMUA1443H";
WiFiClient cmd;
WiFiClient data;
FTPClient client(cmd, data);
FTPFile file;
AudioKitStream kit; // or replace with e.g. I2SStream
MP3DecoderHelix mp3;
EncodedAudioStream decoder(&kit, &mp3);
StreamCopy copier(decoder, file);
File myfile;
void setup() {
  Serial.begin(115200);

  // connect to WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  myfile = LittleFS.open("/testingSend.mp3");
  if (!myfile) {
    Serial.println("Failed to open file for reading");
    return;
  }

  // optional logging
  FTPLogger::setOutput(Serial);
  // FTPLogger::setLogLevel(LOG_DEBUG);

  // open connection
  client.begin(IPAddress(46, 17, 172, 223), "avivsabilalm", "Mujtahid29");

  // copy data from file
  file = client.open("/testingSend.mp3");

  // open output device
  kit.begin();
  decoder.begin();
  myfile.close();

}

void loop() {
  copier.copy();
}
