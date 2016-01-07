// For the Pro Trinket
#include "Statistic.h"

#include <Adafruit_NeoPixel.h>
#include <PixelPattern.h>

#define PIXEL 6
#define PHOTOCELL A1
#define CYCLE_TIME 1000
#define NO_SERIAL

Statistic limits, light;
long startCycle;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIXEL);
PixelPattern pattern = PixelPattern();

HSV from, to;
RGB rgb;

void setup() {
  #ifndef NO_SERIAL
  Serial.begin(9600);
  delay(1000);
  #endif

  limits.clear();
  limits.set_decay(0.0001);
  light.clear();

  startCycle = millis();

  pixels.begin();
  pixels.setBrightness(100);

  from = to = {0, 0, 0};

  rgb.r = rgb.g = rgb.b = 0;

  pattern.fade({62,255,255},{120,255,255},10000);
}

void loop() {
  // Do a single reading, but do it a bunch of times to smooth it out a little.
  int reading = analogRead(1);
  //Serial.println(reading);

  limits.add(reading);
  light.add(reading);

  // If there is lots of light, make the LED cool colours
  // If there's very little light, make the LED warm colours
  // If it's in the middle, make it nothing.
  /*if ( millis() - startCycle >= CYCLE_TIME ) {
    from.h = to.h;
    from.s = to.s;
    from.v = to.v;

    if (limits.decay_fraction(light.average()) > 0.75) {
      to = {random(85) + 85, 255, 255};
    } else if (limits.decay_fraction(light.average()) < 0.25) {
      to = {random(85) + 212, 255, 255};
    } else {
      to = {from.h, 0, 0};
    }

    pattern.fade(from, to, CYCLE_TIME);

    #ifndef NO_SERIAL
    Serial.print("Reading: ");
    Serial.print(reading);
    Serial.print(" | Limits: ");
    Serial.print(limits.decay_maximum());
    Serial.print(", ");
    Serial.print(limits.decay_minimum());
    Serial.print(" | Fraction: ");
    Serial.print(limits.decay_fraction(light.average()));
    Serial.print(" | From/To: ");
    Serial.print(from.h);
    Serial.print(", ");
    Serial.print(from.s);
    Serial.print(", ");
    Serial.print(from.v);
    Serial.print(" | ");
    Serial.print(to.h);
    Serial.print(", ");
    Serial.print(to.s);
    Serial.print(", ");
    Serial.print(to.v);
    Serial.print(" | Cycle: ");
    Serial.print(millis() - startCycle);
      Serial.print(", ");
      Serial.print(CYCLE_TIME);
    Serial.println();
    #endif

    light.clear();
    startCycle = millis();
  }*/

  rgb = pattern.loop();

  pixels.setPixelColor(0, rgb.r, rgb.g, rgb.b);
  pixels.show();
}
