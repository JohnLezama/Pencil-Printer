#include <SD.h>
#include <Stepper.h>
const int cs = 4; // Chip Select pin
File dataFile;
int globalY = 0;
int globalX = 0;

const int STEPS_PER_REV = 2048;
const int stickyNoteDimension = 76;
const int outerDiameter = 14;
const int ImageDimension = 400;

double numOfRevs = stickyNoteDimension/(outerDiameter*M_PI);
double coordinateToStepFactor = numOfRevs*STEPS_PER_REV/ImageDimension;

Stepper stepperX = Stepper(STEPS_PER_REV,6, 7, 8, 9);
Stepper stepperY = Stepper(STEPS_PER_REV,10, 11, 12, 13);

void setup() {
  stepperX.setSpeed(5);
  stepperY.setSpeed(5);
  Serial.begin(9600);
  Serial.print("Initializing SD card...");

  if (!SD.begin(cs)) {
    Serial.println("Card failed, or not present");
    return; // Exit if card doesn't initialize
  }
  Serial.println("Card initialized.");

  // Open the file for reading
  dataFile = SD.open("myFile.txt");
}

void loop(){
if (dataFile) {
    while (dataFile.available()) {
      writeX(-globalX);
      globalX = 0;
      String line = dataFile.readStringUntil('\n'); // read one line

      int colonIndex = line.indexOf(':');
      if (colonIndex == -1) continue; // skip malformed lines

      String group = line.substring(0, colonIndex);
      group.trim();
      writeY(group.toInt());
      globalY = group.toInt();
      int openParen = line.indexOf('(');
      int closeParen = line.indexOf(')');
      String numbers = line.substring(openParen + 1, closeParen);
      numbers.trim();
      int lastComma = 0;
      while (true) {
        int comma = numbers.indexOf(',');
        if (comma == -1) break;
        numbers = numbers.substring(comma+1);
        numbers.trim();
        if (numbers.length() > 0) {
          int xValue = numbers.substring(0,numbers.indexOf(',')).toInt();
          writeX(xValue);
          globalX = xValue;
        }
      }
    }
  }
}

void writeY(int y){
  int steps = int(coordinateToStepFactor*(y-globalY));
  stepperY.step(steps);
}
void writeX(int x){
  int steps = int(coordinateToStepFactor*(x-globalX));
  stepperX.step(steps);
}