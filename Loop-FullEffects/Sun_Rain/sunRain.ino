/*\
|*| Author:
|*|   Juan Molina
|*|   Fiske Planetarium
|*|   jamolinaescalante@gmail.com
|*|   July-December 2019
|*|
|*| Some basic bits taken and remixed from:
|*|   1. Adafruit_NeoPixel Code
|*|   2. Tweaking4All LEDStrip Effects for NeoPixels
\*/
#include <Adafruit_NeoPixel.h>
#include <Keyboard.h>
#include <FastLED.h>
#include <math.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN A0
#define STRIPSIZE 360
#define DELAY 10
#define SENSOR A2 //sensor pin (pwm)
#define NUM_LEDS 360 //adjust this number if needed
#define MIRROR 180 // set to 1/2 of the LED count
#define DELAY 10
// Parameters to read sensor state.
int sensorState;
int sensorCounter = 0;
int lastSensorState = 0;
// Values for reference positions on the strip, gradients and color values.
int Mid = NUM_LEDS / 2; //center of the strip
int g_gradient; // gradient counter to add to the green values
int b_gradient; // gradient counter to add to the blue values
int b = 100; // Blue
int g = 100; // Green
int r = 100; // Red
int delayval = 10; // delay time
int j; // counter
uint32_t period = 10000L; // Millis
int randomVals[100]; // Array of values to pick from randomnly
int pickedIndex; // Param for the picked integer
/********FLAMES EFFECT Parameters*******/
// the data pin for the NeoPixels
#define DATA_PIN A0
// How many NeoPixels we will be using, charge accordingly
#define NUM_LEDS 360
//The variation in yellow color to create the fire effect, define the interval where the color can change.
#define MIN_VARIATION 0
#define MAX_VARIATION 25
//Value must be between 0 & 1.
//If you never want a LED to be completly off, put 0.1 to min
#define MIN_INTENSITY 0.1
#define MAX_INTENSITY 1.0
//Speed for variations, higher is slower
#define NOISE_SPEED_COLOR 3
#define NOISE_SPEED_INTENSITY 10
double n;
double ni;
const byte RED = 255;
CRGB leds[NUM_LEDS];
unsigned int lastTime = 0;
/********FLAMES EFFECT END *******/
// standard nepixel strip of 360 LEDs
Adafruit_NeoPixel * strip = new Adafruit_NeoPixel(STRIPSIZE, PIN, NEO_GRB + NEO_KHZ800);
// arduino logic initialization
void setup()
{
  Serial.begin(9600); // Initialize console
  Keyboard.begin(); // Initialize control over keyboard//
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  pinMode(SENSOR, INPUT); // Assign sensor to pin A2 as input
  strip->begin();
  colorWipe(strip->Color(5, 5, 5), 0);
  // sensorToSerial();
  strip->show(); // Initialize all pixels to 'off'
  // fill array with values
  for (int i=0; i<50; i++)
  {
   randomVals[i] = i;
  }
  for (int i=0; i<50; i++)
  {
   randomVals[50+i] = -i;
  }
}
/*
  DO YOU WANT NEW SUNSPOTS?
  Because this is how we get new sunspots:
  This needs to run EVERY FRAME or the sensors wont make new spots.
  sensorToSerial():
    - Sends the sensor state to the serial stream.
    - The processing script is listening for this...
      ...and makes spots accordingly.
*/
void sensorToSerial()
{
  sensorState = digitalRead(SENSOR);
  Serial.println(sensorState);
}

void serialFlush(){
  while (Serial.available() > 0){
    char t = Serial.read();
    }

  }



