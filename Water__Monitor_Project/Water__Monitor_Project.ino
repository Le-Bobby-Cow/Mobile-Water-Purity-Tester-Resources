
/*-----------------------------------------------------------------------------------------
   Arduino MESA Project 2020-2021

   Water Monitor Project
   - Monitors Various Conditions in Water for determineation of:
    - Safety
    - Quality
    - Conditions

   Integrated Modules:
   - SD Card Mod.
   - RTC Mod.
   - pH Probe
   - EC Probe
   - Temperature Probe
   - Turbidity Probe

  -----------------------------------------------------------------------------------------
*/

#include <arduino-timer.h>

auto timer = timer_create_default();

int MenuState = 0;
#define bootPage 0
#define metersPage 1

const int arrLengths = 60;
float pHVal, ECVal, TempVal, TurbVal;
float pHVals[arrLengths];
float ECVals[arrLengths];
float TempVals[arrLengths];
float TurbVals[arrLengths];

float maxpH, maxEC, maxTemp, maxTurb = 1;

float analogConstant;

bool recording = false;

void setup() {
  Serial.begin(9600);

  setupEC(); //Setup all devices
  setuppH();
  setupTemp();
  setupTurb();

  setupRTC();
  setupLCD();

  delay(500);
  setupSD();

  TFTdisplay(); //Boot screen to allow delay to occur
  delay(1000);

  analogConstant = (3.37 / 5.0) / (float(analogRead(A15)) / float(analogRead(A14)));

  timer.every(500, readSensors);
  timer.every(1000, TFTdisplay);
  timer.every(50, readTouch);
  timer.every(10000, calibrateVolt);

  delay(2000);
  MenuState = 1;
  clearDisplay();
  TFTdisplay();
}

void loop() {
  timer.tick(); //Internal Timer Setup
  switch (MenuState) {
    case bootPage: //Introductory Page
      break;
    case metersPage: //Main Meters Page
      break;
  }
}

bool readSensors() { //Read all sensor data
  //Gather Data
  //analogConstant = (3.37 / 5.0) / (float(analogRead(A15)) / float(analogRead(A14)));
  pHVal = calculatepH();
  ECVal = generateTDS();
  TempVal = generateTemperatureC();
  TurbVal = calculateTurbidity();

  //Determine Max Values
  if (pHVal > maxpH) {
    maxpH = 5 * ceil(pHVal / 5);
  }
  if (ECVal > maxEC) {
    maxEC = 100 * ceil(ECVal / 100);
  }
  if (TempVal > maxTemp) {
    maxTemp = 10 * ceil(TempVal / 10);
  }
  if (TurbVal > maxTurb) {
    maxTurb = 100 * ceil(TurbVal / 100);
  }

  //Limit the readings values
  constrain(pHVal, 0, maxpH);
  constrain(ECVal, 0, maxEC);
  constrain(TempVal, 0, maxTemp);
  constrain(TurbVal, 0, maxTurb);

  //Shift data array
  for (int i = arrLengths - 1; i > 0; i--) {
    pHVals[i] = pHVals[i - 1];
    ECVals[i] = ECVals[i - 1];
    TempVals[i] = TempVals[i - 1];
    TurbVals[i] = TurbVals[i - 1];
  }
  pHVals[0] = pHVal;
  ECVals[0] = ECVal;
  TempVals[0] = TempVal;
  TurbVals[0] = TurbVal;

  //Check for new high reading
  for (int i = 0; i < arrLengths; i++) {
    float maxpHTemp;
    float maxECTemp;
    float maxTempTemp;
    float maxTurbTemp;

    if (pHVals[i] > maxpHTemp) {
      maxpHTemp = pHVals[i];
    }
    if (ECVals[i] > maxECTemp) {
      maxECTemp = ECVals[i];
    }
    if (TempVals[i] > maxTempTemp) {
      maxTempTemp = TempVals[i];
    }
    if (TurbVals[i] > maxTurbTemp) {
      maxTurbTemp = TurbVals[i];
    }

    maxpH = constrain(5 * ceil(maxpHTemp / 5), 0, 15);
    maxEC = 100 * ceil(maxECTemp / 100);
    maxTemp = 10 * ceil(maxTempTemp / 10);
    maxTurb = 100 * ceil(maxTurbTemp / 100);
  }

  //Log Data if applicable
  if (recording) {
    logData();
  }

  //Send though serial port if applicable
  if (Serial) {
    Serial.print(logTime() + ",");
    Serial.print(String(pHVal) + ",");
    Serial.print(String(ECVal) + ",");
    Serial.print(String(TempVal) + ",");
    Serial.print(String(TurbVal) + '\n');
  }
  return true;
}

void calibrateVolt() {
  analogConstant = (3.37 / 5.0) / (float(analogRead(A15)) / float(analogRead(A14)));
  //Serial.println(analogConstant);
}

String dataString() {
  String data = logTime() + "," + String(pHVal) + "," + String(ECVal) + "," + String(TempVal) + "," + String(TurbVal);
  return data;
}
