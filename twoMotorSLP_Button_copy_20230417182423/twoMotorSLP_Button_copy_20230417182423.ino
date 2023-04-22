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

enum driver {
  A,
  B
};

//main and adjustment stepper
Stepper mnStepper(STEPS, 4,5,6,7);
Stepper adjStepper(STEPS, 8,9,10,11);
const int buttonPin = 2;
const int slpA = 12;
const int slpB = 13;
bool toggle = 0;
int buttonState = 0;

void setup(){
  Serial.begin(9600);
  Serial.println("Testing Stepper Motor");
  mnStepper.setSpeed(30);
  pinMode(buttonPin, INPUT);
  pinMode(slpA, OUTPUT);
  pinMode(slpB, OUTPUT);

  swapMotor(A);
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
void swapMotor(driver d) {
  switch(d) {
    case A:
      digitalWrite(slpA, HIGH);
      digitalWrite(slpB, LOW);
      break;
    case B:
      digitalWrite(slpA, LOW);
      digitalWrite(slpB, HIGH);
      break;
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
  if(digitalRead(buttonPin) == HIGH) {
    Serial.println("Button Pressed");
    multiStep(1);
    multiStep(-1);
    if(toggle) {
      swapMotor(A);
    } else {
      swapMotor(B);
    }
    toggle = !toggle;
  }
}