void loop()
{
  // Listen to incoming serial communication from processing script //
  sensorToSerial();
  pickedIndex = random(100);
  // Serial.println(pickedIndex);
  // Check for incoming serial data
  if (Serial.available() > 0)
  {
    int keyPress = Serial.read();
    //Serial.println("Pressed: " + keyPress);
    if (keyPress == 'a')
    {
       // Run solar flare effect first for burst.
       SolarFlare();
       // For # amount of time, run Sun Rain effect.
       for( uint32_t tStart = millis();  (millis()-tStart) < period;  )
       {
        sensorToSerial();
         SunRain(55,120,15,1,randomVals[pickedIndex]);
       }
    }
  }
  else
  {
   // Neutral state when nothing is sent to trigger
   // neutralState();
   flames();
  }
}
/********FLAMES EFFECT*******/
void flames()
{
  unsigned int time = millis();
  //Serial.println(1000/(time - lastTime));
  lastTime = time;
  for (int i = 0; i < NUM_LEDS; i++) {
    //adjust the mult and divide to change the global effect
    // will be added to advanced settings later
    n = inoise8(i*250 , (time+i)/NOISE_SPEED_COLOR);
    ni = inoise8(i*500 , (time+i)/NOISE_SPEED_INTENSITY);
    //You can change the easing function here
    //Used to avoid a linear effect and give a more natural curve.
    float v = QuadraticEaseInOut(n/255);
    float vi = QuadraticEaseInOut(ni/255);

    vi = (MAX_INTENSITY - MIN_INTENSITY) * v + MIN_INTENSITY;
    float red = vi *(RED*v);
    float yellow = vi *((MAX_VARIATION - MIN_VARIATION)*v + MIN_VARIATION);
    leds[i] = CRGB(red , yellow , 0);
  }
  sensorToSerial();
  FastLED.show();
}
float CubicEaseInOut(float p)
{
  if (p < 0.5)
  {
    return 4 * p * p * p;
  }
  else
  {
    float f = ((2 * p) - 2);
    return 0.5 * f * f * f + 1;
  }
}
float QuadraticEaseInOut(float p)
{
  if (p < 0.5)
  {
    return 2 * p * p;
  }
  else
  {
    return (-2 * p * p) + (4 * p) - 1;
  }
}
float SineEaseOut(float p)
{
  return sin(p * M_PI_2);
}
/********FLAMES EFFECT END*******/
// Global functions
void showStrip()
{
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  sensorToSerial();
  strip->show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  sensorToSerial();
  FastLED.show();
#endif
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip->numPixels(); i++) {
      strip->setPixelColor(i, c);
      sensorToSerial();
      strip->show();
      delay(wait);
  }
}
// END Global functions for all effects.
// Sun rain effect from the middle
void SunRain(int Cooling, int Sparking, int SpeedDelay, int Mirror, int randNum) {
  static byte heat[NUM_LEDS];
  int cooldown;
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }
  // Step 4.  Convert heat to LED colors
  // Serial.println(randNum);
  for( int j = 0; j < NUM_LEDS/2; j++) {
    setPixelHeatColor(j+MIRROR-randNum, heat[j] );
    setPixelHeatColor(-randNum+MIRROR-j, heat[j] );
    }
  sensorToSerial();
