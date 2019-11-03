//MN Maker
//11/3/19


#include <Adafruit_NeoPixel.h>

#define LED_PIN    4

boolean slowFadeState = 0;

const int buttonPin = 2;     // the number of the pushbutton pin

// variables will change:
int buttonState = 0;
int buttonStep=0; 

// How many NeoPixels are attached to the ATINY85
#define LED_COUNT 20

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  //Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {

  buttonState = digitalRead(buttonPin);
  //Serial.print(buttonStep);
  if (buttonState == HIGH){
    buttonStep += 1;
    delay(250);
  }
  //Serial.print(buttonStep);
  switch(buttonStep){
    case -1:
      stripClear();
      break;
    case 0:
      stripClear();
      break;
    case 1:
      stripClear();
      break;
    case 2:
      colorWipe(strip.Color(255,   0,   0), 50); // Red
      break;
    case 3:
      colorWipe(strip.Color(0,   255,   0), 50); // Green
      break;
    case 4:
      colorWipe(strip.Color(0,   0,   255), 50); // Blue
      break;
    case 5:
      colorWipe(strip.Color(0,   255,   255), 50); // Neon
      break;
    case 6:
      colorWipe(strip.Color(127,   0,   255), 50); // Purple
      break;
    case 7:
      colorWipe(strip.Color(255,   128,   0), 50); // Orange
      break;
    case 8:
      theaterChase(strip.Color(127, 127, 127), 50); // White, half brightness
      break;
    case 9:
      theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
      break;
    case 10:
      theaterChase(strip.Color(  0,   0, 127), 50); // Blue, half brightness
      break;
    case 11:
      rainbow(1);             // Flowing rainbow cycle along the whole strip
      break;
    case 12:
      theaterChaseRainbow(5); // Rainbow-enhanced theaterChase variant
      break;
    case 13:
      colorFade();
      break;
    case 14:
      buttonStep=-1; //Reset
      break;
    default:
      break;
  }
}


// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.

void colorFade(){
   int picker = random(0,5);
   if (slowFadeState == 1){
      for(int k = 0; k<100; k++){
        switch(picker){
          case 0: colorWipe(strip.Color(0,k,0), 0);
            break;
          case 1: colorWipe(strip.Color(k,0,0), 0);
            break;
          case 2: colorWipe(strip.Color(0,0,k), 0);
            break;
          case 3: colorWipe(strip.Color(0,k,k), 0);
            break;
          case 4: colorWipe(strip.Color(k,k,0), 0);
            break;
          case 5: colorWipe(strip.Color(k,k,k), 0);
            break;
        }
        //strip.show();
        delay(5);
      }
      slowFadeState = 0;
   }
   else if (slowFadeState == 0){
      for(int k = 100; k>0; k--){
        switch(picker){
          case 0: colorWipe(strip.Color(0,k,0), 0);
            break;
          case 1: colorWipe(strip.Color(k,0,0), 0);
            break;
          case 2: colorWipe(strip.Color(0,0,k), 0);
            break;
          case 3: colorWipe(strip.Color(0,k,k), 0);
            break;
          case 4: colorWipe(strip.Color(k,k,0), 0);
            break;
          case 5: colorWipe(strip.Color(k,k,k), 0);
            break;
        }
        //strip.show();
        delay(5);
      }
      slowFadeState = 1;
   }
}




void stripClear(){
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, 0);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(5);                           //  Pause for a moment
  }
}


void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
