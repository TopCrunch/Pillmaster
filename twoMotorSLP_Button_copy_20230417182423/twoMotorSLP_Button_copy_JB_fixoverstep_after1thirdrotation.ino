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

// global variables for counting steps occured, used for fixing overstepping
int mainStepcount;
int adjStepcount;
bool rotationDirection = false; // false means it's rotating in a direction to dispense the pill, true false means it's rotating in a direction to return to it's original spot

void setup()
{
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
void multiStep(int increment)
{
  for(int i = 0; i < STEPS/3; i++)
  {
    stepBoth(increment);
    if (rotationDirection == false)
    {
      mainStepcount++;
      adjStepcount--;
    }
    else
    {
      mainStepcount--;
      adjStepcount++;
    }
  }
}

/*
switch which motor is enabled
*/
void swapMotor(driver d)
{
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
void stepMain(int steps)
{
    mnStepper.step(steps);
}

//step the adjustment motor
void stepAdj(int steps)
{
    adjStepper.step(steps);
}

void stepBoth(int steps)
{
  stepMain(steps);
  //adjuster needs inverted direction because it is flipped
  stepAdj(-steps);
}

//step both motors for the total steps. Don't use this!!!
void blockStep()
{
  stepBoth(STEPS/3);
  stepBoth(-STEPS/3);
}

void loop()
{
  if(digitalRead(buttonPin) == HIGH)
  {
    Serial.println("Button Pressed");
    multiStep(1);
    rotationDirection = true;
    multiStep(-1);
    rotationDirection = false;

    // correcting overstepping with the global variables 
    if (mainStepcount < 0)
    {
      while (mainStepcount != 0)
      {
        stepMain(1);
        mainStepcount++;
      }
    }
    else if (mainStepcount > 0)
    {
      while (mainStepcount != 0)
      {
        stepMain(-1);
        mainStepcount--;
      }
    }

    if (adjStepcount < 0)
    {
      while (adjStepcount != 0)
      {
        stepAdj(1);
        adjStepcount++;
      }
    }
    else if (adjStepcount > 0)
    {
      while (adjStepcount != 0)
      {
        stepAdj(-1);
        adjStepcount--;
      }
    }

    if(toggle) {
      swapMotor(A);
    } else {
      swapMotor(B);
    }
    toggle = !toggle;
  }
}
