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

Stepper stepper(STEPS, 4,5,6,7);
Stepper stepperNew(STEPS, 8,9,10,11);
const int buttonPin = 2;
int buttonState = 0;

void setup(){
  Serial.begin(9600);
  Serial.println("Testing Stepper Motor");
  stepper.setSpeed(30);
  stepperNew.setSpeed(30);
  pinMode(buttonPin, INPUT);
}
void multiStep(int numSteps) {
  for(int i = 0; i < STEPS/3; i++) {
    stepper.step(numSteps);
    stepperNew.step(numSteps);
  }
}

void blockStep() {
    stepper.step(STEPS/3);                                  
    stepper.step(-STEPS/3);
    stepperNew.step(STEPS/3);
    stepperNew.step(-STEPS/3);
}

void loop(){
  if(digitalRead(buttonPin) == HIGH) {
    Serial.println("Button Pressed");
    multiStep(1);
    multiStep(-1);
  }
}