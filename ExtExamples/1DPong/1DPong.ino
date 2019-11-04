
#include <FastLED.h>

#define NUM_LEDS1 40
#define NUM_LEDS2 15
#define NUM_LEDS3 15

CRGB leds1[NUM_LEDS1];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];

const byte N0[12] = {0, 9, 10, 1, 11, 2, 12, 3, 13, 4, 5, 14};
const byte N1[5] = {10, 11, 12, 13, 14};
const byte N2[11] = {0, 9, 10, 11, 2, 7, 12, 3, 4, 5, 14};
const byte N3[11] = {0, 9, 10, 11, 2, 7, 12, 13, 4, 5, 14};
const byte N4[9] = {0, 10, 1, 11, 2, 7, 12, 13, 14};
const byte N5[11] = {0, 9, 10, 1, 2, 7, 12, 13, 4, 5, 14};
const byte N6[12] = {0, 9, 10, 1, 2, 7, 12, 3, 13, 4, 5, 14};
const byte N7[7] = {0, 9, 10, 11, 12, 13, 14};
const byte N8[13] = {0, 9, 10, 1, 11, 2, 7, 12, 3, 13, 4, 5, 14};
const byte N9[12] = {0, 9, 10, 1, 11, 2, 7, 12, 13, 4, 5, 14};
byte gamestate = 0;
byte start;
byte redscore = 0;
byte greenscore = 0;
char i;
char m;
int x;
char ledposition;
bool redpush = 0;
bool greenpush = 0;
bool greenwin = 0;
bool redwin = 0;
bool dooncered = 0;
bool dooncegreen = 0;
unsigned long starttime;
unsigned long currenttime;
unsigned int waittime = 60;

void RED() {
  if (dooncered == 0) {
    switch (gamestate) {
      case 0:
        gamestate = 1;
        start = 0;
        dooncered = 1;
        break;
      case 1:
        if (start == 1) {
          redpush = 1;
        }
        dooncered = 1;
        break;
      case 2:
        redpush = 1;
        dooncered = 1;
        break;
    }
  }
}

void GREEN() {
  if (dooncegreen == 0) {
    switch (gamestate) {
      case 0:
        gamestate = 1;
        start = 1;
        dooncegreen = 1;
      case 1:
        if (start == 0) {
          greenpush = 1;
        }
        dooncegreen = 1;
        break;
      case 3:
        greenpush = 1;
        dooncegreen = 1;
        break;
    }
  }
}

void setup() {
  FastLED.addLeds<WS2812B, 8, GRB>(leds1, NUM_LEDS1);
  FastLED.addLeds<WS2812B, 9, GRB>(leds2, NUM_LEDS2);
  FastLED.addLeds<WS2812B, 10, GRB>(leds3, NUM_LEDS3);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(0, RED, FALLING);
  attachInterrupt(1, GREEN, FALLING);
}

void loop() {
  switch (gamestate) {
    case 0:
      leds1[0].setRGB(255, 0, 0);
      leds1[1].setRGB(255, 0, 0);
      leds1[2].setRGB(255, 0, 0);
      leds1[37].setRGB(255, 0, 0);
      leds1[38].setRGB(255, 0, 0);
      leds1[39].setRGB(255, 0, 0);
      FastLED.show();
      break;
    case 1:
      showscore();
      if (start == 0) {
        redtogreenstart();
      }
      else {
        greentoredstart();
      }
      break;
    case 2:
      if (ledposition > 34) {
        dooncered = 0;
        greentored();
      }
      else {
        redwin = 1;
        gamestate = 4;
      }
      break;
    case 3:
      if (ledposition < 5) {
        dooncegreen = 0;
        redtogreen();
      }
      else {
        greenwin = 1;
        gamestate = 4;
      }
      break;
    case 4:
      waittime = 60;
      if (redwin == 1) {
        redwin = 0;
        redscore++;
        if (redscore == 10) {
          redwinanimation();
          gamestate = 0;
          redscore = 0;
          greenscore = 0;
          dooncered = 0;
          dooncegreen = 0;
          leds2off();
          leds3off();
        }
        else {
          showscore();
          start = 1;
          gamestate = 1;
          dooncered = 0;
        }
      }
      if (greenwin == 1) {
        greenwin = 0;
        greenscore++;
        if (greenscore == 10) {
          greenwinanimation();
          gamestate = 0;
          redscore = 0;
          greenscore = 0;
          dooncered = 0;
          dooncegreen = 0;
          leds2off();
          leds3off();
        }
        else {
          showscore();
          start = 0;
          gamestate = 1;
          dooncegreen = 0;
        }
      }
      break;
  }
}

