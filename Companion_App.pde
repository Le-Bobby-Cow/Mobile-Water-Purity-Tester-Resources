import processing.serial.*;

Serial myPort;  // Create object from Serial class
String input = ",,,,";      // Data received from the serial port
String[] splitInputs = {"", "", "", "", ""};

int arrLengths = 150;
float pHVal, ECVal, TempVal, TurbVal;
float[] pHVals = new float[arrLengths];
float[] ECVals = new float[arrLengths];
float[] TempVals = new float[arrLengths];
float[] TurbVals = new float[arrLengths];

float maxpH, maxEC, maxTemp, maxTurb;


void setup() 
{
  size(1200, 600);
  // Initialize the Serial Port
  try {
    myPort = new Serial(this, Serial.list()[0], 9600);
    myPort.bufferUntil('\n');
  } 
  catch (Exception e) {
    stop();
  }
}

void draw()
{
  // Draw all data
  background(195);
  drawGraphs();
}

void drawGraphs() {
  // Draw all line graphs
  stroke(0);
  strokeWeight(2);
  fill(255);
  rect(50, 50, 300, 200);
  rect(50, 300, 300, 200);
  rect(425, 50, 300, 200);
  rect(425, 300, 300, 200);
  stroke(255, 0, 0);
  strokeWeight(2); 

  for (int i = 0; i < pHVals.length - 1; i++) {
    line(50 + i * 2, 50 + ((200 / maxpH) * (maxpH - pHVals[i])), 50 + (1 + i) * 2, 50 + ((200 / maxpH) * (maxpH - pHVals[i + 1])));
  }
  for (int i = 0; i < ECVals.length - 1; i++) {
    line(425 + i * 2, 50 + ((200 / maxEC) * (maxEC - ECVals[i])), 425 + (1 + i) * 2, 50 + ((200 / maxEC) * (maxEC - ECVals[i + 1])));
  }
  for (int i = 0; i < TempVals.length - 1; i++) {
    line(50 + i * 2, 300 + ((200 / maxTemp) * (maxTemp - TempVals[i])), 50 + (1 + i) * 2, 300 + ((200 / maxTemp) * (maxTemp - TempVals[i + 1])));
  }
  for (int i = 0; i < TurbVals.length - 1; i++) {
    line(425 + i * 2, 300 + ((200 / maxTurb) * (maxTurb - TurbVals[i])), 425 + (1 + i) * 2, 300 + ((200 / maxTurb) * (maxTurb - TurbVals[i + 1])));
  }

// Draw rating graphs

  fill(0);
  stroke(255);
  textSize(20);
  textAlign(LEFT, BOTTOM);
  text("pH: " + pHVal, 50, 275);
  text("EC: " + ECVal, 425, 275);
  text("Temperature: " + TempVal, 50, 525);
  text("Turbidity: " + TurbVal, 425, 525);

  fill(255, 0, 0);
  stroke(0);
  rect(800, 50, 50, 500);
  fill(#FFA500);
  rect(800, 175, 50, 250);
  fill(#FFFF00);
  rect(800, 225, 50, 150);
  fill(0, 255, 0);
  rect(800, 250, 50, 100);
  fill(255);

  fill(255, 0, 0);
  stroke(0);
  rect(900, 50, 50, 500);
  fill(#FFA500);
  rect(900, 250, 50, 250);
  fill(#FFFF00);
  rect(900, 400, 50, 150);
  fill(0, 255, 0);
  rect(900, 500, 50, 50);

  fill(255, 0, 0);
  stroke(0);
  rect(1000, 50, 50, 250);
  fill(255, 255, 0);
  rect(1000, 300, 50, 100);
  fill(0, 255, 0);
  rect(1000, 400, 50, 50);
  fill(0, 255, 255);
  rect(1000, 450, 50, 50);
  fill(0, 0, 255);
  rect(1000, 500, 50, 50);

  fill(255);
  rect(1100, 50, 50, 500);

  fill(0);
  stroke(255);
  strokeWeight(8); 
  textSize(12);
  textAlign(CENTER, BOTTOM);
  text("pH", 825, 45);
  text("EC", 925, 45);
  text("Temp", 1025, 45);
  text("Turb", 1125, 45);

  line(800, 550 - (500 * (pHVal / 14)), 850, 550 - (500 * (pHVal / 14)));
  line(900, 550 - constrain((500 * (ECVal / 2000)), 0, 500), 950, 550 - constrain((500 * (ECVal / 2000)), 0, 500));
  line(1000, 550 - (500 * (TempVal / 105)), 1050, 550 - (500 * (TempVal / 105)));
  stroke(100);

  line(1100, 550 - (500 * (TurbVal / 3000)), 1150, 550 - (500 * (TurbVal / 3000)));

/*
  strokeWeight(2); 
  stroke(0);
  fill(255);
  fill(255, 0, 0);
  rect(100, 525, 120, 50);
  fill(#0FA8F7);
  rect(300, 525, 120, 50);
  fill(0);
  textSize(20);
  text("STOP", 160, 560);
  text("LOG", 360, 560);
  */
}

void manageValues() {

  // Manage the input string
  pHVal = float(splitInputs[1]);
  ECVal = float(splitInputs[2]);
  TempVal = float(splitInputs[3]);
  TurbVal = float(splitInputs[4]);

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
  for (int i = pHVals.length - 1; i > 0; i--) {
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
  for (int i = 0; i < pHVals.length; i++) {
    float maxpHTemp = maxpH;
    float maxECTemp = maxEC;
    float maxTempTemp = maxTemp;
    float maxTurbTemp = maxTurb;

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

  println(pHVal, ECVal, TempVal, TurbVal);
}

void serialEvent(Serial myPort) { 
  input = myPort.readString();         // read it and store it in val
  input = trim(input);
  println(input);
  String[] prevVals = splitInputs;
  splitInputs = split(input, ',');
  if (splitInputs.length < 5) {
    splitInputs = prevVals;
  }
  manageValues();
} 
