/*-----------------------------------------------------------------------------------------
  Manager for the Real Time Clock Module (DS3231)

  Module connected to dedicated SCL and SDA pins

  ------------------------------------------------------------------------------------------*/

#include <RTClib.h>

bool RTCPresent = false;
bool powerLoss = false;

RTC_DS3231 rtc;


void setupRTC() {
  if (!rtc.begin()) {
    RTCPresent = false;
    //Serial.println("Couldn't find RTC");
  } else {
    RTCPresent = true;
  }

  if (!rtc.lostPower()) {
    powerLoss = false;
  } else {
    powerLoss = true;
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void setRTC(int yyyy, int mo, int d, int h, int mi) {
  rtc.adjust(DateTime(yyyy, mo, d, h, mi , 0));
}

String logTime() {
  DateTime now = rtc.now();
  String logPhrase = "";
  logPhrase = (String(now.month()) + "/") + (String(now.day()) + "/") + (String(now.year()) + "  ") + (String(now.hour()) + ":") + (String(now.minute()) + ":") + String(now.second());
  return logPhrase;
}

String fileTimestamp() {
  DateTime now = rtc.now();
  String Timestamp = "";
  Timestamp = String(now.month()) + "." + String(now.day()) + "." + String(now.year()) + "-" + String(now.hour()) + "." + String(now.minute());
  return Timestamp;
}
