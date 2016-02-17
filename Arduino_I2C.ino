#include <Wire.h>

// uses adafruit NeoPixels on pin 6 
#include <Adafruit_NeoPixel.h>
#define PIN 6
#define MaxPixels 24
Adafruit_NeoPixel strip = Adafruit_NeoPixel(MaxPixels, PIN, NEO_GRB + NEO_KHZ800);

int c;

void setup() {
  //setup wire library (i2c)
  Wire.begin(0);
  //setup neopixels
  strip.begin();
  strip.show();
  
  //setup serial for testing purposes
  Serial.begin(9600);
}
void loop() {
  Wire.onReceive(receiveEvent);
  
  //look for data sent to arduino and set it to a case depending on that data
  while(c > 0) {
    if(c == 6) {
    CylonBounce(0xff, 0, 0, 1, 10, 50);
    }
    if(c == 5) {
      Twinkle(0xff, 0, 0, 10, 50, false);
    }
    if(c == 4) {
      Fire(55,120,15);
    }
    if(c == 3) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
       strip.setPixelColor(i, 255, 0, 127);
       strip.show();
       Serial.print("c is now ");
       Serial.println(c);
    }
    }
    
    // case 1
  if (c == 1) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
       strip.setPixelColor(i, 255, 0, 0);
       strip.show();
       Serial.print("c is now ");
       Serial.println(c);
    }
  }
  
  //case 2
  if (c == 2) {
    Shooting(100
    );
    strip.show();
    Serial.print("c is now ");
    Serial.println(c);
  }
  }
  //default case - if nothing else matches
  for(uint16_t i=0; i<strip.numPixels(); i++) {
       strip.setPixelColor(i, 0, 0, 0);
       strip.show();
       Serial.print("c is now ");
       Serial.println(c);
  }
}

void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    int c = Wire.read();    // receive byte as an integer
    Serial.println(c);    // print the integer
  }
    c = Wire.read();
    
}
//2
void Shooting(uint8_t wait) {
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 127, 0, 0);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 127, 127, 127);        //turn every third pixel off
      }
    }
  }
//4
void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[MaxPixels];
  int cooldown;
  
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < MaxPixels; i++) {
    cooldown = random(0, ((Cooling * 10) / MaxPixels) + 2);
    
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= MaxPixels - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
    
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < MaxPixels; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  strip.show();
  delay(SpeedDelay);
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    strip.setPixelColor(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    strip.setPixelColor(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    strip.setPixelColor(Pixel, heatramp, 0, 0);
  }
}
//5
void Twinkle(byte red, byte green, byte blue, int Count, int SpeedDelay, boolean OnlyOne) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
       strip.setPixelColor(i, 0, 0, 0);
       strip.show();
  
  for (int i=0; i<Count; i++) {
     strip.setPixelColor(random(MaxPixels),red,green,blue);
     strip.show();
     delay(SpeedDelay);
     if(OnlyOne) { 
       for(uint16_t i=0; i<strip.numPixels(); i++) {
       strip.setPixelColor(i, 0, 0, 0);
       strip.show(); 
     }
   }
  
  delay(SpeedDelay);
}
  }
}

//6
void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i < MaxPixels-EyeSize-2; i++) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
       strip.setPixelColor(i, 0, 0, 0);
       strip.show(); 
     }
    strip.setPixelColor(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      strip.setPixelColor(i+j, red, green, blue); 
    }
    strip.setPixelColor(i+EyeSize+1, red/10, green/10, blue/10);
    strip.show();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = MaxPixels-EyeSize-2; i > 0; i--) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
       strip.setPixelColor(i, 0, 0, 0);
       strip.show(); 
     }
    strip.setPixelColor(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      strip.setPixelColor(i+j, red, green, blue); 
    }
    strip.setPixelColor(i+EyeSize+1, red/10, green/10, blue/10);
    strip.show();
    delay(SpeedDelay);
  }
  
  delay(ReturnDelay);
}
