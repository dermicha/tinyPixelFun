#include <FastLED.h>
#include "SoftwareSerial.h"

// How many leds in your strip?
#define NUM_LEDS 20

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define STRIPE_PIN 0
#define P1_PIN 1
#define P2_PIN 2

// Define the array of leds
CRGB leds[NUM_LEDS];
CRGB playerStateColors[4];
CRGB pongColor = CRGB::Blue;

//Player 1
int p1 = 0;
int p1Button = 0;
int p1State = 0;

//Player 2
int p2 = NUM_LEDS-1;
int p2Button = 0;
int p2State = 0;

int cIdx = p1 + int(NUM_LEDS / 2);
int offset = 1;

int currentDelay = 150;
int deltaDelay = 5;
int minDelay = 20;

void resetPlayerStates() {
  p1State = 0;
  p2State = 0;
}

int incPlayerState(int ps) {
  if(ps < 3)
    return ps + 1;
  else 
    return ps;
}

void setup() { 
      pinMode(P1_PIN,INPUT);
      pinMode(P2_PIN,INPUT);

      playerStateColors[0] = CRGB::Green;
      playerStateColors[1] = CRGB::Yellow;
      playerStateColors[2] = CRGB::Orange;
      playerStateColors[3] = CRGB::Red;

      resetPlayerStates();
      
      FastLED.addLeds<WS2812B, STRIPE_PIN, GRB>(leds, NUM_LEDS);
}

void loop() { 
  
  p1Button = digitalRead(P1_PIN);
  p2Button = digitalRead(P2_PIN);
  
  leds[cIdx] = CRGB::Black;
  
  if(p1 >= (cIdx + offset) || p2 <= (cIdx + offset) ) {
    offset = offset * -1;
    if (offset > 0 && (currentDelay - deltaDelay) >= minDelay) {
      currentDelay = currentDelay - deltaDelay;
    }
  }
  
  cIdx = cIdx + offset;
  leds[cIdx] = pongColor;

  if( (p1Button > 0 && cIdx != (p1+1)) || (p1Button == 0 && cIdx <= (p1 + 1)) ){
      p1State = incPlayerState(p1State);
  }

  if( (p2Button > 0 && cIdx != (p2-1)) || (p2Button == 0 && cIdx >= (p2-1)) ){
    p2State = incPlayerState(p2State);
  }
    
  //Status Player 1
  leds[p1] = playerStateColors[p1State];
  //Status Player 2
  leds[p2] = playerStateColors[p2State];
  
  FastLED.show();
  
  delay(currentDelay);

  if( (p1Button > 0 && p1State == 3) && (p2Button > 0 && p2State == 3))
    resetPlayerStates();  
}
