/*
  Input Arrays WS2812B Controller



  The circuit:
  - Digital Sensors from GPIO pins 2 through 12 to ground

  LED_S_D = LED Signal Direction
  PF = Pattern Flow

  created 2021
  by Kolade O. Adetoyinbo (Capt. Weiss)

*/
#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#define DATA_PIN 13
#define PIXELS 126      // 56 //126
unsigned int sensePINs; // 11 sensors are connected to these PINs
unsigned int PINs;      // stores the PIN checks at interval
bool started = false;

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0; // will store last time LED was updated

// constants won't change :
const int interval = 4000;

// LEDs configuration

uint8_t G1 = 20;              // 8 ; // 20
uint8_t G2 = 46;              // 20 ; //46
uint8_t G3[3] = {20, 20, 20}; // {8, 8, 8} ; //{20, 20, 20}

// Do not change this setting
int G100 = G1 / 2;
int G101 = G1;
int G200 = G101 + G2 / 4;
int G201 = G101 + G2 / 2;
int G202 = G101 + ((G2 / 4) * 3);
int G203 = G101 + G2;
int G300 = G203 + (G3[0] / 2);
int G301 = G203 + G3[0];
int G310 = G301 + (G3[1] / 2);
int G311 = G301 + G3[1];
int G320 = G311 + (G3[2] / 2);
int G321 = G311 + G3[2];

// Define the array of leds
CRGB leds[PIXELS];

/**
 * Checks the state of all the inputs from D2 ~ D12 and marges it together.
 * This gives us the status of all the inputs
 */
unsigned int checkSensePINs()
{
  // loop from the lowest pin to the highest:
  if (!started)
  {
    Serial.println("Checking Sensor PINs...");
    started = true;
  }
  uint8_t a = PIND; // read Digital PIN 0 ~ 7
  uint8_t b = PINB; // read Digital PIN 8 ~ 15

  a >>= 4; // Discard PIN 0 & 1 as they are the Rx and Tx respectively and we do not need their state ; 00001111
  b <<= 3; // Discard the 3 MSB(Most Significant Bits). The two MSB are D14 and D15 doesn't exist and the next(3rd) MSB, D13, is configured as Output.; 11111000

  unsigned int c = b;

  c = (c << 1) | a; //Merge both pins together
  return c;
}

