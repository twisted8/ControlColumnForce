// Bounce.pde
// -*- mode: C++ -*-
//
// Make a single stepper bounce from one limit 8o another
//


//Take 12-24 volt power supply and wire + in to pin 9 on driver
//Ground to pin 6 to turn on motor.
//You can put extra 1 switch or industrial push button on off.For turning on off servo motor if you wish

//ground wire from arduino in to pin 14 and pin 5 of driver
//Pulse pin from arduino to pin 3 of driver
//Direction pin from arduino in to pin 4 of driver.

#include <AccelStepper.h>

int driverPUL = 8;    // PUL- pin
int driverDIR = 9;    // DIR- pin

// Define a stepper and the pins it will use
AccelStepper stepper(1, driverPUL, driverDIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

void setup()
{  
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(1200);
  stepper.setAcceleration(10000);
  stepper.moveTo(1600);
  
}

void loop()
{
    // If at the end of travel go to the other end
    if (stepper.distanceToGo() == 0)
      stepper.moveTo(-stepper.currentPosition());

    stepper.run();
}
