


// Defin pins


int reverseSwitch = 2;  // Push button for reverse

int driverPUL = 8;    // PUL- pin

int driverDIR = 9;    // DIR- pin

int spd = A0;     // Potentiometer

int steps = 0;

// Variables


int pd = 700;       // Pulse Delay period

boolean setdir = LOW; // Set Direction


// Interrupt Handler

void revmotor (){

 setdir = !setdir;

}





void setup() {



 pinMode (driverPUL, OUTPUT);

 pinMode (driverDIR, OUTPUT);

 attachInterrupt(digitalPinToInterrupt(reverseSwitch),revmotor, FALLING);

 
}




void loop() {

 
   // pd = map((analogRead(spd)),0,1023,2000,50);
while (steps < 1200) {

   digitalWrite(driverDIR,setdir);

   digitalWrite(driverPUL,HIGH);

   delayMicroseconds(pd);

   digitalWrite(driverPUL,LOW);

   delayMicroseconds(pd);

   steps++;
}

steps = 0;
revmotor();
}
