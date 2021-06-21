/*-----------------------------------------------------------------------------------------
  Manager for the SD Module

  Module Connected to SPI Pin Header

  ------------------------------------------------------------------------------------------*/
#include <SdFat.h>

const uint8_t SOFT_MISO_PIN = 29;
const uint8_t SOFT_MOSI_PIN = 31;
const uint8_t SOFT_SCK_PIN  = 33;
const uint8_t SD_CHIP_SELECT_PIN = 35;

SdFatSoftSpi<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> sd;

SdFile loggingFile;

bool SDCardPresent = false;
String currentFileName = "";

void setupSD() {
  pinMode(SOFT_MISO_PIN, INPUT_PULLUP);
  if (!sd.begin(SD_CHIP_SELECT_PIN)) {
    SDCardPresent = false;
    //sd.initErrorHalt();
    //Serial.println("NoCard");
  } else {
    SDCardPresent = true;
    //Serial.println("Card");
  }
}

void logData() {
  if (loggingFile.open(currentFileName.c_str(), O_RDWR | O_CREAT)) {
    if (recording) {
      loggingFile.print(dataString());
    }
    loggingFile.println();
  } else {
    recording = false;
    SDCardPresent = false;
    //Serial.println("errorSD");
  }
  loggingFile.close();
}

void startLogFile() {
  currentFileName = fileTimestamp() + ".csv";
  createFile();
}

void closeLogFile() {
  loggingFile.close();
  //Serial.println("sd close");
}

void createFile() {
  if (loggingFile.open(currentFileName.c_str(), O_RDWR | O_CREAT)) {
    loggingFile.println(F("Time,pH,Electrical Conductivity,Temperature,Turbidity"));
    recording = true;
  } else {
        recording = false;
        SDCardPresent = false;
  }
  loggingFile.close();
}
