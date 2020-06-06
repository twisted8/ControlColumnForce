// Bounce.pde
// -*- mode: C++ -*-
//
// Make a single stepper bounce from one limit 8o another
//
// Copyright (C) 2012 Mike McCauley
// $Id: Random.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>

int driverPUL = 8;    // PUL- pin
int driverDIR = 9;    // DIR- pin

// Define a stepper and the pins it will use
AccelStepper stepper(1, driverPUL, driverDIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

void setup()
{  
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(1600);
  stepper.setAcceleration(1200);
  stepper.moveTo(2600);
  
}

void loop()
{
    // If at the end of travel go to the other end
    if (stepper.distanceToGo() == 0)
      stepper.moveTo(-stepper.currentPosition());

    stepper.run();
}
