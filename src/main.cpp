/*
  Input Arrays WS2812B Controller



  The circuit:
  - Digital Sensors input now uses 74HC165 PISO shift-register
  
  created 02-01-2021
  modified 31-07-2021
  Kolade O. Adetoyinbo [https://github.com/CaptWeiss](Capt. Weiss)

*/
#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <main.h>

#define DATA_PIN 12
#define NUM_LEDS 126      // 56 //126
#define COLOR_POT A1

uint8_t sensePINs = 0; // this persist input state to check for posible changes in the next input pin reads
bool started = false; // used to decide a specific operation on first power


// Define Connections to 74HC165
// PL pin 1
#define load 7
// CE pin 15
#define clockEnablePin 4
// Q7 pin 7
#define dataIn 5
// CP pin 2
#define clockIn 6


uint8_t S1[2] = {0, 20} ; // {segmentStartPixel, segmentLength}
uint8_t S2[2] = {S1[1], 46} ;
uint8_t S3[2] = {S1[1]+S2[1], 60} ;

boolean AllSeg[3] = {false, false, false} ;
uint8_t SegMask[3] = {0x3,0x3c,0xc0} ;


volatile uint8_t favColor = 120 ;
volatile uint8_t favSaturation = 255;
uint8_t brigtness = 255 ;

// Define the array of leds
CRGB leds[NUM_LEDS];


void setup() {
  Serial.begin(115200);
  delay(200);
  // 74HC165 pins
  pinMode(load, OUTPUT);
  pinMode(clockEnablePin, OUTPUT);
  pinMode(clockIn, OUTPUT);
  pinMode(dataIn, INPUT);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); // GRB ordering is typical
  FastLED.setBrightness(brigtness);
  readAnalogPin();
  intro();
  delay(5000);
}

void loop() {

  EVERY_N_SECONDS(2) {
    programme();
  }

  EVERY_N_MILLISECONDS(200) {
    fadeOffDueSegment();
    readAnalogPin();
  }

  EVERY_N_MILLISECONDS(400) {
    update();
  }

  EVERY_N_SECONDS(20) {
    blinkIndicator();
  }

  FastLED.show();
}



void programme() {
  // write pulse to load pin
  digitalWrite(load,LOW);
  delayMicroseconds(5);
  digitalWrite(load,HIGH);
  delayMicroseconds(5);
  
  // Get data from 74HC165
  digitalWrite(clockIn, HIGH);
  digitalWrite(clockEnablePin, LOW);
  uint8_t rawIncoming = shiftIn(dataIn, clockIn, LSBFIRST);
  uint8_t incoming = ~rawIncoming ;
  digitalWrite(clockEnablePin, HIGH);

  if (sensePINs ^ incoming) { // Check if any of the bits(input state) has changed since last checked
    Serial.print("\n\nPINs state changed to - ");
    Serial.print(incoming, BIN);
    Serial.print("【");
    Serial.print(incoming);
    Serial.println("】\n");
    Serial.print("\n\n【RAW】 PINs state changed to - ");
    Serial.print(rawIncoming, BIN);
    Serial.print("【");
    Serial.print(rawIncoming);
    Serial.println("】\n");
    for (uint8_t i = 0; i < 8; i++)
    {
      int n = incoming & (1 << i);
      int changedPIN = (sensePINs & (1 << i)) ^ n;
      if (changedPIN)
      {
        Serial.print("\nSwitch-");
        Serial.print(i + 1);
        Serial.println(n > 0 ? ": ON" : ": OFF\n");
        handleInputChange(i, n > 0);
      }
    }
    sensePINs = incoming;
  }
  readAnalogPin();
}

void fadeOffDueSegment() {
  for (uint8_t i = 0; i < 3; i++) {
    if ((SegMask[i] & sensePINs) == 0)
    { // this segment is off
      switch (i)
      {
      case 0:
        turnOff(S1[0], S1[1], i);
        if (AllSeg[i]){
          Serial.print("\nTurning off segment " + String(i + 1));
          AllSeg[i] = false;
        }
        break;
      case 1:
        turnOff(S2[0], S2[1], i);
        if (AllSeg[i]) {
          Serial.print("\nTurning off segment " + String(i + 1));
          AllSeg[i] = false;
        }
        break;
      case 2:
        turnOff(S3[0], S3[1], i);
        if (AllSeg[i]) {
          Serial.print("\nTurning off segment " + String(i + 1));
          AllSeg[i] = false;
        }
        break;
      default:
        break;
      }
    }
  }
}

void blinkIndicator() {
  leds[S1[0]+1] = CRGB::Green;
  FastLED.show();
  FastLED.delay(200);
  leds[S1[0]+1] = leds[S1[0]];
}

void handleInputChange(uint8_t pin, bool state) {
  switch (pin) {
    case 0:
    case 1:
      if (state) turnOn(S1[0], S1[1], 0);
      break;
    case 2:
    case 3:
    case 4:
    case 5:
      if (state) turnOn(S2[0],S2[1],1);
      break;
    case 6:
    case 7:
      if (state) turnOn(S3[0],S3[1],2);
      break;
  }
};

void turnOn(uint8_t start, uint8_t leds_length, uint8_t segIdx) {
  if(!AllSeg[segIdx]) {
    Serial.print("\nTurning on segment "+String(segIdx+1));
    for (int i = 0; i < brigtness; i+=10) {
      fill_solid(leds+start, leds_length, CHSV(favColor, favSaturation, i));
      FastLED.show();
      FastLED.delay(10);
    }
    AllSeg[segIdx] = true ;
  }
}

void turnOff(uint8_t start, uint8_t leds_length, uint8_t segIdx) {
    fadeToBlackBy(leds + start, leds_length, 30);
}

void readAnalogPin() {
  int analVal = analogRead(COLOR_POT);
    if(analVal<=776){
      favColor = map(analVal, 0, 776, 0, 255);
    }
    if(analVal>776) {
      favSaturation = map(analVal, 777, 1023, 255, 0);
    }
}

void update() {
  for (uint8_t i = 0; i < 3; i++) {
    if ((SegMask[i] & sensePINs) > 0) {
      Serial.print("\n This Segment "+String(i)+" is currently on!\n");
        switch (i) {
        case 0:
          fill_solid(leds + S1[0], S1[1], CHSV(favColor, favSaturation, brigtness));
          break;
        case 1:
          fill_solid(leds + S2[0], S2[1], CHSV(favColor, favSaturation, brigtness));
          break;
        case 2:
          fill_solid(leds + S3[0], S3[1], CHSV(favColor, favSaturation, brigtness));
          break;
        default:
          break;
        }
    }
  }
}

void intro() {
  fill_solid(leds+S1[0],S1[1],CHSV(favColor-50,favSaturation,150)) ;
  delay(2000);
  FastLED.show();
  fill_solid(leds+S2[0],S2[1],CHSV(favColor-100,favSaturation,150)) ;
  delay(2000);
  FastLED.show();
  fill_solid(leds+S3[0],S3[1],CHSV(favColor-150,favSaturation,150)) ;
  delay(2000);
  FastLED.show();
  for(uint8_t i = 0; i<100 ; i++) {
    fadeToBlackBy(leds,NUM_LEDS,30);
  }
  fill_solid(leds,NUM_LEDS,CRGB::Black);
  delay(2000);
}
