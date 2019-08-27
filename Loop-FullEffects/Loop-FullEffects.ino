#include <Keyboard.h>
#include <math.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN A0 //LED strip pin (pwm)
#define SENSOR A2 //sensor pin (pwm)
#define NUM_LEDS 360 //adjust this number if needed
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// Random variables
long LEDrandomNumber;
long randomTailNumber;
long randomSpeed;

// Parameters to read sensor state
int sensorState;
int sensorCounter = 0;
int lastSensorState = 0;

// Variables that can communicate with sensor values
int REDbrightness;
int brightnessCounter;
int minBrightness = 20;

// Values for reference positions on the strip, gradients and color values. Color values for the whole strip between 0 and 255 -------- If the values are too high the Arduino pulls too much power
int Mid = NUM_LEDS / 2; //center of the strip
int delayval = 10; // delay time
int j; // counter
int g_gradient; // gradient counter to add to the green values
int b_gradient; // gradient counter to add to the blue values
int R = 100; // Red
int r = 100;
int G = 10; // Green
int Gn = 0;
int g = 100;
int B = 0; // Blue
int b = 100;
int buttonValue = 1;
int prevStepPadValue = 1;
int stepPadValue = 20;

//Arrays filled with the initial gradient colors split into R, G and B arrays
int grad_red[(NUM_LEDS / 2) + 4];
int grad_green[(NUM_LEDS / 2) + 4];
int grad_blue[(NUM_LEDS / 2) + 4];

boolean sensorBlocked = false;


void setup()
{
  Serial.begin(9600); // Initialize console
  pinMode(SENSOR, INPUT); // Assign sensor to pin A2 as input
  strip.begin(); // Wake-up LED strip
  colorWipe(strip.Color(5, 5, 5), 0);
  strip.show(); // Initialize all pixels to 'off'
  Keyboard.begin(); // Initialize control over keyboard//
}

void loop()
{
  // Example: If instance from processing is for effect 1: Call effectOneListener(). Same for calls to other effects or any other listener and so on

  // Check for incoming serial data
  if (Serial.available() > 0)
  {
    int keyPress = Serial.read();
    Serial.println("Pressed: " + keyPress);

    switch (keyPress)
    {
      case 'a':
        effectOneListener();
        break;
      case 'b':
        effectTwoListener();
        break;
      case 'c':
        effectThreeListener();
        break;
      case 'd':
        effectFourListener();
        break;
    }
  }
  else
  {
    effectFiveListener();
  }

  sensorState = digitalRead(SENSOR);
    Serial.println(sensorState);

//  if (sensorState == '1' && sensorBlocked == false){
//    Serial.println(sensorState);
//    sensorBlocked = true;
//  }
//  else if(sensorState == 0 && sensorBlocked == true){
//    sensorBlocked == false;
//  }


}

// Global functions for all effects.
void showStrip()
{
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
#endif
}

void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
// END Global functions for all effects.


// Effect 1: Two flares crossing from one side to another side.
void effectOneListener()
{
  LEDrandomNumber = random(5, 10);
  randomTailNumber = random(10, 30);
  randomSpeed = random(1, 1);
  plasmaRain_1(0xff, 0, 0, LEDrandomNumber, randomTailNumber, true, randomSpeed);
  Serial.println("Effect one started");
}