void redtogreenstart() {
  dooncered = 0;
  for (i = 0; i < NUM_LEDS1; i++) {
    leds1off();
    if (i < 5) {
      leds1[i].setRGB(255, 0, 0);
    }
    if (i >= 5 && i < 10) {
      leds1[i].setRGB(255, 255, 0);
    }
    if (i >= 10 && i < 30) {
      leds1[i].setRGB(0, 255, 0);
    }
    if (i >= 30 && i < 35) {
      leds1[i].setRGB(255, 255, 0);
    }
    if (i > 34) {
      leds1[i].setRGB(255, 0, 0);
    }
    FastLED.show();
    if (greenpush == 1) {
      ledposition = i;
      i = NUM_LEDS1;
      gamestate = 2;
      greenpush = 0;
    }
    pause();
    if (i == 39) {
      redwin = 1;
      gamestate = 4;
    }
  }
}

void greentoredstart() {
  dooncegreen = 0;
  for (i = 39; i > -1; i = i - 1) {
    leds1off();
    if (i < 5) {
      leds1[i].setRGB(255, 0, 0);
    }
    if (i >= 5 && i < 10) {
      leds1[i].setRGB(255, 255, 0);
    }
    if (i >= 10 && i < 30) {
      leds1[i].setRGB(0, 255, 0);
    }
    if (i >= 30 && i < 35) {
      leds1[i].setRGB(255, 255, 0);
    }
    if (i > 34) {
      leds1[i].setRGB(255, 0, 0);
    }
    FastLED.show();
    if (redpush == 1) {
      ledposition = i;
      i = -1;
      gamestate = 3;
      redpush = 0;
    }
    pause();
    if (i == 0) {
      greenwin = 1;
      gamestate = 4;
    }
  }
}

void redtogreen() {
  if (waittime == 3) {
  }
  else {
    waittime = waittime - 3;
  }
  for (i = ledposition; i < NUM_LEDS1; i++) {
    leds1off();
    if (i < 5) {
      leds1[i].setRGB(255, 0, 0);
    }
    if (i >= 5 && i < 10) {
      leds1[i].setRGB(255, 255, 0);
    }
    if (i >= 10 && i < 30) {
      leds1[i].setRGB(0, 255, 0);
    }
    if (i >= 30 && i < 35) {
      leds1[i].setRGB(255, 255, 0);
    }
    if (i > 34) {
      leds1[i].setRGB(255, 0, 0);
    }
    FastLED.show();
    if (greenpush == 1) {
      ledposition = i;
      i = NUM_LEDS1;
      gamestate = 2;
      greenpush = 0;
    }
    pause();
    if (i == 39) {
      redwin = 1;
      gamestate = 4;
    }
  }
}

void greentored() {
  if (waittime == 3) {
  }
  else {
    waittime = waittime - 3;
  }
  for (i = ledposition; i > -1; i = i - 1) {
    leds1off();
    if (i < 5) {
      leds1[i].setRGB(255, 0, 0);
    }
    if (i >= 5 && i < 10) {
      leds1[i].setRGB(255, 255, 0);
    }
    if (i >= 10 && i < 30) {
      leds1[i].setRGB(0, 255, 0);
    }
    if (i >= 30 && i < 35) {
      leds1[i].setRGB(255, 255, 0);
    }
    if (i > 34) {
      leds1[i].setRGB(255, 0, 0);
    }
    FastLED.show();
    if (redpush == 1) {
      ledposition = i;
      i = -1;
      gamestate = 3;
      redpush = 0;
    }
    pause();
    if (i == 0) {
      greenwin = 1;
      gamestate = 4;
    }
  }
}

