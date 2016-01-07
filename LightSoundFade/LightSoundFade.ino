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
#define CYCLE_TIME 500

Statistic limits, light;
unsigned int st  artCycle;

bool falling, rising;
int freq;

void setup() {
  pinMode(SPEAKER,OUTPUT);  // Set Trinket/Gemma Digital 0 to output
                            // to drive the piezo buzzer (important)

  limits.clear();
  limits.set_decay(1E-4);
  light.clear();

  startCycle = millis();

  falling = false;
  rising = false;
}
 
void loop() {
  int reading=analogRead(PHOTOCELL);  // Read Analog pin 1 for the changing
                             // voltage from the CdS divider
  limits.add(reading);
  light.add(reading);

  /*for( int i = 1000; i > 220; i-- ){
    beep(SPEAKER, i, 10);
  }*/
  
  if( millis() - startCycle >= CYCLE_TIME && !falling && !rising ) {
    if (limits.decay_fraction(light.average()) > 0.95) {
      falling = true;
      freq = 1000;
    } else if(limits.decay_fraction(light.average()) < 0.05) {
      rising = true;
      freq = 220;
    }

    light.clear();
    startCycle = millis();
  }

  if( rising ){
    if( freq > 1000 ){
      rising = false;
    } else {
      beep(SPEAKER, freq, 10);
      freq++;
    }
  } else if ( falling ){
    if( freq < 220 ){
      falling = false;
    } else {
      beep(SPEAKER, freq, 10);
      freq--;
    }
  }
                             
  /*int freq=220+(int)(reading*SCALE); // change the voltage read to a frequency
                             // you can change the values to scale
                             // your frequency range  
  beep(SPEAKER,freq,400);    // output the tone to digital pin 0
                             // you can change 400 to different times}
  delay(50);                 // delay a bit between notes  (also adjustable to taste)*/
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
