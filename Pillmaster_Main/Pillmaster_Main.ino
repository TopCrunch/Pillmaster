#include <Stepper.h>
#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif
/*
PillMaster Stepper Program
Designed for 28BYJ-48 Stepper Motor
  5V DC 1:16
  running through DRV8833
  Stride Angle = 5.265 degrees / 64
  Full rotation is between 514 and 515 steps?
  will need to have slight adjustments between multiple runs
*/
#define STEPS 514
#define THIRD_STEPS 171
#define SIXTH_STEPS 84  
#define adjustGear_FullyClose 71 // found out using the CAD software that the adjustment gear rotates 50 degrees to open/close the gear opening, so this is set to 71 steps to fully close the gear opening
#define adjustGear_HalfClose 36  // this is set to 36 which is about half of the third steps

#define ADJUSTER_A_OFFSET 0
#define ADJUSTER_B_OFFSET 0
#define ADJUSTER_C_OFFSET 0

int adjA_dir = 1;
int adjB_dir = 1;
int adjC_dir = 1;

//main and adjustment stepper
Stepper mnStepper(STEPS, 8,9,10,11);
Stepper adjStepper(STEPS, 4,5,6,7);
const int slpA = 12;
const int slpB = 13;
//const int slpC = 3; //replaced by NOR gate

const int weightDT = 2;
const int weightSCK = 3;
HX711_ADC LoadCell(weightDT, weightSCK);
const int calibrationAddr = 0;
unsigned long time = 0;

void setup(){
  Serial.begin(9600);
  Serial.println("Pillmaster Arduino setting up");
  mnStepper.setSpeed(30);
  adjStepper.setSpeed(30);
  pinMode(slpA, OUTPUT);
  pinMode(slpB, OUTPUT);
  //pinMode(slpC, OUTPUT); //replaced by NOR gate
  swapMotor('a');

  weightSetup();
}

void weightSetup() {
  Serial.println("Weight Sensor setting up...");
  LoadCell.begin();
  float calibrationValue;
  #if defined(ESP8266)|| defined(ESP32)
    EEPROM.begin(512);
  #endif
  EEPROM.get(calibrationAddr, calibrationValue); //get stored calibration value from EEPROM

  LoadCell.start(2000, true);
  if(LoadCell.getTareTimeoutFlag()) {
    Serial.println("Load cell timeout. Check wiring and reset");
    while(1);
  } else {
    LoadCell.setCalFactor(calibrationValue);
    Serial.println("Weight sensor setup complete");
  }
}

/*
motors can't run at the same time so operate them in tiny increments one after the other
*/
void multiStep(int increment, int max) {
  for(int i = 0; i < max; i++) {
    stepBoth(increment);
  }
}

/*
switch which motor is enabled
*/
void swapMotor(char val) {
  switch(val) {
    case 'a':
      digitalWrite(slpA, HIGH);
      digitalWrite(slpB, LOW);
      //digitalWrite(slpC, LOW);
      break;
    case 'b':
      digitalWrite(slpA, LOW);
      digitalWrite(slpB, HIGH);
      //digitalWrite(slpC, LOW);
      break;
    case 'c': //uses the NOR gate to actuvate the last motor set
      digitalWrite(slpA, LOW);
      digitalWrite(slpB, LOW);
      //digitalWrite(slpC, HIGH);
      break;
    default:
      //do nothing
      Serial.println("wrong byte value");
  }
}

//step the main motor
void stepMain(int steps) {
  mnStepper.step(steps);
}

//step the adjustment motor
void stepAdj(int steps){
  adjStepper.step(steps);
}

void stepBoth(int steps) {
  stepMain(steps);
  //adjuster needs inverted direction because it is flipped
  stepAdj(-steps);
}

void operateCanister() {
  multiStep(1, THIRD_STEPS);
  multiStep(-1, THIRD_STEPS);
}

void operateAdjusterSixth(int direction, int offset)
{  
  for(int i = 0; i < adjustGear_HalfClose + offset; i++) {
    stepAdj(direction);
  }
}

void operateAdjusterThird(int direction, int offset)
{  
  for(int i = 0; i < adjustGear_FullyClose + offset; i++) {
    stepAdj(direction);
  }
}

void loop(){
  if(Serial.available() > 0) {
    char serialByte = Serial.read();
    switch(serialByte) {
      case 'a':
        swapMotor('a');
        delay(500);
        Serial.println("Operating canister A...");
        operateCanister();
        break;
      case 'A':
        swapMotor('a');
        delay(500);
        Serial.println("Operating Adjuster A...");
        operateAdjusterSixth(adjA_dir, ADJUSTER_A_OFFSET);
        adjA_dir = adjA_dir * -1;
        break;
      case 'b':
        swapMotor('b');
        delay(500);
        Serial.println("Operating canister B...");
        operateCanister();
        break;
      case 'B':
        swapMotor('b');
        delay(500);
        Serial.println("Operating Adjuster B...");
        operateAdjusterSixth(adjB_dir, ADJUSTER_B_OFFSET);
        adjB_dir = adjB_dir * -1;
        break;
      case 'c':
        swapMotor('c');
        delay(500);
        Serial.println("Operating canister C...");
        operateCanister();
        break;
      case 'C':
        swapMotor('c');
        delay(500);
        Serial.println("Operating Adjuster C...");
        operateAdjusterSixth(adjC_dir, ADJUSTER_C_OFFSET);
        adjC_dir = adjC_dir * -1;
        break;
      case 'w':
        weightLoop();
        break;
    }
  }
}

void weightLoop() {
  boolean weightMode = true;
  static boolean newDataReady = false;
  const int interval = 0;
  while(weightMode) {
    if(LoadCell.update()) {
      newDataReady = true;
    }
    if(newDataReady) { //for smoothing out load cell data
      if(millis() > time + interval) {
        float d = LoadCell.getData();
        Serial.print("Load cell output value: ");
        Serial.println(d);
        newDataReady = 0;
        time = millis();
      }
    }
    if(Serial.available() > 0) {
      char serialByte = Serial.read();
      switch(serialByte) {
        case 't':
          LoadCell.tareNoDelay();
          break;
        case 'x':
          weightMode = false;
          break;
      }
    }

    if(LoadCell.getTareStatus() == true) {
      Serial.println("Tare completed");
    }
  }
}