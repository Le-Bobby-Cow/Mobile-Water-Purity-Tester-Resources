/*-----------------------------------------------------------------------------------------
  Manager for the TFT Display and Touch Functionality

  Display Connected to Pins defined in the Main File

  ------------------------------------------------------------------------------------------*/
//TFT
#include "TFT_ILI9341.h"
#include "TFT_Touch.h"
#include <SPI.h>

#define DOUT 44
#define DIN  45
#define DCS  46
#define DCLK 47
//CS   -  42
//RST  -  48
//DC   -  49
//MOSI -  51
//SCK  -  52

TFT_ILI9341 tft = TFT_ILI9341();
TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);

int color = TFT_WHITE;
#define TFT_GREY 0x5AEB
unsigned int colors[10] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_BLACK, TFT_CYAN, TFT_YELLOW, TFT_WHITE, TFT_MAGENTA, TFT_BLACK, TFT_BLACK};

bool fileSplash = false;
int splashcount = 0;

void setupLCD() {
  tft.init();
  touch.setCal(3611, 671, 3397, 698, 320, 240, 1);

  tft.setTextColor(TFT_BLACK);
  tft.setRotation(1);
  touch.setRotation(1);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);

  //tft.setTextColor(TFT_GREEN);
}

bool readTouch() {
  int X_RawData;
  int Y_RawData;
  int X_Coord;
  int Y_Coord;

  if (touch.Pressed()) {
    X_Coord = touch.X();
    Y_Coord = touch.Y();
    //Serial.println("XY");
    //Serial.println(X_Coord);
    //Serial.println(Y_Coord);
    switch (MenuState) {
      case bootPage:
        break;
      case metersPage:
        if (X_Coord >= 0 && X_Coord <= 100 && Y_Coord >= 0 && Y_Coord <= 150) {
          if (recording) {
            recording = false;
            closeLogFile();
          } else if (!recording) {
            if (SDCardPresent) {
              startLogFile();
            } else if (!SDCardPresent) {

            }
            fileSplash = true;
            splashcount = 3;
          }
        }
        if (X_Coord >= 0 && X_Coord <= 100 && Y_Coord >= 150 && Y_Coord <= 220) {
          if (SDCardPresent) {
            if (recording) {
              closeLogFile();
              fileSplash = true;
              splashcount = 3;
            }
          } else {
            setupSD();
          }
        }
        delay(250);
        break;
    }
  }
  return true;
}

void clearDisplay() {
  tft.fillScreen(TFT_BLACK);
}

