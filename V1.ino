#include <LedControl.h>
#include <Adafruit_NeoPixel.h>

#define PINPIXELS 6
#define NUMPIXELS 86

// Anzahl der Jahre (Punkte auf dem NeoPixel Streifen), die gezählt werden sollen
#define JAHRE 8

/*
      PIN Belegung

      10 - DIN
      11 - CLK
      12 - CS/LOAD
      6  - NeoPixel
*/

LedControl lc(12, 11, 10, 1);
Adafruit_NeoPixel pixels(NUMPIXELS, PINPIXELS, NEO_GRB + NEO_KHZ800);

int pos = NUMPIXELS / 2;
int anz = 0;

bool end = false;
bool animation = true;
int animationOffset = -8;
byte* animationData;
// TEXT: "September 2015"
byte text1[91] = {
B00000000,	B01100010,	B10010010,	B10010010,	B10010010,	B10001100,	B00000000,
B00000000,	B00011100,	B00101010,	B00101010,	B00011010,	B00000000,
B00000000,	B00111111,	B00100100,	B00100100,	B00011000,	B00000000,
B00000000,	B00100000,	B11111110,	B00100000,	B00000000,
B00000000,	B00011100,	B00101010,	B00101010,	B00011010,	B00000000,
B00000000,	B00111110,	B00100000,	B00011110,	B00100000,	B00011110,	B00000000,
B00000000,	B11111110,	B00100010,	B00100010,	B00011100,	B00000000,
B00000000,	B00011100,	B00101010,	B00101010,	B00011010,	B00000000,
B00000000,	B00111110,	B00100000,	B00100000,	B00010000,	B00000000,
B00000000,	B00000000,	B00000000,	B00000000,	B00000000,	B00000000,	B00000000,
B00000000,	B10001110,	B10010010,	B10010010,	B10010010,	B01100010,	B00000000,
B00000000,	B01111100,	B10000010,	B10000010,	B10000010,	B01111100,	B00000000,
B00000000,	B00100000,	B01000000,	B11111110,	B00000000,
B00000000,	B11110010,	B10010010,	B10010010,	B10010010,	B10001100,	B00000000
};
// TEXT: "Geschafft"
byte text2[9*6] = {
B00000000,	B00111000,	B01000100,	B10000010,	B10001010,	B01001100,	B00000000,
B00000000,	B00011100,	B00101010,	B00101010,	B00011010,	B00000000,
B00000000,	B00010010,	B00101010,	B00101010,	B00100100,	B00000000,
B00000000,	B00011100,	B00100010,	B00100010,	B00100010,	B00000000,
B00000000,	B11111110,	B00100000,	B00100000,	B00011110,	B00000000,
B00000000,	B00011100,	B00100010,	B00100010,	B00111110,	B00000000,
B00000000,	B00010000,	B11111111,	B10010000,	B01000000,	B00000000,
B00000000,	B00010000,	B11111111,	B10010000,	B01000000,	B00000000,
B00000000,	B00100000,	B11111110,	B00100000,	B00000000
};
// BUS PixelArt
byte bus[21] = {
  B00011000, B00011100, B00110100, B01010010, B10010101, B10010010, B11110100, B10010100, B10010100, B10010100, B11110010, B10010101, B10010010, B10010100, B11111100, B00001000, B00001000, B00110000, B01110000, B01100000, B00100000
};
int animationDataLength = 91;

void setup() {
  Serial.begin(9600);

  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  pixels.begin();
  pixels.setBrightness(100);

  Serial.begin(9600);

  animationData = text1;
}

void loop() {
  if (!end) {
    pixels.clear();
    if (!animation) pos++;
    pixels.setPixelColor(pos, pixels.Color(255, 255, 255));

    if (pos >= NUMPIXELS && !animation) pos -= NUMPIXELS;
    if (pos == (NUMPIXELS / 2 + (JAHRE/2) - anz) && !animation) {
      anz++;
    }
    for (int i = 0; i < anz; i++) {
      switch(i) {
        default:
        case 1:
        case 8:
          pixels.setPixelColor((NUMPIXELS / 2 + 4 - i), pixels.Color(255, 255, 255));
          break;
        case 2:
          pixels.setPixelColor((NUMPIXELS / 2 + 4 - i), pixels.Color(255, 0, 0));
          break;
        case 3:
          pixels.setPixelColor((NUMPIXELS / 2 + 4 - i), pixels.Color(255, 80, 0));
          break;
        case 4:
          pixels.setPixelColor((NUMPIXELS / 2 + 4 - i), pixels.Color(255, 255, 0));
          break;
        case 5:
          pixels.setPixelColor((NUMPIXELS / 2 + 4 - i), pixels.Color(0, 255, 80));
          break;
        case 6:
          pixels.setPixelColor((NUMPIXELS / 2 + 4 - i), pixels.Color(0, 140, 255));
          break;
        case 7:
          pixels.setPixelColor((NUMPIXELS / 2 + 4 - i), pixels.Color(140, 0, 255));
          break;
      }
    }
    if (anz > JAHRE) {
      animationData = text2;
      animationDataLength = 9*6;
      animationOffset = -8;
      animation = true;
      end = true;
      anz = -1;
    }
    pixels.show();
  
    animate();

    delay(50);
  } else if (!animation) {
    delay(2000);
    // Fill along the length of the strip in various colors...
    colorWipe(pixels.Color(255,   0,   0), 50); // Red
    colorWipe(pixels.Color(  0, 255,   0), 50); // Green
    colorWipe(pixels.Color(  0,   0, 255), 50); // Blue
  
    // Do a theater marquee effect in various colors...
    theaterChase(pixels.Color(127, 127, 127), 50); // White, half brightness
    theaterChase(pixels.Color(127,   0,   0), 50); // Red, half brightness
    theaterChase(pixels.Color(  0,   0, 127), 50); // Blue, half brightness
  
    rainbow(10);             // Flowing rainbow cycle along the whole strip
    theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
  } else {
    animate();
    delay(100);
  }
}

void animate() {
  animationOffset++;
  if (animationOffset > animationDataLength) {
    animationOffset = -8;
    animation = false;
    animationData = bus;
    animationDataLength = 21;
  }
  Serial.println(String(animationOffset) + " - " + String(animationDataLength));
  for (int i = 0; i < 8; i++) {
    if((animationOffset + i) < animationDataLength && (animationOffset + i) > 0)
      lc.setColumn(0, i, animationData[i + animationOffset]);
    else
      lc.setColumn(0, i, 0);
  }
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
    pixels.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    pixels.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      pixels.clear();         //   Set all pixels in RAM to 0 (off)
      for(int c=b; c<pixels.numPixels(); c += 3) {
        pixels.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      pixels.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    pixels.rainbow(firstPixelHue);
    pixels.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      pixels.clear();         //   Set all pixels in RAM to 0 (off)
      for(int c=b; c<pixels.numPixels(); c += 3) {
        int      hue   = firstPixelHue + c * 65536L / pixels.numPixels();
        uint32_t color = pixels.gamma32(pixels.ColorHSV(hue)); // hue -> RGB
        pixels.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      pixels.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

