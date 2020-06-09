
int OUT_LED_1 = 13;
int speedval = 200;
// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(OUT_LED_1, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {

   while(Serial.available() > 0 ){
    String str = Serial.readString();
    String sval = str.substring(0);
    if(str.substring(0) == "send\n"){
      Serial.println("identified");} 
    else{
      Serial.println("unknown " + str.substring(0));
    }
    speedval = sval.toInt();
  }
  
  digitalWrite(OUT_LED_1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(speedval);                       // wait for a second
  digitalWrite(OUT_LED_1, LOW);    // turn the LED off by making the voltage LOW
  delay(speedval);                       // wait for a second
}
