/*
2018-10-29 cewood		amp-active
context: Arduino Nano clone
note: BOTH the sound module and the relay are active LOW
	    this will matter in line 44 digitalRead(detpin);
*/

// declare pins
int relaypin = 12;    	// digital output pin for the relay
int sensepin = A0;      // analog pin to read from
int directSense = 0;    // variable to put the sensepin reading into

// declare variables
int stayOn = 10;	      // seconds (loops) for relay (and amp) to stay on after sound is no longer detected
                        // set to 10 seconds for debugging. Normal 2-minute delay, set to 120
int howLong = 0;	      // this is the count of seconds (loops) so far for relay on, sound detect off
bool isSoundDetected = false;	// will be set true when sound is detected
bool relayOn = false;	  // will be set high when relay is active

void setup()
{
  Serial.begin(9600);            // setup serial monitor

  pinMode(relaypin, OUTPUT);	   // output to relay module
  pinMode(sensepin, INPUT);      // set the sensepin to read

  digitalWrite(relaypin, HIGH);  // HIGH turns off the relay when we start
}

void loop()
{
  directSense = 0;
  for (int i=0; i <= 200; i++){
    directSense += analogRead(sensepin);
  }
  isSoundDetected = (directSense > 5);  // If the sum of all analog samples is over 5, then sound is detected

  // these serial monitor outputs are for debugging, but they could be left in the sketch  
  Serial.print("isSoundDetected ");
  Serial.print(isSoundDetected);
  Serial.print("      relayOn ");
  Serial.print(relayOn);
  Serial.print("      howLong ");
  Serial.print(howLong);
  Serial.print("      analogReading ");
  Serial.print(directSense);
  Serial.println();
  
  if (isSoundDetected == true)
    {
    howLong = 0;				              // reset counter, wherever it was before
    if (relayOn == false)		      	  // could be "else"
      {
      digitalWrite(relaypin, LOW);	  // LOW turns on the relay
      relayOn = true;		            	// this sets the relay active flag
      }
    }
  
  if (isSoundDetected == false)
    {
    if (relayOn == true)
      {
      howLong = (howLong + 1);		    // increment the counter
      if (howLong >= stayOn)	    	  // has delay period elapsed?
        {
        digitalWrite(relaypin, HIGH);	// HIGH turns off the relay
        relayOn = false;		        	// reset flag
        howLong = 0;		            	// reset counter
        }
      }
    }
  delay(1000);				                // wait 1000 miliseconds and loop again
}
