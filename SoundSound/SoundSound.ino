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
#define MIC_PIN 1    // CdS photocell on GPIO #2 (A1)
#define SCALE     2.0  // You can change this to change the tone scale
#define CYCLE_TIME 250

Statistic limits, light;
unsigned int startCycle;

const int sampleWindow = 10; // Sample window width in mS (50 mS = 20Hz)

void setup() {
  pinMode(SPEAKER,OUTPUT);  // Set Trinket/Gemma Digital 0 to output
                            // to drive the piezo buzzer (important)

  limits.clear();
  limits.set_decay(1E-4);
  light.clear();

  for( int i=0; i<25; i++ ){
    double reading = detectSound();
  
    limits.add(reading);
    light.add(reading);
  }

  startCycle = millis();
}
 
void loop() {
  
  double reading = detectSound();
  
  limits.add(reading);
  light.add(reading);
  
  if( millis() - startCycle >= CYCLE_TIME) {
    if (limits.decay_fraction(light.average()) > 0.95) {
      for(int i=220; i < 500; i++){
        beep(SPEAKER, i, 10);
      }
      for(int i=500; i > 300; i--){
        beep(SPEAKER, i, 10);
      }
    } else if(limits.decay_fraction(light.average()) < 0.05) {
      for(int j=0; j < 3; j++){
        for(int i=600; i < 700; i++){
          beep(SPEAKER, i, 10);
        }
        for(int i=700; i > 600; i--){
          beep(SPEAKER, i, 10);
        }
      }
    }

    light.clear();
    startCycle = millis();
  }
}

double detectSound(){
  unsigned long startMillis = millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  
  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    unsigned int sample = analogRead(MIC_PIN);
    if (sample < 1024)  // toss out spurious readings
    {
       if (sample > signalMax)
       {
          signalMax = sample;  // save just the max levels
       }
       else if (sample < signalMin)
       {
          signalMin = sample;  // save just the min levels
       }
    }
    delayMicroseconds(1);
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  double volts = (peakToPeak * 3.3) / 1024;  // convert to volts
  return volts;
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
