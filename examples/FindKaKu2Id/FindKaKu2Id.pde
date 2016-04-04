#include <RemoteReceiver.h>

/*
* Demo for RF remote switch receiver for KaKu2 remotes (learning remotes).
* For details, see RemoteReceiver.h!
*
* Code provided by Sander Chandon Janssens
*
* This sketch shows the received signals on the serial port.
* Connect the receiver to digital pin 2.
*/

/*

Sample output:

Code: 1180391824, period duration: 321us.

Remote id: 18443622
Command: on
Group: 0
Device id: 0
Command: sendSignal(18443622,0,0,true)﻿
*/



void setup() {
  Serial.begin(57600);
  
  //Initialize receiver on interrupt 0 (= digital pin 2), calls the callback "showCode"
  //after 3 identical codes have been received in a row. (thus, keep the button pressed
  //for a moment)
  //
  //See the interrupt-parameter of attachInterrupt for possible values (and pins)
  //to connect the receiver.
  RemoteReceiver::init(2, 0, 3, showCode, 0);
}

void loop() {
}

//Callback function is called only when a valid code is received.
void showCode(unsigned long receivedCode, unsigned int period) {
  //Note: interrupts are disabled. You can re-enable them if needed.
  
  //Print the received code.
  Serial.print("Code: ");
  Serial.print(receivedCode);
  Serial.print(", period duration: ");
  Serial.print(period);
  Serial.println("us.");
  Serial.println("");
  Serial.print("Remote id: ");
  Serial.println(receivedCode >> 6);
  if ((receivedCode & 16) == 16){
    Serial.println("Command: on");
  }
  else{
    Serial.println("Command: off");
  }
  Serial.print("Group: ");

  Serial.println((receivedCode & 32)/32);
  Serial.print("Device id: ");
  Serial.println(receivedCode & 15);
  Serial.print("Command: sendSignal(");
  Serial.print(receivedCode >> 6);
  Serial.print(",");
  Serial.print((receivedCode & 32)/32);
  Serial.print(",");
  Serial.print(receivedCode & 15);
  Serial.print(",");
  if ((receivedCode & 16) == 16){
    Serial.print("true");
  }
  else{
    Serial.print("false");
  }
  Serial.println(")");
  Serial.println("");
  
}