void plasmaRain_1(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay)
{
  setAll_1(200, 5, 0);

  for (int i = Mid - 3; i > 0; i--) {
    //for (int i = 0; i < NUM_LEDS; i++) {
    // fade brightness all LEDs one step
    for (int j = 0; j < Mid + 5; j++) {
      if ( (!meteorRandomDecay) || (random(10) > 5) ) {
        fadeToBlack_1(j, meteorTrailDecay );
        fadeToBlack_1(NUM_LEDS - j, meteorTrailDecay );
      }
    }

    // draw tail
    for (int j = 0; j < meteorSize; j++) {
      if ( ( i - j < Mid) || (i - j >= 0) ) {
        setPixel_1(i - j, red, blue, green);
        setPixel_1(NUM_LEDS - i, red, blue, green);
      }
    }

    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack_1(int ledNo, byte fadeValue)
{
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  uint32_t oldColor;
  uint8_t r, g, b;
  int value;

  oldColor = strip.getPixelColor(ledNo);
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);

  r = (r <= 10) ? 0 : (int) r - (r * fadeValue / 256);
  g = (g <= 10) ? 0 : (int) g - (g * fadeValue / 256);
  b = (b <= 10) ? 0 : (int) b - (b * fadeValue / 256);

  strip.setPixelColor(ledNo, 20, 4, 0); //overall strip have dim red- as a default

  //  strip.setPixelColor(ledNo, r, g, b); //you can experiment this line as well
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[ledNo].fadeToBlackBy( fadeValue );
#endif
}

void setPixel_1(int Pixel, byte red, byte green, byte blue)
{
  //dripping flare's color control begins
#ifdef ADAFRUIT_NEOPIXEL_H
  sensorState = digitalRead(SENSOR);
  Serial.write(sensorState);
  //  Serial.println(sensorState);

  if (sensorState != lastSensorState) {
    if (sensorState == HIGH) {
      sensorCounter++;
      Serial.println("on");
      Serial.print ("number of sensor triggers: " );
      Serial.println(sensorCounter);
    } else if (sensorState == LOW) {
      Serial.println("off");
    }
    delay(50);
  }
  lastSensorState = sensorState;

  if (sensorCounter % 3 == 0) {
    REDbrightness = 100;
    strip.setPixelColor(Pixel, strip.Color(REDbrightness, 30, 0));
  }
  else if (sensorCounter % 5 == 0) {
    REDbrightness = 180;
    strip.setPixelColor(Pixel, strip.Color(REDbrightness, 20, 10));
  }
  else if (sensorCounter % 7 == 0) {
    REDbrightness = 255;
    strip.setPixelColor(Pixel, strip.Color(REDbrightness, 0, 0));
  }
  else {
    minBrightness = 50;
    strip.setPixelColor(Pixel, strip.Color(minBrightness, 5, 0));
  }

  /*
     if you want to control the brightness by using an analog sensor, following lines would be helpful (map)

     REDbrightness = map(sensorValue, 0, 1023, 20, 255);
     //map(sensorValue, minimum input sensor value, maximum input sensor value, minimum LEd brightness, maximum LED brightness);
     strip.setPixelColor(Pixel, strip.Color(REDbrightness, 0, 0));
  */

#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
#endif
}

void setAll_1(byte red, byte green, byte blue)
{
  for (int i = 0; i < NUM_LEDS; i++ ) {
    setPixel_1(i, red, green, blue);
  }
  showStrip();
}
// END Effect 1: Two flares crossing from one side to another side.




// Effect 2: Two flares begin from the center of a loop to make random tail and fade-speed brightness is controlled by sensor values.
void effectTwoListener()
{
  LEDrandomNumber = random(5, 10);
  randomTailNumber = random(10, 30);
  randomSpeed = random(1, 1);
  plasmaRain_2(0xff, 0, 0, LEDrandomNumber, randomTailNumber, true, randomSpeed);
}

void plasmaRain_2(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay)
{
  setAll_2(125, 114, 0);

  for (int i = 0; i < (NUM_LEDS + NUM_LEDS) / 3 ; i++) {
    //or try this ->  for (int i = 0; i < NUM_LEDS + NUM_LEDS / 3 ; i++) {
    // fade brightness all LEDs one step
    for (int j = 0; j < NUM_LEDS; j++) {
      if ( (!meteorRandomDecay) || (random(10) > 5) ) {
        fadeToBlack_2(j, meteorTrailDecay );
        fadeToBlack_2(NUM_LEDS - i, meteorTrailDecay );

      }
    }

    // draw flare
    for (int j = 0; j < meteorSize; j++) {
      if ( ( i - j < NUM_LEDS) && (i - j >= 0) ) {
        setPixel_2(i - j, red, green, blue);
        setPixel_2(NUM_LEDS - i, red, green, blue);
      }
    }

    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack_2(int ledNo, byte fadeValue)
{
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  uint32_t oldColor;
  uint8_t r, g, b;
  int value;

  oldColor = strip.getPixelColor(ledNo);
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);

  r = (r <= 10) ? 0 : (int) r - (r * fadeValue / 256);
  g = (g <= 10) ? 0 : (int) g - (g * fadeValue / 256);
  b = (b <= 10) ? 0 : (int) b - (b * fadeValue / 256);

  strip.setPixelColor(ledNo, 10, 4, 0);//yellow default back-lit
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[ledNo].fadeToBlackBy( fadeValue );
#endif
}

void setPixel_2(int Pixel, byte red, byte green, byte blue) {
#ifdef ADAFRUIT_NEOPIXEL_H

  sensorState = digitalRead(SENSOR);
  //  Serial.println(sensorState);

  if (sensorState != lastSensorState) {
    if (sensorState == HIGH) {
      sensorCounter++;
      Serial.println("on");
      Serial.print ("number of sensor triggers: " );
      Serial.println(sensorCounter);
    } else if (sensorState == LOW) {
      Serial.println("off");
    }
    delay(50);
  }
  lastSensorState = sensorState;

  if (sensorCounter % 3 == 0) {
    REDbrightness = 100;
    strip.setPixelColor(Pixel, strip.Color(REDbrightness, 30, 0));
  }
  else if (sensorCounter % 5 == 0) {
    REDbrightness = 180;
    strip.setPixelColor(Pixel, strip.Color(20, REDbrightness, 10));
  }
  else if (sensorCounter % 7 == 0) {
    REDbrightness = 255;
    strip.setPixelColor(Pixel, strip.Color(REDbrightness, 0, 0));
  }
  else {
    minBrightness = 50;
    strip.setPixelColor(Pixel, strip.Color(minBrightness, 5, 0));
  }
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
#endif
}

void setAll_2(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    setPixel_2(i, red, green, blue);
  }
  showStrip();
}
// END Effect 2: Two flares begin from the center of a loop to make random tail and fade-speed brightness is controlled by sensor values.



// Effect 3: One flare from left to right or viceversa
void effectThreeListener()
{
  LEDrandomNumber = random(5, 10);
  randomTailNumber = random(10, 30);
  randomSpeed = random(1, 5);
  plasmaRain_3(0xff, 0, 0, LEDrandomNumber, randomTailNumber, true, randomSpeed);
}

void plasmaRain_3(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay)
{
  setAll_3(0, 0, 0);

  for (int i = 0; i < NUM_LEDS*1.1; i++) {
    // fade brightness all LEDs one step
    for (int j = 0; j < NUM_LEDS*1.1; j++) {
      if ( (!meteorRandomDecay) || (random(10) > 5) ) {
        fadeToBlack_3(j, meteorTrailDecay );
        fadeToBlack_3(NUM_LEDS - j, meteorTrailDecay );

      }
    }

    // draw flare
    for (int j = 0; j < meteorSize; j++) {
      if ( ( i - j < NUM_LEDS) && (i - j >= 0) ) {
        setPixel_3(i - j, red, green, blue);
      }
    }

    showStrip();
    delay(SpeedDelay);
  }
}


void fadeToBlack_3(int ledNo, byte fadeValue)
{
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  uint32_t oldColor;
  uint8_t r, g, b;
  int value;

  oldColor = strip.getPixelColor(ledNo);
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);

  r = (r <= 10) ? 0 : (int) r - (r * fadeValue / 256);
  g = (g <= 10) ? 0 : (int) g - (g * fadeValue / 256);
  b = (b <= 10) ? 0 : (int) b - (b * fadeValue / 256);

  strip.setPixelColor(ledNo, 5, 0, 0);
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[ledNo].fadeToBlackBy( fadeValue );
#endif
}