int handleInputChange(uint8_t pin, bool state)
{
  switch (pin)
  {
  // Group1
  case 2:
    if (state)
    {
      //start LED sequence
      // wakeupBlink
      for (uint8_t i = 0; i < 3; i++)
      {
        leds[(G1 / 2) - 1] = CRGB::Red;
        FastLED.show();
        delay(100);
        leds[(G1 / 2) - 1] = CRGB::Black;
        FastLED.show();
        delay(100);
      }
      // Turn the LEDs on
      for (int i = (G1 / 2) - 1; i >= 0; i--)
      {
        leds[i] = CRGB::Blue;
        FastLED.show();
        delay(100);
      }
    }
    else
    {
      // terminate LED sequence
      // Turn the LEDs off
      for (uint8_t i = 0; i < (G1 / 2); i++)
      {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(100);
      }
    }
    break;

  case 3:
    if (state)
    {
      //start LED sequence
      // wakeupBlink
      for (uint8_t i = 0; i < 3; i++)
      {
        leds[G1 / 2] = CRGB::Red;
        FastLED.show();
        delay(100);
        leds[G1 / 2] = CRGB::Black;
        FastLED.show();
        delay(100);
      }
      // Turn the LEDs on
      for (uint8_t i = (G1 / 2); i < G1; i++)
      {
        leds[i] = CRGB::Blue;
        FastLED.show();
        delay(100);
      }
    }
    else
    {
      // terminate LED sequence
      // Now turn the LED off, then pause
      for (uint8_t i = G1 - 1; i >= (G1 / 2); i--)
      {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(100);
      }
    }
    break;
  // Group 2
  case 4:
    if (state)
    {
      //start LED sequence
      // wakeupBlink
      for (uint8_t i = 0; i < 3; i++)
      {
        leds[(G1 + (G2 / 4)) - 1] = CRGB::Red;
        FastLED.show();
        delay(50);
        leds[(G1 + (G2 / 4)) - 1] = CRGB::Black;
        FastLED.show();
        delay(50);
      }
      // Turn the LEDs on
      for (int i = (G1 + (G2 / 4)) - 1; i >= G1; i--)
      {
        leds[i] = CRGB::Blue;
        FastLED.show();
        delay(100);
        Serial.println(i);
      }
    }
    else
    {
      // terminate LED sequence
      // Turn the LEDs off
      for (uint8_t i = G1; i < (G1 + (G2 / 4)); i++)
      {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(100);
      }
    }
    break;

  case 5:
    if (state)
    {
      //start LED sequence
      // wakeupBlink
      for (uint8_t i = 0; i < 3; i++)
      {
        leds[G1 + (G2 / 4)] = CRGB::Red;
        FastLED.show();
        delay(50);
        leds[G1 + (G2 / 4)] = CRGB::Black;
        FastLED.show();
        delay(50);
      }
      // Turn the LEDs on
      for (uint8_t i = (G1 + (G2 / 4)); i < G1 + (G2 / 2); i++)
      {
        leds[i] = CRGB::Blue;
        FastLED.show();
        delay(100);
      }
    }
    else
    {
      // terminate LED sequence
      // Now turn the LED off, then pause
      for (uint8_t i = G1 + (G2 / 2) - 1; i >= (G1 + (G2 / 4)); i--)
      {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(100);
      }
    }
    break;

  case 6:
    if (state)
    {
      //start LED sequence
      // wakeupBlink
      for (uint8_t i = 0; i < 3; i++)
      {
        leds[(G1 + ((G2 / 4) * 3)) - 1] = CRGB::Red;
        FastLED.show();
        delay(50);
        leds[(G1 + ((G2 / 4) * 3)) - 1] = CRGB::Black;
        FastLED.show();
        delay(50);
      }
      // Turn the LEDs on
      for (int i = (G1 + ((G2 / 4) * 3)) - 1; i >= G1 + (G2 / 2); i--)
      {
        leds[i] = CRGB::Blue;
        FastLED.show();
        delay(100);
        Serial.println(i);
      }
    }
    else
    {
      // terminate LED sequence
      // Turn the LEDs off
      for (uint8_t i = G1 + (G2 / 2); i < (G1 + ((G2 / 4) * 3)); i++)
      {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(100);
      }
    }
    break;

  case 7:
    if (state)
    {
      //start LED sequence
      // wakeupBlink
      for (uint8_t i = 0; i < 3; i++)
      {
        leds[(G1 + ((G2 / 4) * 3))] = CRGB::Red;
        FastLED.show();
        delay(100);
        leds[(G1 + ((G2 / 4) * 3))] = CRGB::Black;
        FastLED.show();
        delay(100);
      }
      // Turn the LEDs on
      for (uint8_t i = (G1 + ((G2 / 4) * 3)); i < G1 + G2; i++)
      {
        leds[i] = CRGB::Blue;
        FastLED.show();
        delay(100);
      }
    }
    else
    {
      // terminate LED sequence
      // Now turn the LED off, then pause
      for (uint8_t i = G1 + G2; i >= (G1 + ((G2 / 4) * 3)); i--)
      {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(100);
      }
    }
    break;
    // Group 3

  case 8:
    if (state)
    {
      //start LED sequence
      // wakeupBlink
      for (uint8_t i = 0; i < 3; i++)
      {
        leds[G300 - 1] = CRGB::Red;
        leds[G310 - 1] = CRGB::Green;
        leds[G320 - 1] = CRGB::Purple;
        FastLED.show();
        delay(50);
        leds[G300 - 1] = CRGB::Black;
        leds[G310 - 1] = CRGB::Black;
        leds[G320 - 1] = CRGB::Black;
        FastLED.show();
        delay(50);
      }
      // Turn the LEDs on
      for (uint8_t i = G300 - 1; i >= G203; i--)
      { // LED_S_D: >>> | PF: <...<
        leds[i] = CRGB::Blue;
        FastLED.show();
        delay(50);
      }
      for (uint8_t i = G310; i < G311; i++)
      { // LED_S_D: <<< | PF: <...<
        leds[i] = CRGB::Blue;
        FastLED.show();
        delay(50);
      }
      for (uint8_t i = G320 - 1; i >= G311; i--)
      { // LED_S_D: >>> | PF: <...<
        leds[i] = CRGB::Blue;
        FastLED.show();
        delay(50);
      }
    }
    else
    {
      // terminate LED sequence
      // Turn the LEDs off
      for (uint8_t i = G203; i < G300; i++)
      {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(50);
      }
      for (uint8_t i = G311; i >= G310; i--)
      {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(50);
      }
      for (uint8_t i = G311; i < G320; i++)
      {
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(50);
      }
    }
    break;

  case 9:
    if (state)
    {
      //start LED sequence
      // wakeupBlink
      for (uint8_t i = 0; i < 3; i++)
      {
        leds[G300] = CRGB::Red;
        leds[G310] = CRGB::Green;
        leds[G320] = CRGB::Purple;
        FastLED.show();
        delay(50);
        leds[G300] = CRGB::Black;
        leds[G310] = CRGB::Black;
        leds[G320] = CRGB::Black;
        FastLED.show();
        delay(50);
      }
      // Turn the LEDs on
      for (uint8_t i = G300; i < G301; i++)
      { // LED_S_D: >>> | PF: >...>
        leds[i] = CRGB::Blue;
        FastLED.show();
        delay(50);
      }
      for (uint8_t i = G310 - 1; i > G301; i--)
      { // LED_S_D: <<< | PF: >...>
        leds[i] = CRGB::Blue;
        FastLED.show();
        delay(50);
      }
      for (uint8_t i = G320; i < G321; i++)
      { // LED_S_D: >>> | PF: >...>
        leds[i] = CRGB::Blue;
        FastLED.show();
        delay(50);
      }
    }
    else
    {
      // terminate LED sequence
      // Turn the LEDs off
      for (uint8_t i = G301; i >= G300; i--)
      { // LED_S_D: >>> | PF: <...<
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(50);
      }
      for (uint8_t i = G301; i < G310; i++)
      { // LED_S_D: <<< | PF: <...<
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(50);
      }
      for (uint8_t i = G321; i >= G320; i--)
      { // LED_S_D: >>> | PF: <...<
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(50);
      }
    }
    break;
  }
};

