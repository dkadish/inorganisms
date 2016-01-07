/* Adafruit Trinket/Gemma Example: Simple Theramin
 
Read the voltage from a Cadmium Sulfide (CdS) photocell voltage divider
and output a corresponding tone to a piezo buzzer
 
Trinket Wiring: Photocell voltage divider center wire to GPIO #2
(analog 1) and output tone to GPIO #0 (digital 0)
 
Gemma Wiring: Voltage divider center wire to Gemma D2/A1, output
tone to Gemma D1.
 
Note: The Arduino tone library does not work for the ATTiny85 on the
Trinket and Gemma.  The beep function below is similar.  The beep code
is adapted from Dr. Leah Buechley at
http://web.media.mit.edu/~leah/LilyPad/07_sound_code.html
 
September 18, 2013
*/
#include "Statistic.h"
 
#define SPEAKER   0    // Speaker on GPIO #0
#define PHOTOCELL 1    // CdS photocell on GPIO #2 (A1)
#define SCALE     2.0  // You can change this to change the tone scale

#define CYCLE_TIME 50

Statistic limits, light;
long startCycle;
bool play;

void setup() {
  pinMode(SPEAKER,OUTPUT);  // Set Trinket/Gemma Digital 0 to output
                            // to drive the piezo buzzer (important)
  limits.clear();
  light.clear();
  
  startCycle = millis();
  play = false;
}
 
void loop() {
  int reading=analogRead(PHOTOCELL);  // Read Analog pin 1 for the changing
                             // voltage from the CdS divider
  int freq=220;//+(int)(reading*SCALE); // change the voltage read to a frequency
                             // you can change the values to scale
                             // your frequency range

  limits.add(freq);
  light.add(freq);

  //if( play ){
    beep(SPEAKER,freq,1);    // output the tone to digital pin 0
                             // you can change 400 to different times}
  //}
  
  if ( millis() - startCycle >= CYCLE_TIME ) {
    if( limits.decay_fraction(light.average()) > 0.5 ){
      play = true;
    } else {
      play = false;
    }
    
    startCycle = millis();
    light.clear();
  }
}
 
// the sound producing function
void beep (unsigned char speakerPin, int frequencyInHertz, long timeInMilliseconds)
{   // http://web.media.mit.edu/~leah/LilyPad/07_sound_code.html
          int x;   
          long delayAmount = (long)(1000000/frequencyInHertz);
          long loopTime = (long)((timeInMilliseconds*1000)/(delayAmount*2));
          for (x=0;x<loopTime;x++)   
          {  
              digitalWrite(speakerPin,HIGH);
              delayMicroseconds(delayAmount);
              digitalWrite(speakerPin,LOW);
              delayMicroseconds(delayAmount);
          }  
}
