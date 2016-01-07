#include "Statistic.h"
#include <Adafruit_NeoPixel.h>

#define PIXEL 0
#define PHOTOCELL 1
#define CYCLE_TIME 50

struct RGB {
  unsigned char r, g, b;
};

RGB hsv2rgb(unsigned char h);

Statistic limits, light;
unsigned int startCycle;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIXEL);

unsigned char h;
RGB rgb;

void setup() {
  delay(1000);

  limits.clear();
  limits.set_decay(1E-4);
  light.clear();

  startCycle = millis();

  pixels.begin();
  pixels.setBrightness(100);

  h = 0;

  rgb = {0, 0, 0};
  
}

void loop() {
  /*for( unsigned char i = 0; i < 255; i+=10;){
    rgb = hsv2rgb(i);
    pixels.setPixelColor(0, rgb.r, rgb.g, rgb.b);
    pixels.show();
    delay(100);
  }
  delay(1000);*/
  
  // Do a single reading, but do it a bunch of times to smooth it out a little.
  int reading = analogRead(PHOTOCELL);

  limits.add(reading);
  light.add(reading);

  // If there is lots of light, make the LED cool colours
  // If there's very little light, make the LED warm colours
  // If it's in the middle, make it nothing.
  if ( millis() - startCycle >= CYCLE_TIME ) {

    if (limits.decay_fraction(light.average()) < 0.5) {
      rgb = hsv2rgb(limits.decay_fraction_int(light.average())*2);
    } else {
      rgb = {0, 0, 0};
    }

    light.clear();
    startCycle = millis();
  }

  pixels.setPixelColor(0, rgb.r, rgb.g, rgb.b);
  pixels.show();
}
