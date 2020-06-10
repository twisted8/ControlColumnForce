// -*- mode: C++ -*-
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

int speedval = 400.3453;

// the setup function runs once when you press reset or power the board
void setup() {
  

  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(5200);
  stepper.setAcceleration(5000);
//stepper.moveTo(-1000);
  Serial.begin(115200); 
}

// the loop function runs over and over again forever
void loop() {




 //  while(Serial.available() > 0 ){
 if (Serial.available() > 0){
    String str = Serial.readString();
    String sval = str.substring(0);

    speedval = sval.toInt();

     while (stepper.distanceToGo() != 0) {
      stepper.run();
    }
    

   
  }

}
