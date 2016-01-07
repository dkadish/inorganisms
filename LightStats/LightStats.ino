//
//    FILE: Average.ino
//  AUTHOR: Rob dot Tillaart at gmail dot com
// VERSION: 0.2
// PURPOSE: Sample sketch for statistic library Arduino
//

#define STAT_USE_STDEV 1
#define NEO_PIN 6
#define PHOTO_PIN 1
#define CONSEC_READS 10

#include "Statistic.h"
#include <Adafruit_NeoPixel.h>

Statistic lightStats;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, NEO_PIN, NEO_RGB);// + NEO_KHZ400);

void setup(void)
{
  lightStats.clear(); //explicitly start clean
}

void loop(void)
{
  // Read the new light levels and add it to the stats object.
  int lightLevel = readLight();
  lightStats.add(lightLevel);
  
  // Map light levels to the colours.
}

int readLight(){
  long light = 0;
  for(int i=0; i<CONSEC_READS; i++){
    light += analogRead(PHOTO_PIN);
    delay(1);
  }
  light = light/CONSEC_READS;

  return (int) light;
}

