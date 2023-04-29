#include <Stepper.h>
/*
PillMaster Stepper Motor Program
Designed for 28BYJ-48 Stepper Motor
  5V DC 1:16
  running through DRV8833
  Stride Angle = 5.265 degrees / 64
  Full rotation is between 514 and 515 steps?
  will need to have slight adjustments between multiple runs
*/
#define STEPS 515

//main and adjustment stepper
Stepper mnStepper(STEPS, 8,9,10,11);
Stepper adjStepper(STEPS, 4,5,6,7);
const int loadDT = 2;
const int loadSCK = 1;
const int slpA = 12;
const int slpB = 13;
const int slpC = 3;

void setup(){
  Serial.begin(9600);
  Serial.println("Pillmaster Arduino setting up");
  mnStepper.setSpeed(30);
  //weight sensor setup goes here
  pinMode(slpA, OUTPUT);
  pinMode(slpB, OUTPUT);
  pinMode(slpC, OUTPUT);

  swapMotor('a');
}

/*
motors can't run at the same time so operate them in tiny increments one after the other
*/
void multiStep(int increment) {
  for(int i = 0; i < STEPS/3; i++) {
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
      digitalWrite(slpC, LOW);
      break;
    case 'b':
      digitalWrite(slpA, LOW);
      digitalWrite(slpB, HIGH);
      digitalWrite(slpC, LOW);
      break;
    case 'c':
      digitalWrite(slpA, LOW);
      digitalWrite(slpB, LOW);
      digitalWrite(slpC, HIGH);
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

//step both motors for the total steps. Don't use this!!!
void blockStep() {
  stepBoth(STEPS/3);
  stepBoth(-STEPS/3);
}

void loop(){
  if(Serial.available() > 0) {
    char serialByte = Serial.read();
    if(serialByte == 'a' || serialByte == 'b' || serialByte == 'c') {
      swapMotor(toLowerCase(serialByte));
      delay(500);
      Serial.println("Operating canister...");
      multiStep(1);
      multiStep(-1);
    }
  }
}