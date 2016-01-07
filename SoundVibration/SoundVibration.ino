/****************************************
Example Sound Level Sketch for the 
Adafruit Microphone Amplifier
****************************************/
#include "Statistic.h"

#define CYCLE_TIME 250
#define MOTOR_PIN 0
#define MIC_PIN 2

Statistic limits, sound;
long startCycle;

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)

void setup()
{
  //Serial.begin(9600);
  limits.clear();
  limits.set_decay(0.0001);
  sound.clear();

  pinMode(MOTOR_PIN, OUTPUT);
   
  startCycle = millis();
}
 
 
void loop() 
{
  double volts = detectSound();
  limits.add(volts);
  sound.add(volts);

  if( millis() - startCycle > CYCLE_TIME ){
    if(limits.decay_fraction(sound.average()) > 0.5){
      digitalWrite(MOTOR_PIN, HIGH);
    } else {
      digitalWrite(MOTOR_PIN, LOW);
    }
    
    /*Serial.print("Reading: ");
    Serial.print(sound.average());
    Serial.print(" | Limits: ");
    Serial.print(limits.decay_maximum());
    Serial.print(", ");
    Serial.print(limits.decay_minimum());
    Serial.print(" | Fraction: ");
    Serial.print(limits.decay_fraction(sound.average()));*/
    
    sound.clear();
  }
  
  //Serial.println(volts);
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

