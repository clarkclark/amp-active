/*
2018-10-29 cewood		amp-active
context: Arduino Nano clone
note: BOTH the sound module and the relay are active LOW
	    this will matter in line 44 digitalRead(detpin);
*/

// declare pins
int detpin = 2;	        // digital input pin for the sound module
int relaypin = 12;    	// digital output pin for the relay
int ledpinSD = 5;	      // digital output pin for sound detect flag LED
int ledpinRA = 9;	      // digital output pin for relay active flag LED

// declare variables
int stayOn = 10;	      // seconds (loops) for relay (and amp) to stay on after sound is no longer detected
                        // set to 10 seconds for debugging. Normal 2-minute delay, set to 120
int howLong = 0;	      // this is the count of seconds (loops) so far for relay on, sound detect off
bool soundDet = false;	// will be set high when sound is detected
bool relayOn = false;	  // will be set high when relay is active

void setup()
{
  Serial.begin(9600);            // setup serial monitor

  pinMode(detpin, INPUT);	       // input from sound module
  pinMode(relaypin, OUTPUT);	   // output to relay module
  pinMode(ledpinSD, OUTPUT);	   // LED to show soundDet flag
  pinMode(ledpinRA, OUTPUT);	   // LED to show relayOn flag

  digitalWrite(relaypin, HIGH);  // HIGH turns off the relay when we start
}

void loop()
{
// these serial monitor outputs are for debugging, but they could be left in the sketch  
  Serial.print("soundDet "); 
  Serial.print(soundDet);
  Serial.print("      relayOn "); 
  Serial.print(relayOn);
  Serial.print("      howLong "); 
  Serial.print(howLong);
  Serial.println();
  
soundDet = digitalRead(detpin);		  // check sound module
//
// This sketch was written (sketched?) before the audio module arrived.
// If the sound module is NOT low active (as advertised), remark the next line.
soundDet = !soundDet;
//
if (soundDet == true)
  {
  digitalWrite(ledpinSD, HIGH);		  // LED to show the status of sound detect flag
  howLong = 0;				              // reset counter, wherever it was before
  if (relayOn == true)
    {
    digitalWrite(ledpinRA, HIGH);  	// LED to show the status of relay active flag
    }
  if (relayOn == false)		      	  // could be "else"
    {
    digitalWrite(relaypin, LOW);	  // LOW turns on the relay
    relayOn = true;		            	// this sets the relay active flag
    digitalWrite(ledpinRA, HIGH); 	// LED to show the status of relay active flag
    }
  }

if (soundDet == false)
  {
  digitalWrite(ledpinSD, LOW);	  	// LED to show the status of sound detect flag
  if (relayOn == true)
    {
    digitalWrite(ledpinRA, HIGH); 	// LED to show the status of relay active flag
    howLong = (howLong + 1);		    // increment the counter
    if (howLong >= stayOn)	    	  // has delay period elapsed?
      {
      digitalWrite(relaypin, HIGH);	// HIGH turns off the relay
      relayOn = false;		        	// reset flag
      digitalWrite(ledpinRA, LOW);	// LED to show the status of relay active flag
      howLong = 0;		            	// reset counter
      }
    }
  }
  if (relayOn == false)		        	// could be "else"
    {
    digitalWrite(ledpinRA, LOW);	  // LED to show the status of relay active flag
    }
delay(1000);				                // wait 1000 miliseconds and loop again
}
