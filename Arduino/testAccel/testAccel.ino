// Bounce.pde
// -*- mode: C++ -*-
//

//Take 12-24 volt power supply and wire + in to pin 9 on driver
//Ground to pin 6 to turn on motor.
//You can put extra 1 switch or industrial push button on off.For turning on off servo motor if you wish

//ground wire from arduino in to pin 14 and pin 5 of driver
//Pulse pin from arduino to pin 3 of driver
//Direction pin from arduino in to pin 4 of driver.
  
#include <AccelStepper.h>

int driverPUL = 8; // PUL- pin
int driverDIR = 9; // DIR- pin

int driverPUL2 = 10;    // PUL- pin
int driverDIR2 = 11;   // DIR- pin

// Define a stepper and the pins it will use
AccelStepper stepper(1, driverPUL, driverDIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper stepper2(1, driverPUL2, driverDIR2);


void setup()
{  
  // Change these to suit your stepper if you want
  stepper2.setMaxSpeed(4000);
  stepper2.setAcceleration(10000);
  stepper2.moveTo(8200);

  stepper.setMaxSpeed(2600);
  stepper.setAcceleration(170000);
  stepper.moveTo(5400);
}

void loop()
{
    // If at the end of travel go to the other end
    if (stepper.distanceToGo() == 0)
      stepper.moveTo(-stepper.currentPosition());

    stepper.run();

     if (stepper2.distanceToGo() == 0)
      stepper2.moveTo(-stepper2.currentPosition());

    stepper2.run();
}
