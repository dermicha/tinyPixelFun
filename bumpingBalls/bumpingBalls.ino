#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define NUM_PIXELS          20
#define DATA_PIN          0
#define BRIGHTNESS        150
#define GRAVITY           -7                 // Downward (negative) acceleration of gravity in m/s^2
#define h0                1                  // Starting height, in meters, of the ball (strip length)
#define NUM_BALLS         2                  // Number of bouncing balls you want (recommend < 7, but 20 is fun in its own way)

float h[NUM_BALLS] ;                         // An array of heights
float vImpact0 = sqrt( -2 * GRAVITY * h0 );  // Impact velocity of the ball when it hits the ground if "dropped" from the top of the strip
float vImpact[NUM_BALLS] ;                   // As time goes on the impact velocity will change, so make an array to store those values
float tCycle[NUM_BALLS] ;                    // The time since the last time the ball struck the ground
int   pos[NUM_BALLS] ;                       // The integer position of the dot on the strip (LED index)
long  tLast[NUM_BALLS] ;                     // The clock time of the last ground strike
float COR[NUM_BALLS] ;                       // Coefficient of Restitution (bounce damping)


Adafruit_NeoPixel pixels(NUM_PIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  clock_prescale_set(clock_div_1);
  
  pixels.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear();
  pixels.setBrightness(BRIGHTNESS);
  pixels.show();            // Turn OFF all pixels ASAP
  
  for (int i = 0 ; i < NUM_BALLS ; i++) {    // Initialize variables
    tLast[i] = millis();
    h[i] = h0;
    pos[i] = 0;                              // Balls start on the ground
    vImpact[i] = vImpact0;                   // And "pop" up at vImpact0
    tCycle[i] = 0;
    COR[i] = 0.90 - float(i)/pow(NUM_BALLS,2); 
  }
}

void loop() {

  for (int i = 0 ; i < NUM_BALLS ; i++) {
    tCycle[i] =  millis() - tLast[i] ;     // Calculate the time since the last time the ball was on the ground

    // A little kinematics equation calculates positon as a function of time, acceleration (gravity) and intial velocity
    h[i] = 0.5 * GRAVITY * pow( tCycle[i]/1000 , 2.0 ) + vImpact[i] * tCycle[i]/1000;

    if ( h[i] < 0 ) {                      
      h[i] = 0;                            // If the ball crossed the threshold of the "ground," put it back on the ground
      vImpact[i] = COR[i] * vImpact[i] ;   // and recalculate its new upward velocity as it's old velocity * COR
      tLast[i] = millis();

      if ( vImpact[i] < 0.01 ) vImpact[i] = vImpact0;  // If the ball is barely moving, "pop" it back up at vImpact0
    }
    pos[i] = round( h[i] * (NUM_PIXELS - 1) / h0);       // Map "h" to a "pos" integer index position on the LED strip
  }

  //Choose color of LEDs, then the "pos" LED on
  for (int i = 0 ; i < NUM_BALLS ; i++) 
    pixels.setPixelColor(pos[i], pixels.Color( uint8_t ((pos[i] * 10) % 255) , uint8_t((i * 70) % 255) , 0));
  
  pixels.show();
  
  // clear current pixel before next round
  for (int i = 0 ; i < NUM_BALLS ; i++) 
    pixels.setPixelColor(pos[i], pixels.Color(0,0,0));
}