void showscore() {
  leds2off();
  leds3off();
  switch (redscore) {
    case 0:
      for (i = 0; i < sizeof(N0); i++) {
        leds2[N0[i]].setRGB(255, 255, 255);
      }
      break;
    case 1:
      for (i = 0; i < sizeof(N1); i++) {
        leds2[N1[i]].setRGB(255, 255, 255);
      }
      break;
    case 2:
      for (i = 0; i < sizeof(N2); i++) {
        leds2[N2[i]].setRGB(255, 255, 255);
      }
      break;
    case 3:
      for (i = 0; i < sizeof(N3); i++) {
        leds2[N3[i]].setRGB(255, 255, 255);
      }
      break;
    case 4:
      for (i = 0; i < sizeof(N4); i++) {
        leds2[N4[i]].setRGB(255, 255, 255);
      }
      break;
    case 5:
      for (i = 0; i < sizeof(N5); i++) {
        leds2[N5[i]].setRGB(255, 255, 255);
      }
      break;
    case 6:
      for (i = 0; i < sizeof(N6); i++) {
        leds2[N6[i]].setRGB(255, 255, 255);
      }
      break;
    case 7:
      for (i = 0; i < sizeof(N7); i++) {
        leds2[N7[i]].setRGB(255, 255, 255);
      }
      break;
    case 8:
      for (i = 0; i < sizeof(N8); i++) {
        leds2[N8[i]].setRGB(255, 255, 255);
      }
      break;
    case 9:
      for (i = 0; i < sizeof(N9); i++) {
        leds2[N9[i]].setRGB(255, 255, 255);
      }
      break;
  }
  switch (greenscore) {
    case 0:
      for (i = 0; i < sizeof(N0); i++) {
        leds3[N0[i]].setRGB(255, 255, 255);
      }
      break;
    case 1:
      for (i = 0; i < sizeof(N1); i++) {
        leds3[N1[i]].setRGB(255, 255, 255);
      }
      break;
    case 2:
      for (i = 0; i < sizeof(N2); i++) {
        leds3[N2[i]].setRGB(255, 255, 255);
      }
      break;
    case 3:
      for (i = 0; i < sizeof(N3); i++) {
        leds3[N3[i]].setRGB(255, 255, 255);
      }
      break;
    case 4:
      for (i = 0; i < sizeof(N4); i++) {
        leds3[N4[i]].setRGB(255, 255, 255);
      }
      break;
    case 5:
      for (i = 0; i < sizeof(N5); i++) {
        leds3[N5[i]].setRGB(255, 255, 255);
      }
      break;
    case 6:
      for (i = 0; i < sizeof(N6); i++) {
        leds3[N6[i]].setRGB(255, 255, 255);
      }
      break;
    case 7:
      for (i = 0; i < sizeof(N7); i++) {
        leds3[N7[i]].setRGB(255, 255, 255);
      }
      break;
    case 8:
      for (i = 0; i < sizeof(N8); i++) {
        leds3[N8[i]].setRGB(255, 255, 255);
      }
      break;
    case 9:
      for (i = 0; i < sizeof(N9); i++) {
        leds3[N9[i]].setRGB(255, 255, 255);
      }
      break;
  }
  FastLED.show();
}

void redwinanimation() {
  leds1off();
  leds2off();
  leds3off();
  for (i = 0; i < 4; i++) {
    for (m = 0; m < NUM_LEDS2; m++) {
      leds2[m].setRGB(0, 255, 0);
    }
    for (m = 0; m < NUM_LEDS3; m++) {
      leds3[m].setRGB(255, 0, 0);
    }
    FastLED.show();
    delay(500);
    for (m = 0; m < NUM_LEDS2; m++) {
      leds2[m].setRGB(0, 0, 0);
    }
    for (m = 0; m < NUM_LEDS3; m++) {
      leds3[m].setRGB(0, 0, 0);
    }
    FastLED.show();
    delay(500);
  }
}

void greenwinanimation() {
  leds1off();
  leds2off();
  leds3off();
  for (i = 0; i < 4; i++) {
    for (m = 0; m < NUM_LEDS3; m++) {
      leds3[m].setRGB(0, 255, 0);
    }
    for (m = 0; m < NUM_LEDS2; m++) {
      leds2[m].setRGB(255, 0, 0);
    }
    FastLED.show();
    delay(500);
    for (m = 0; m < NUM_LEDS2; m++) {
      leds3[m].setRGB(0, 0, 0);
    }
    for (m = 0; m < NUM_LEDS3; m++) {
      leds2[m].setRGB(0, 0, 0);
    }
    FastLED.show();
    delay(500);
  }
}

void leds2off() {
  for (m = 0; m < NUM_LEDS2; m++) {
    leds2[m].setRGB(0, 0, 0);
  }
}

void leds3off() {
  for (m = 0; m < NUM_LEDS3; m++) {
    leds3[m].setRGB(0, 0, 0);
  }
}

void leds1off() {
  for (m = 0; m < NUM_LEDS1; m++) {
    leds1[m].setRGB(0, 0, 0);
  }
}

void pause() {
  starttime = millis();
  currenttime = millis();
  while (currenttime - starttime <= waittime) {
    currenttime = millis();
  }
}
