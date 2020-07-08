#include <Joystick.h>

/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK, 6, 0,
  true, true, false, false, false, false,
  false, false, false, false, false);

  int lastButtonState[6] = {0,0,0,0,0,0};
  int buttonMap[6] = {13,12,11,10,9,8};

// the setup routine runs once when you press reset:
void setup() {
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  Joystick.setXAxisRange(192, 523);
  Joystick.setYAxisRange(580, 622);
  Joystick.begin();

  // Init Pins
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  pinMode(13, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);

}

// the loop routine runs over and over again forever:
void loop() {
  
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  int sensorValue2 = analogRead(A1);

  Joystick.setXAxis(sensorValue);
  Joystick.setYAxis(sensorValue2);
  
  // print out the value you read:
 // Serial.println(sensorValue);
  //Serial.println(sensorValue2);


   // Read pin values
  for (int index = 0; index < 6; index++)
  {
    int currentButtonState = !digitalRead(buttonMap[index]);
    if (currentButtonState != lastButtonState[index])
    {
      switch (index) {
       
        case 0: // Black Button 1
          Joystick.setButton(0, currentButtonState);
          break;
        case 1: // Black Button 2
          Joystick.setButton(1, currentButtonState);
          break;
        case 2: // Black Button 3
          Joystick.setButton(2, currentButtonState);
          break;
        case 3: // Black Button 4
          Joystick.setButton(3, currentButtonState);
          break;
        case 4: // Black Button 5
          Joystick.setButton(4, currentButtonState);
          break;
        case 5: // Black Button 6
          Joystick.setButton(5, currentButtonState);
          break;
        
      }
       Serial.println(index);
       Serial.println(currentButtonState);
       Serial.println(lastButtonState[index]);
       Serial.println("");
       
      lastButtonState[index] = currentButtonState;
    }
  }

  
  delay(1);        // delay in between reads for stability
}