bool TFTdisplay() {

  switch (MenuState) {
    case bootPage:
      tft.fillRect(10, 10, 300, 60, TFT_WHITE);
      tft.fillRect(20, 50, 250, 4, TFT_BLUE);
      tft.fillRect(10, 90, 250, 40, TFT_WHITE);
      tft.setTextSize(2);
      tft.setCursor(15, 15);
      tft.print("ARDUINO WATER TESTER");
      tft.setTextSize(2);
      tft.setCursor(15, 95);
      tft.print(logTime());
      break;
    case metersPage:
      if (recording == true) {
        tft.fillRect(10, 10, 20, 150, TFT_RED);
        tft.setTextSize(1);
        tft.setRotation(2);
        tft.setCursor(20, 295);
        tft.print("RECORDING");
        tft.setCursor(21, 295);
        tft.print("RECORDING");
        tft.setRotation(1);
      } else {
        tft.fillRect(10, 10, 20, 150, TFT_GREEN);
        tft.setTextSize(1);
        tft.setRotation(2);
        tft.setCursor(20, 295);
        tft.print("Press to Begin Log");
        tft.setCursor(21, 295);
        tft.print("Press to Begin Log");
        tft.setRotation(1);
      }

      if (SDCardPresent == true) {
        tft.fillRect(10, 170, 20, 50, TFT_GREEN);
        tft.setTextSize(1);
        tft.setRotation(2);
        tft.setCursor(175, 295);
        tft.print("SD GOOD");
        tft.setCursor(176, 295);
        tft.print("SD GOOD");
        tft.setRotation(1);
      } else {
        tft.fillRect(10, 170, 20, 50, TFT_RED);
        tft.setTextSize(1);
        tft.setRotation(2);
        tft.setCursor(175, 295);
        tft.print("NO CARD");
        tft.setCursor(176, 295);
        tft.print("NO CARD");
        tft.setRotation(1);
      }

      if (!fileSplash) {
        tft.fillRect(60, 0, 120, 110, TFT_WHITE);
        tft.fillRect(190, 0, 120, 110, TFT_WHITE);
        tft.fillRect(60, 110, 120, 110, TFT_WHITE);
        tft.fillRect(190, 110, 120, 110, TFT_WHITE);

        tft.drawRect(60, 100, 120, 10, TFT_BLACK);
        tft.drawRect(190, 100, 120, 10, TFT_BLACK);
        tft.drawRect(60, 210, 120, 10, TFT_BLACK);
        tft.drawRect(190, 210, 120, 10, TFT_BLACK);

        tft.setTextSize(1);
        tft.setCursor(62, 101);
        tft.println("pH: " + String(pHVal, 2));
        tft.setCursor(192, 101);
        tft.println("EC (ppm): " + String(ECVal, 2));
        tft.setCursor(62, 211);
        tft.println("Temp (C): " + String(TempVal, 1));
        tft.setCursor(192, 211);
        tft.println("Turb. (NTU): " + String(TurbVal, 0));

        for (int i = 0; i < arrLengths - 1; i++) {
          tft.drawLine(60 + i * 2, ((100 / maxpH) * (maxpH - pHVals[i])), 60 + (1 + i) * 2, ((100 / maxpH) * (maxpH - pHVals[i + 1])), TFT_BLACK);
          tft.drawLine(61 + i * 2, ((100 / maxpH) * (maxpH - pHVals[i])), 61 + (1 + i) * 2, ((100 / maxpH) * (maxpH - pHVals[i + 1])), TFT_GREY);
          tft.drawLine(59 + i * 2, ((100 / maxpH) * (maxpH - pHVals[i])), 59 + (1 + i) * 2, ((100 / maxpH) * (maxpH - pHVals[i + 1])), TFT_GREY);
        }
        for (int i = 0; i < arrLengths - 1; i++) {
          tft.drawLine(190 + i * 2, ((100 / maxEC) * (maxEC - ECVals[i])), 190 + (1 + i) * 2, ((100 / maxEC) * (maxEC - ECVals[i + 1])), TFT_BLACK);
          tft.drawLine(191 + i * 2, ((100 / maxEC) * (maxEC - ECVals[i])), 191 + (1 + i) * 2, ((100 / maxEC) * (maxEC - ECVals[i + 1])), TFT_GREY);
          tft.drawLine(189 + i * 2, ((100 / maxEC) * (maxEC - ECVals[i])), 189 + (1 + i) * 2, ((100 / maxEC) * (maxEC - ECVals[i + 1])), TFT_GREY);
        }
        for (int i = 0; i < arrLengths - 1; i++) {
          tft.drawLine(60 + i * 2, 110 + ((100 / maxTemp) * (maxTemp - TempVals[i])), 60 + (1 + i) * 2, 110 + ((100 / maxTemp) * (maxTemp - TempVals[i + 1])), TFT_BLACK);
          tft.drawLine(61 + i * 2, 110 + ((100 / maxTemp) * (maxTemp - TempVals[i])), 61 + (1 + i) * 2, 110 + ((100 / maxTemp) * (maxTemp - TempVals[i + 1])), TFT_GREY);
          tft.drawLine(59 + i * 2, 110 + ((100 / maxTemp) * (maxTemp - TempVals[i])), 59 + (1 + i) * 2, 110 + ((100 / maxTemp) * (maxTemp - TempVals[i + 1])), TFT_GREY);
        }
        for (int i = 0; i < arrLengths - 1; i++) {
          tft.drawLine(190 + i * 2, 110 + ((100 / maxTurb) * (maxTurb - TurbVals[i])), 190 + (1 + i) * 2, 110 + ((100 / maxTurb) * (maxTurb - TurbVals[i + 1])), TFT_BLACK);
          tft.drawLine(191 + i * 2, 110 + ((100 / maxTurb) * (maxTurb - TurbVals[i])), 191 + (1 + i) * 2, 110 + ((100 / maxTurb) * (maxTurb - TurbVals[i + 1])), TFT_GREY);
          tft.drawLine(189 + i * 2, 110 + ((100 / maxTurb) * (maxTurb - TurbVals[i])), 189 + (1 + i) * 2, 110 + ((100 / maxTurb) * (maxTurb - TurbVals[i + 1])), TFT_GREY);
        }
      }

      if (fileSplash == true) {
        if (splashcount == 0) {
          fileSplash = false;
          clearDisplay();
        } else {
          splashcount--;
        }
      }

      if (fileSplash == true) {
        tft.fillRect(80, 40, 200, 100, TFT_WHITE);
        tft.drawRect(80, 40, 200, 100, TFT_BLACK);
        if (SDCardPresent) {
          if (recording) {
            tft.setCursor(85, 45);
            tft.println("File Created. Called:");
            tft.setCursor(85, 60);
            tft.println(currentFileName);
          } else {
            tft.setCursor(85, 45);
            tft.println("Error");
          }
        } else if (!SDCardPresent) {
          tft.setCursor(85, 45);
          tft.println("NO SD CARD INSERTED");
        }
      }
      break;
  }
  return true;

}
