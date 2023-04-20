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

Stepper stepper(STEPS, 4,5,6,7);
const int buttonPin = 2;
const int slpA = 8;
const int slpB = 9;
bool toggle = 0;
int buttonState = 0;

void setup(){
  Serial.begin(9600);
  Serial.println("Testing Stepper Motor");
  stepper.setSpeed(30);
  pinMode(buttonPin, INPUT);
  pinMode(slpA, OUTPUT);
  pinMode(slpB, OUTPUT);

  swapMotor(A);
}

void multiStep(int numSteps) {
  for(int i = 0; i < STEPS/3; i++) {
    stepper.step(numSteps);
  }
}

void swapMotor(driver d) {
  if(d == A) {
    digitalWrite(slpA, HIGH);
    digitalWrite(slpB, LOW);
  } else if (d == B) {
    digitalWrite(slpA, LOW);
    digitalWrite(slpB, HIGH);
  }
}

void blockStep() {
    stepper.step(STEPS/3);                                  
    stepper.step(-STEPS/3);
}

void loop(){
  if(digitalRead(buttonPin) == HIGH) {
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