#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            A0

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS     365

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 20; // delay time
int j; //counter
int mid = NUMPIXELS / 2;
int g_gradient; //gradient counter to add to the green values
int b_gradient; //gradient counter to add to the blue values

//Color values for the whole strip between 0 and 255 -------- If the values are too high the Arduino pulls too much power
int R = 100;
int r = 100;

int G = 0;
int g = 100;
int B = 0;
int b = 100;
int buttonValue = 1;
int prevStepPadValue = 1;
int stepPadValue = 20;

//Flags for solar flare animation
bool flag1 = false;
bool flag2 = true; 

//Arrays filled with the initial gradient colors split into R, G and B arrays
int grad_red[(NUMPIXELS / 2) + 4];
int grad_green[(NUMPIXELS / 2) + 4];
int grad_blue[(NUMPIXELS / 2) + 4];


void setup() {
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {


  buttonValue = (buttonValue + analogRead(A4)) / 2;
  //Serial.println (buttonValue);
  prevStepPadValue = stepPadValue;
  stepPadValue = analogRead(A2);
  Serial.println (stepPadValue);

  if(stepPadValue < 5){
    flag1 = true;
  }
  else{
    flag1 = false;
  }

  if(stepPadValue > 50 && prevStepPadValue > 50){
    flag2 = true;
  }

  if (flag1 == true && flag2 == true) {
    flag2 = false;
    for (int i = 0; i < NUMPIXELS / 2; i++) {

      j = NUMPIXELS - 1 - i;

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Front to off
      pixels.setPixelColor(j, pixels.Color(0, 0, 0)); //Back to off
    }
    pixels.show();
    delay(delayval);

    // Top flashes on
    for (int i = 0; i < NUMPIXELS / 2; i++) {

      j = NUMPIXELS - 1 - i;

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      if (i >= 3 * NUMPIXELS / 8) {
        pixels.setPixelColor(i, pixels.Color(r, g / 2, b / 8)); // Front to off
        pixels.setPixelColor(j, pixels.Color(r, g / 2, b / 8)); //Back to off

      }
    }
    pixels.show();
    delay(75);

    //Top turns off all at once
    for (int i = 0; i < NUMPIXELS / 2; i++) {

      j = NUMPIXELS - 1 - i;

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Front to off
      pixels.setPixelColor(j, pixels.Color(0, 0, 0)); //Back to off

    }
    pixels.show();
    delay(75);

    //Runs down each side fast on
    for (int i = 0; i < NUMPIXELS / 4; i = i + 5) {

      j = NUMPIXELS - 1 - i;

      pixels.setPixelColor(i + mid, pixels.Color(r, g / 2, b / 8));
      pixels.setPixelColor(i + mid + 1, pixels.Color(r, g / 2, b / 8));
      pixels.setPixelColor(i + mid + 2, pixels.Color(r, g / 2, b / 8));
      pixels.setPixelColor(i + mid + 3, pixels.Color(r, g / 2, b / 8));
      pixels.setPixelColor(i + mid + 4, pixels.Color(r, g / 2, b / 8));

      pixels.setPixelColor(j - mid, pixels.Color(r, g / 2, b / 8));
      pixels.setPixelColor(j - mid - 1, pixels.Color(r, g / 2, b / 8));
      pixels.setPixelColor(j - mid - 2, pixels.Color(r, g / 2, b / 8));
      pixels.setPixelColor(j - mid - 3, pixels.Color(r, g / 2, b / 8));
      pixels.setPixelColor(j - mid - 4, pixels.Color(r, g / 2, b / 8));

      pixels.show();
      //delay(delayval);
    }

    //Runs down each side fast off
    for (int i = 0; i < NUMPIXELS / 2; i = i + 5) {

      j = NUMPIXELS - 1 - i;

      if (i < NUMPIXELS / 4) {
        pixels.setPixelColor(i + mid + NUMPIXELS / 4, pixels.Color(r, g / 2, b / 8));
        pixels.setPixelColor(i + mid + 1 + NUMPIXELS / 4, pixels.Color(r, g / 2, b / 8));
        pixels.setPixelColor(i + mid + 2 + NUMPIXELS / 4, pixels.Color(r, g / 2, b / 8));
        pixels.setPixelColor(i + mid + 3 + NUMPIXELS / 4, pixels.Color(r, g / 2, b / 8));
        pixels.setPixelColor(i + mid + 4 + NUMPIXELS / 4, pixels.Color(r, g / 2, b / 8));

        pixels.setPixelColor(j - mid - NUMPIXELS / 4, pixels.Color(r, g / 2, b / 8));
        pixels.setPixelColor(j - mid - 1 - NUMPIXELS / 4, pixels.Color(r, g / 2, b / 8));
        pixels.setPixelColor(j - mid - 2 - NUMPIXELS / 4, pixels.Color(r, g / 2, b / 8));
        pixels.setPixelColor(j - mid - 3 - NUMPIXELS / 4, pixels.Color(r, g / 2, b / 8));
        pixels.setPixelColor(j - mid - 4 - NUMPIXELS / 4, pixels.Color(r, g / 2, b / 8));

      }

      pixels.setPixelColor(i + mid, pixels.Color(0, 0, 0));
      pixels.setPixelColor(i + mid + 1, pixels.Color(0, 0, 0));
      pixels.setPixelColor(i + mid + 2, pixels.Color(0, 0, 0));
      pixels.setPixelColor(i + mid + 3, pixels.Color(0, 0, 0));
      pixels.setPixelColor(i + mid + 4, pixels.Color(0, 0, 0));

      pixels.setPixelColor(j - mid, pixels.Color(0, 0, 0));
      pixels.setPixelColor(j - mid - 1, pixels.Color(0, 0, 0));
      pixels.setPixelColor(j - mid - 2, pixels.Color(0, 0, 0));
      pixels.setPixelColor(j - mid - 3, pixels.Color(0, 0, 0));
      pixels.setPixelColor(j - mid - 4, pixels.Color(0, 0, 0));

      pixels.show();
      //delay(delayval);
    }

    //Pauses while everything is dark
    delay(10);

    //Slowly creeps back up to the top
    for (int i = 0; i < NUMPIXELS / 2; i += 4) {

      j = NUMPIXELS - 1 - i;

      pixels.setPixelColor(i, pixels.Color(r, g / 2, b / 8));
      pixels.setPixelColor(j, pixels.Color(r, g / 2, b / 8));
      pixels.show();
      //delay(delayval*10);
      pixels.setPixelColor(i + 3, pixels.Color(r, g / 2, b / 8));
      pixels.setPixelColor(j + 2, pixels.Color(r, g / 2, b / 8));
      pixels.show();
      //delay(delayval*10);
      pixels.setPixelColor(i + 1, pixels.Color(r, g / 2, b / 8));
      pixels.setPixelColor(j + 1, pixels.Color(r, g / 2, b / 8));
      pixels.show();
      //delay(delayval*10);
      pixels.setPixelColor(i + 2, pixels.Color(r, g / 2, b / 8));
      pixels.setPixelColor(j + 3, pixels.Color(r, g / 2, b / 8));
      pixels.show();
      //delay(delayval*10);
    }
    //Pauses while everything is light
    delay(10);

    //Fade off
    for (int i = 0; i < NUMPIXELS / 2; i += 5) {

      j = NUMPIXELS - 1 - i;

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Front to off
      pixels.setPixelColor(i + 1, pixels.Color(0, 0, 0)); // Front to off
      pixels.setPixelColor(i + 2, pixels.Color(0, 0, 0)); // Front to off
      pixels.setPixelColor(i + 3, pixels.Color(0, 0, 0)); // Front to off
      pixels.setPixelColor(i + 4, pixels.Color(0, 0, 0)); // Front to off

      pixels.setPixelColor(j, pixels.Color(0, 0, 0)); //Back to off
      pixels.setPixelColor(j - 1, pixels.Color(0, 0, 0)); //Back to off
      pixels.setPixelColor(j - 2, pixels.Color(0, 0, 0)); //Back to off
      pixels.setPixelColor(j - 3, pixels.Color(0, 0, 0)); //Back to off
      pixels.setPixelColor(j - 4, pixels.Color(0, 0, 0)); //Back to off

      pixels.show();
    }

    delay(delayval);

    //Quick flash on
    for (int i = 0; i < NUMPIXELS / 2; i++) {

      j = NUMPIXELS - 1 - i;

      if (i >= 3 * NUMPIXELS / 8) {
        pixels.setPixelColor(i, pixels.Color(r, g / 2, b / 8)); // Front to off
        pixels.setPixelColor(j, pixels.Color(r, g / 2, b / 8)); //Back to off

      }
    }
    pixels.show();

    delay(10);
    /*
                                                                            //Quick off to the end
      for(int i=0; i<NUMPIXELS/2; i++){

        j=NUMPIXELS-1-i;

        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(0,0,0)); // Front to off
        pixels.setPixelColor(j, pixels.Color(0,0,0)); //Back to off

        }
      pixels.show(); */
    delay(100);
  }
  else if (buttonValue < 20) {
    for (int i = 0; i < NUMPIXELS / 2 ; i++) {

      j = NUMPIXELS - 1 - i;

      if (i <= NUMPIXELS / 2) {
        pixels.setPixelColor(i, pixels.Color(0, 0, b)); //Red End
      }

      if (j >= NUMPIXELS / 2) {
        pixels.setPixelColor(j, pixels.Color(r, 0, 0)); //Blue End
      }

    }
    pixels.show(); // This sends the updated pixel color to the hardware.
  }




  else {
    for (int i = 0; i < (NUMPIXELS / 2) + 1; i++) { // Sets the gradient once

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255

      j = NUMPIXELS - 1 - i;
      g_gradient = map(i, 0, NUMPIXELS / 2, 0, R / 3);
      b_gradient = map(i, 0, NUMPIXELS / 2, 0, R / 3);

      pixels.setPixelColor(i + mid, pixels.Color(R, g_gradient, 0)); //Front
      pixels.setPixelColor(j - mid, pixels.Color(R, g_gradient, 0)); //Back

    }
    pixels.show(); // This sends the updated pixel color to the hardware.
    //delay(delayval); // Delay for a period of time (in milliseconds).
  }

}