void setup()
{
  DDRB = B11100000;
  DDRD = DDRD | B00000000;                               // this is safer way to as it sets pins 2 to 7 as input without changing the value of pins 0 & 1, which are RX & TX
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, PIXELS); // GRB ordering is typical
  FastLED.setBrightness(180);

  Serial.begin(9600);
  Serial.println("Reading inputs...");

  Serial.print("PIND D0~D7 in BIN: ");
  Serial.print(PIND, BIN);
  Serial.print(" - DEC: ");
  Serial.println(PIND);

  Serial.print("PINB D8~D15 in BIN: ");
  Serial.print(PINB, BIN);
  Serial.print(" - DEC: ");
  Serial.println(PINB);
  Serial.println();
  sensePINs = checkSensePINs();
  PINs = sensePINs;
  Serial.println();
  delay(5000);
  Serial.println("************************ - STARTED - *****************************");
  Serial.println();
}

void loop()
{
  PINs = checkSensePINs();
  if (sensePINs ^ PINs)
  { // Check if any of the bits(input state) has changed since last checked
    Serial.print("PINs state changed to - ");
    Serial.print(PINs, BIN);
    Serial.print("(");
    Serial.print(PINs);
    Serial.println(")");
    for (uint8_t i = 0; i < 11; i++)
    {
      int n = PINs & (1 << i);
      int changedPIN = (sensePINs & (1 << i)) ^ n;
      if (changedPIN)
      {
        Serial.print("Switch-");
        Serial.print(i + 2);
        Serial.println(n > 0 ? ": ON" : ": OFF");
        Serial.println();
        handleInputChange(i + 2, n > 0);
      }
    }
    sensePINs = PINs;
  }

  //Check Sensors state and Blink LED
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    PORTB ^= B00100000;
    delay(10);
    PORTB ^= B00100000;
  }
}