void setPixel_3(int Pixel, byte red, byte green, byte blue)
{
#ifdef ADAFRUIT_NEOPIXEL_H

  sensorState = digitalRead(SENSOR);
  //  Serial.println(sensorState);

  if (sensorState != lastSensorState) {
    if (sensorState == HIGH) {
      sensorCounter++;
      Serial.println("on");
      Serial.print ("number of sensor triggers: " );
      Serial.println(sensorCounter);
    } else if (sensorState == LOW) {
      Serial.println("off");
    }
    delay(50);
  }
  lastSensorState = sensorState;

  if (sensorCounter % 3 == 0) {
    REDbrightness = 100;
    strip.setPixelColor(Pixel, strip.Color(REDbrightness, 30, 0));
  }
  else if (sensorCounter % 5 == 0) {
    REDbrightness = 180;
    strip.setPixelColor(Pixel, strip.Color(20, REDbrightness,10));
  }
  else if (sensorCounter % 7 == 0) {
    REDbrightness = 255;
    strip.setPixelColor(Pixel, strip.Color(REDbrightness, 0, 0));
  }
  else {
    minBrightness = 50;
    strip.setPixelColor(Pixel, strip.Color(minBrightness, 5, 0));
  }

  // NeoPixel
 // strip.setPixelColor(Pixel, strip.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
#endif
}

void setAll_3(byte red, byte green, byte blue)
{
  for (int i = 0; i < NUM_LEDS; i++ ) {
    setPixel_3(i, red, green, blue);
  }
  showStrip();
}
// END Effect 3: One flare from left to right or viceversa