//  serialFlush();
  showStrip();
  delay(SpeedDelay);
}
void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*110);
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(Pixel, 255, 150, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}
void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip->setPixelColor(Pixel, strip->Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}
void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  sensorToSerial();
  showStrip();
}
// END: Sun rain effect from the middle
// The solar flare effect; From the center, going down to opposite side.
void SolarFlare()
{
  for (int i = 0; i < NUM_LEDS / 2; i++) {
      j = NUM_LEDS - 1 - i;
      // strip->Color takes RGB values, from 0,0,0 up to 255,255,255
      strip->setPixelColor(i, strip->Color(0, 0, 0)); // Front to off
      strip->setPixelColor(j, strip->Color(0, 0, 0)); //Back to off
    }
    sensorToSerial();
    strip->show();
    delay(delayval);
    // Top flashes on
    for (int i = 0; i < NUM_LEDS / 2; i++) {
      j = NUM_LEDS - 1 - i;
      // strip->Color takes RGB values, from 0,0,0 up to 255,255,255
      if (i >= 3 * NUM_LEDS / 8) {
        strip->setPixelColor(i, strip->Color(r, g / 2, b / 8)); // Front to off
        strip->setPixelColor(j, strip->Color(r, g / 2, b / 8)); //Back to off
      }
    }
    sensorToSerial();
    strip->show();
    delay(75);
    //Top turns off all at once
    for (int i = 0; i < NUM_LEDS / 2; i++) {
      j = NUM_LEDS - 1 - i;
      // strip->Color takes RGB values, from 0,0,0 up to 255,255,255
      strip->setPixelColor(i, strip->Color(0, 0, 0)); // Front to off
      strip->setPixelColor(j, strip->Color(0, 0, 0)); //Back to off
    }
    sensorToSerial();
    strip->show();
    delay(75);
    //Runs down each side fast on
    for (int i = 0; i < NUM_LEDS / 4; i = i + 5) {
      j = NUM_LEDS - 1 - i;
      strip->setPixelColor(i + Mid, strip->Color(r, g / 2, b / 8));
      strip->setPixelColor(i + Mid + 1, strip->Color(r, g / 2, b / 8));
      strip->setPixelColor(i + Mid + 2, strip->Color(r, g / 2, b / 8));
      strip->setPixelColor(i + Mid + 3, strip->Color(r, g / 2, b / 8));
      strip->setPixelColor(i + Mid + 4, strip->Color(r, g / 2, b / 8));
      strip->setPixelColor(j - Mid, strip->Color(r, g / 2, b / 8));
      strip->setPixelColor(j - Mid - 1, strip->Color(r, g / 2, b / 8));
      strip->setPixelColor(j - Mid - 2, strip->Color(r, g / 2, b / 8));
      strip->setPixelColor(j - Mid - 3, strip->Color(r, g / 2, b / 8));
      strip->setPixelColor(j - Mid - 4, strip->Color(r, g / 2, b / 8));
      sensorToSerial();
      strip->show();
      //delay(delayval);
    }
    //Runs down each side fast off
    for (int i = 0; i < NUM_LEDS / 2; i = i + 5) {
      j = NUM_LEDS - 1 - i;
      if (i < NUM_LEDS / 4) {
        strip->setPixelColor(i + Mid + NUM_LEDS / 4, strip->Color(r, g / 2, b / 8));
        strip->setPixelColor(i + Mid + 1 + NUM_LEDS / 4, strip->Color(r, g / 2, b / 8));
        strip->setPixelColor(i + Mid + 2 + NUM_LEDS / 4, strip->Color(r, g / 2, b / 8));
        strip->setPixelColor(i + Mid + 3 + NUM_LEDS / 4, strip->Color(r, g / 2, b / 8));
        strip->setPixelColor(i + Mid + 4 + NUM_LEDS / 4, strip->Color(r, g / 2, b / 8));
        strip->setPixelColor(j - Mid - NUM_LEDS / 4, strip->Color(r, g / 2, b / 8));
        strip->setPixelColor(j - Mid - 1 - NUM_LEDS / 4, strip->Color(r, g / 2, b / 8));
        strip->setPixelColor(j - Mid - 2 - NUM_LEDS / 4, strip->Color(r, g / 2, b / 8));
        strip->setPixelColor(j - Mid - 3 - NUM_LEDS / 4, strip->Color(r, g / 2, b / 8));
        strip->setPixelColor(j - Mid - 4 - NUM_LEDS / 4, strip->Color(r, g / 2, b / 8));
      }
      strip->setPixelColor(i + Mid, strip->Color(0, 0, 0));
      strip->setPixelColor(i + Mid + 1, strip->Color(0, 0, 0));
      strip->setPixelColor(i + Mid + 2, strip->Color(0, 0, 0));
      strip->setPixelColor(i + Mid + 3, strip->Color(0, 0, 0));
      strip->setPixelColor(i + Mid + 4, strip->Color(0, 0, 0));
      strip->setPixelColor(j - Mid, strip->Color(0, 0, 0));
      strip->setPixelColor(j - Mid - 1, strip->Color(0, 0, 0));
      strip->setPixelColor(j - Mid - 2, strip->Color(0, 0, 0));
      strip->setPixelColor(j - Mid - 3, strip->Color(0, 0, 0));
      strip->setPixelColor(j - Mid - 4, strip->Color(0, 0, 0));
      sensorToSerial();
      strip->show();
      //delay(delayval);
    }
    //Pauses while everything is dark
    delay(10);
    //Slowly creeps back up to the top
    for (int i = 0; i < NUM_LEDS / 2; i += 4) {
      j = NUM_LEDS - 1 - i;
      strip->setPixelColor(i, strip->Color(r, g / 2, b / 8));
      strip->setPixelColor(j, strip->Color(r, g / 2, b / 8));
      sensorToSerial();
      strip->show();
      //delay(delayval*10);
      strip->setPixelColor(i + 3, strip->Color(r, g / 2, b / 8));
      strip->setPixelColor(j + 2, strip->Color(r, g / 2, b / 8));
      sensorToSerial();
      strip->show();
      //delay(delayval*10);
      strip->setPixelColor(i + 1, strip->Color(r, g / 2, b / 8));
      strip->setPixelColor(j + 1, strip->Color(r, g / 2, b / 8));
      sensorToSerial();
      strip->show();
      //delay(delayval*10);
      strip->setPixelColor(i + 2, strip->Color(r, g / 2, b / 8));
      strip->setPixelColor(j + 3, strip->Color(r, g / 2, b / 8));
      sensorToSerial();
      strip->show();
      //delay(delayval*10);
    }
    //Pauses while everything is light
    delay(10);
    //Fade off
    for (int i = 0; i < NUM_LEDS / 2; i += 5) {
      j = NUM_LEDS - 1 - i;
      // strip->Color takes RGB values, from 0,0,0 up to 255,255,255
      strip->setPixelColor(i, strip->Color(0, 0, 0)); // Front to off
      strip->setPixelColor(i + 1, strip->Color(0, 0, 0)); // Front to off
      strip->setPixelColor(i + 2, strip->Color(0, 0, 0)); // Front to off
      strip->setPixelColor(i + 3, strip->Color(0, 0, 0)); // Front to off
      strip->setPixelColor(i + 4, strip->Color(0, 0, 0)); // Front to off
      strip->setPixelColor(j, strip->Color(0, 0, 0)); //Back to off
      strip->setPixelColor(j - 1, strip->Color(0, 0, 0)); //Back to off
      strip->setPixelColor(j - 2, strip->Color(0, 0, 0)); //Back to off
      strip->setPixelColor(j - 3, strip->Color(0, 0, 0)); //Back to off
      strip->setPixelColor(j - 4, strip->Color(0, 0, 0)); //Back to off
      sensorToSerial();
      strip->show();
    }
    delay(delayval);
    //Quick flash on
    for (int i = 0; i < NUM_LEDS / 2; i++) {
      j = NUM_LEDS - 1 - i;
      if (i >= 3 * NUM_LEDS / 8) {
        strip->setPixelColor(i, strip->Color(r, g / 2, b / 8)); // Front to off
        strip->setPixelColor(j, strip->Color(r, g / 2, b / 8)); //Back to off
      }
    }
    sensorToSerial();
    strip->show();
    delay(10);
    delay(100);
}
// END: The solar flare effect; From the center, going down to opposite sides
// Neutral state; Yellow to red patterns
void neutralState()
{
    // Sets the gradient once
    for (int i = 0; i < (NUM_LEDS / 2) + 1; i++)
    {
    // strip->Color takes RGB values, from 0,0,0 up to 255,255,255
    j = NUM_LEDS - 1 - i;
    g_gradient = map(i, 0, NUM_LEDS / 2, 0, r / 3);
    b_gradient = map(i, 0, NUM_LEDS / 2, 0, r / 3);
    strip->setPixelColor(i + Mid, strip->Color(r, g_gradient, 0)); //Front
    strip->setPixelColor(j - Mid, strip->Color(r, g_gradient, 0)); //Back
  }
  sensorToSerial();
  strip->show(); // This sends the updated pixel color to the hardware.
  //delay(delayval); // Delay for a period of time (in milliseconds).
}
// END: Neutral state; Yellow to red patterns
