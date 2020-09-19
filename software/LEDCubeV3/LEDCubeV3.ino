#include <Adafruit_NeoPixel.h>

#define RGB_PIN D3
#define RGB_NUM 200

// Max 10 Layers.
#define LAYER_NUM 8


// Instance for RGB
Adafruit_NeoPixel strip = Adafruit_NeoPixel(RGB_NUM, RGB_PIN, NEO_GRB + NEO_KHZ800);

// RGB Number Mapping for max 10 layers (225 RGB).
uint8_t rgb_map[] = {
0,1,2,3,4,9,8,7,6,5,10,11,12,13,14,19,18,17,16,15,20,21,22,23,24,
49,48,47,46,45,40,41,42,43,44,39,38,37,36,35,30,31,32,33,34,29,28,27,26,25,
50,51,52,53,54,59,58,57,56,55,60,61,62,63,64,69,68,67,66,65,70,71,72,73,74,
99,98,97,96,95,90,91,92,93,94,89,88,87,86,85,80,81,82,83,84,79,78,77,76,75,
100,101,102,103,104,109,108,107,106,105,110,111,112,113,114,119,118,117,116,115,120,121,122,123,124,
149,148,147,146,145,140,141,142,143,144,139,138,137,136,135,130,131,132,133,134,129,128,127,126,125,
150,151,152,153,154,159,158,157,156,155,160,161,162,163,164,169,168,167,166,165,170,171,172,173,174,
199,198,197,196,195,190,191,192,193,194,189,188,187,186,185,180,181,182,183,184,179,178,177,176,175,
200,201,202,203,204,209,208,207,206,205,210,211,212,213,214,219,218,217,216,215,220,221,222,223,224,
249,248,247,246,245,240,241,242,243,244,239,238,237,236,235,230,231,232,233,234,229,228,227,226,225,
};

// The colours are a transition r - g - b - back to r. (Wheel Pos 0-255)
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowWipe(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< sizeof(rgb_map); i++) {
      strip.setPixelColor(standard(i), Wheel(((i * 256 / sizeof(rgb_map)) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  uint16_t i;

  for(i = 0; i < sizeof(rgb_map); i++) {
    strip.setPixelColor(standard(i), c);
    strip.show();
    delay(wait);
  }
}

void raining(uint32_t c, uint8_t wait, uint16_t timeout) {

  uint16_t t, i;
  // 0th is buffer layer (for clearing last layer).
  int16_t drop[11] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

  // Clear Grid.
  for(i = 0; i < sizeof(rgb_map); i++) {
    strip.setPixelColor(standard(i), 0);
  }

  for(t = 0; t < timeout; t++) {
    // Generate the next LED position.
    drop[10] = random(0,25);

    for(i = 0; i < (sizeof(drop)/sizeof(int16_t)); i++) { 
      if(i == 0) {
        // Remove the last bottom LED.
        strip.setPixelColor(standard(drop[i]), 0);
        drop[i] = -1;
      } else if(drop[i] != -1) {
        // Color in the current layer LED.
        strip.setPixelColor(standard(drop[i]+(i*25)), 0);
        strip.setPixelColor(standard(drop[i]+((i-1)*25)), c); 

        // Move LED
        drop[i-1] = drop[i];
      }
    }    
    strip.show();
    delay(wait);
  }
  
}


void layerX(uint32_t c, uint8_t x) {
  uint8_t y,z;
  
  for(y = 0; y < 5; y++) {
    for(z = 0; z < 10; z++) {
      strip.setPixelColor(rgb(x, y, z), c);
    }
  }
  strip.show();
}

void layerY(uint32_t c, uint8_t y) {
  uint8_t x,z;
  
  for(x = 0; x < 5; x++) {
    for(z = 0; z < 10; z++) {
      strip.setPixelColor(rgb(x, y, z), c);
    }
  }
  strip.show();
}

void layerZ(uint32_t c, uint8_t z) {
  uint8_t x,y;
  
  for(x = 0; x < 5; x++) {
    for(y = 0; y < 5; y++) {
      strip.setPixelColor(rgb(x, y, z), c);
    }
  }
  strip.show();
}




// Get the specific rgb_mapping from cartesian coordinates.
uint8_t rgb(uint8_t x, uint8_t y, uint8_t z) {
  x = (x >= 5) ? 4 : x;
  y = (x >= 5) ? 4 : y;
  z = (x >= 10) ? 9 : z;  

  return rgb_map[ x + 5*y + 25*z ];
}

// Physical mapping (Windy S shape).
uint8_t strand(uint8_t x, uint8_t y, uint8_t z) {
  x = (x >= 5) ? 4 : x;
  y = (x >= 5) ? 4 : y;
  z = (x >= 10) ? 9 : z;  

  return (x + 5*y + 25*z);
}


uint8_t standard(uint8_t i) {
  return rgb_map[i];
}

uint8_t standard_vertical(uint8_t i) {
  return rgb((i/50)%5, (i/10)%5, i%10);
}

uint8_t windy(uint8_t i) {
  return i;
}






void setup() {
  // put your setup code here, to run once:

  // Start Serial.
  Serial.begin(115200);

  // Start RGB.
  strip.begin();
  strip.setBrightness(50); // Default Brightness (0 - 255)
  strip.show(); // Initialize all pixels to 'off'

}

void loop() {
  // put your main code here, to run repeatedly:
  for (int x = 0; x < 5; x++) {
  int p = 5;
  for(int i = 0; i < p; i++) {
    layerX(strip.Color(255,0,255),i);
    layerX(strip.Color(0,0,0),(i-1)%p);
    delay(100);
  }
  for(int i = p-1; i >= 0; i--) {
    layerX(strip.Color(255,0,255),i);
    layerX(strip.Color(0,0,0),(i+1)%p);
    delay(100);
  }
    
  }

    for (int x = 0; x < 5; x++) {
  int p = 5;
  for(int i = 0; i < p; i++) {
    layerY(strip.Color(255,0,255),i);
    layerY(strip.Color(0,0,0),(i-1)%p);
    delay(100);
  }
  for(int i = p-1; i >= 0; i--) {
    layerY(strip.Color(255,0,255),i);
    layerY(strip.Color(0,0,0),(i+1)%p);
    delay(100);
  }
    
  }

    for (int x = 0; x < 5; x++) {
  int p = 8;
  for(int i = 0; i < p; i++) {
    layerZ(strip.Color(255,0,255),i);
    layerZ(strip.Color(0,0,0),(i-1)%p);
    delay(100);
  }
  for(int i = p-1; i >= 0; i--) {
    layerZ(strip.Color(255,0,255),i);
    layerZ(strip.Color(0,0,0),(i+1)%p);
    delay(100);
  }
    
  }


  


}