// Effect 4: The solar flare effect; From the center, going down to opposite side and then each bottom makes new light move slower up to make a spark the top
void effectFourListener()
{
  for (int i = 0; i < NUM_LEDS / 2; i++) {

      j = NUM_LEDS - 1 - i;

      // strip.Color takes RGB values, from 0,0,0 up to 255,255,255
      strip.setPixelColor(i, strip.Color(0, 0, 0)); // Front to off
      strip.setPixelColor(j, strip.Color(0, 0, 0)); //Back to off
    }
    strip.show();
    delay(delayval);

    // Top flashes on
    for (int i = 0; i < NUM_LEDS / 2; i++) {

      j = NUM_LEDS - 1 - i;

      // strip.Color takes RGB values, from 0,0,0 up to 255,255,255
      if (i >= 3 * NUM_LEDS / 8) {
        strip.setPixelColor(i, strip.Color(r, g / 2, b / 8)); // Front to off
        strip.setPixelColor(j, strip.Color(r, g / 2, b / 8)); //Back to off

      }
    }
    strip.show();
    delay(75);

    //Top turns off all at once
    for (int i = 0; i < NUM_LEDS / 2; i++) {

      j = NUM_LEDS - 1 - i;

      // strip.Color takes RGB values, from 0,0,0 up to 255,255,255
      strip.setPixelColor(i, strip.Color(0, 0, 0)); // Front to off
      strip.setPixelColor(j, strip.Color(0, 0, 0)); //Back to off

    }
    strip.show();
    delay(75);

    //Runs down each side fast on
    for (int i = 0; i < NUM_LEDS / 4; i = i + 5) {

      j = NUM_LEDS - 1 - i;

      strip.setPixelColor(i + Mid, strip.Color(r, g / 2, b / 8));
      strip.setPixelColor(i + Mid + 1, strip.Color(r, g / 2, b / 8));
      strip.setPixelColor(i + Mid + 2, strip.Color(r, g / 2, b / 8));
      strip.setPixelColor(i + Mid + 3, strip.Color(r, g / 2, b / 8));
      strip.setPixelColor(i + Mid + 4, strip.Color(r, g / 2, b / 8));

      strip.setPixelColor(j - Mid, strip.Color(r, g / 2, b / 8));
      strip.setPixelColor(j - Mid - 1, strip.Color(r, g / 2, b / 8));
      strip.setPixelColor(j - Mid - 2, strip.Color(r, g / 2, b / 8));
      strip.setPixelColor(j - Mid - 3, strip.Color(r, g / 2, b / 8));
      strip.setPixelColor(j - Mid - 4, strip.Color(r, g / 2, b / 8));

      strip.show();
      //delay(delayval);
    }

    //Runs down each side fast off
    for (int i = 0; i < NUM_LEDS / 2; i = i + 5) {

      j = NUM_LEDS - 1 - i;

      if (i < NUM_LEDS / 4) {
        strip.setPixelColor(i + Mid + NUM_LEDS / 4, strip.Color(r, g / 2, b / 8));
        strip.setPixelColor(i + Mid + 1 + NUM_LEDS / 4, strip.Color(r, g / 2, b / 8));
        strip.setPixelColor(i + Mid + 2 + NUM_LEDS / 4, strip.Color(r, g / 2, b / 8));
        strip.setPixelColor(i + Mid + 3 + NUM_LEDS / 4, strip.Color(r, g / 2, b / 8));
        strip.setPixelColor(i + Mid + 4 + NUM_LEDS / 4, strip.Color(r, g / 2, b / 8));

        strip.setPixelColor(j - Mid - NUM_LEDS / 4, strip.Color(r, g / 2, b / 8));
        strip.setPixelColor(j - Mid - 1 - NUM_LEDS / 4, strip.Color(r, g / 2, b / 8));
        strip.setPixelColor(j - Mid - 2 - NUM_LEDS / 4, strip.Color(r, g / 2, b / 8));
        strip.setPixelColor(j - Mid - 3 - NUM_LEDS / 4, strip.Color(r, g / 2, b / 8));
        strip.setPixelColor(j - Mid - 4 - NUM_LEDS / 4, strip.Color(r, g / 2, b / 8));

      }

      strip.setPixelColor(i + Mid, strip.Color(0, 0, 0));
      strip.setPixelColor(i + Mid + 1, strip.Color(0, 0, 0));
      strip.setPixelColor(i + Mid + 2, strip.Color(0, 0, 0));
      strip.setPixelColor(i + Mid + 3, strip.Color(0, 0, 0));
      strip.setPixelColor(i + Mid + 4, strip.Color(0, 0, 0));

      strip.setPixelColor(j - Mid, strip.Color(0, 0, 0));
      strip.setPixelColor(j - Mid - 1, strip.Color(0, 0, 0));
      strip.setPixelColor(j - Mid - 2, strip.Color(0, 0, 0));
      strip.setPixelColor(j - Mid - 3, strip.Color(0, 0, 0));
      strip.setPixelColor(j - Mid - 4, strip.Color(0, 0, 0));

      strip.show();
      //delay(delayval);
    }

    //Pauses while everything is dark
    delay(10);

    //Slowly creeps back up to the top
    for (int i = 0; i < NUM_LEDS / 2; i += 4) {

      j = NUM_LEDS - 1 - i;

      strip.setPixelColor(i, strip.Color(r, g / 2, b / 8));
      strip.setPixelColor(j, strip.Color(r, g / 2, b / 8));
      strip.show();
      //delay(delayval*10);
      strip.setPixelColor(i + 3, strip.Color(r, g / 2, b / 8));
      strip.setPixelColor(j + 2, strip.Color(r, g / 2, b / 8));
      strip.show();
      //delay(delayval*10);
      strip.setPixelColor(i + 1, strip.Color(r, g / 2, b / 8));
      strip.setPixelColor(j + 1, strip.Color(r, g / 2, b / 8));
      strip.show();
      //delay(delayval*10);
      strip.setPixelColor(i + 2, strip.Color(r, g / 2, b / 8));
      strip.setPixelColor(j + 3, strip.Color(r, g / 2, b / 8));
      strip.show();
      //delay(delayval*10);
    }
    //Pauses while everything is light
    delay(10);

    //Fade off
    for (int i = 0; i < NUM_LEDS / 2; i += 5) {

      j = NUM_LEDS - 1 - i;

      // strip.Color takes RGB values, from 0,0,0 up to 255,255,255
      strip.setPixelColor(i, strip.Color(0, 0, 0)); // Front to off
      strip.setPixelColor(i + 1, strip.Color(0, 0, 0)); // Front to off
      strip.setPixelColor(i + 2, strip.Color(0, 0, 0)); // Front to off
      strip.setPixelColor(i + 3, strip.Color(0, 0, 0)); // Front to off
      strip.setPixelColor(i + 4, strip.Color(0, 0, 0)); // Front to off

      strip.setPixelColor(j, strip.Color(0, 0, 0)); //Back to off
      strip.setPixelColor(j - 1, strip.Color(0, 0, 0)); //Back to off
      strip.setPixelColor(j - 2, strip.Color(0, 0, 0)); //Back to off
      strip.setPixelColor(j - 3, strip.Color(0, 0, 0)); //Back to off
      strip.setPixelColor(j - 4, strip.Color(0, 0, 0)); //Back to off

      strip.show();
    }

    delay(delayval);

    //Quick flash on
    for (int i = 0; i < NUM_LEDS / 2; i++) {

      j = NUM_LEDS - 1 - i;

      if (i >= 3 * NUM_LEDS / 8) {
        strip.setPixelColor(i, strip.Color(r, g / 2, b / 8)); // Front to off
        strip.setPixelColor(j, strip.Color(r, g / 2, b / 8)); //Back to off

      }
    }
    strip.show();

    delay(10);
    /*
                                                                            //Quick off to the end
      for(int i=0; i<NUM_LEDS/2; i++){

        j=NUM_LEDS-1-i;

        // strip.Color takes RGB values, from 0,0,0 up to 255,255,255
        strip.setPixelColor(i, strip.Color(0,0,0)); // Front to off
        strip.setPixelColor(j, strip.Color(0,0,0)); //Back to off

        }
      strip.show(); */
    delay(100);
}
// END Effect 4: The solar flare effect; From the center, going down to opposite side and then each bottom makes new light move slower up to make a spark the top


// Effect 5: Neutral state; Yellow to red patterns
void effectFiveListener()
{
    // Sets the gradient once
    for (int i = 0; i < (NUM_LEDS / 2) + 1; i++)
    {

    // strip.Color takes RGB values, from 0,0,0 up to 255,255,255

    j = NUM_LEDS - 1 - i;
    g_gradient = map(i, 0, NUM_LEDS / 2, 0, R / 3);
    b_gradient = map(i, 0, NUM_LEDS / 2, 0, R / 3);

    strip.setPixelColor(i + Mid, strip.Color(R, g_gradient, 0)); //Front
    strip.setPixelColor(j - Mid, strip.Color(R, g_gradient, 0)); //Back

  }
  strip.show(); // This sends the updated pixel color to the hardware.
  //delay(delayval); // Delay for a period of time (in milliseconds).
}
