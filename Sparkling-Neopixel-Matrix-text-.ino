//Sparkling Text using NeoPixelBusGfx
// Contains code (c) Adafruit, license BSD

#include <NeoPixelBrightnessBusGfx.h>
#include <NeoPixelBrightnessBus.h>

//////////////////////////////////////////////////////
// Enter you text here
char MessageTop[] = "Happy Fathers Day";
char MessageBot[] = "To the best Dad Ever";

// Pins are method specific. See https://github.com/Makuna/NeoPixelBus/wiki/NeoPixelBus-object-API
#define PIN 2

// Max is 255, 32 is a conservative value to not overload
// a USB power supply (500mA) for 12x12 pixels.
#define BRIGHTNESS 96

// Define matrix width and height.
#define mw 32
#define mh 32

// See NeoPixelBus documentation for choosing the correct Feature and Method
// (https://github.com/Makuna/NeoPixelBus/wiki/NeoPixelBus-object)
//NeoPixelBrightnessBusGfx<NeoGrbFeature, Neo800KbpsMethod> *matrix = new NeoPixelBrightnessBusGfx<NeoGrbFeature, Neo800KbpsMethod>(mw, mh, PIN);
NeoPixelBrightnessBusGfx<NeoGrbFeature, NeoWs2812xMethod> *matrix = new NeoPixelBrightnessBusGfx<NeoGrbFeature, NeoWs2812xMethod>(mw, mh, PIN);

// See NeoPixelBus documentation for choosing the correct NeoTopology
// you may also use NeoTile or NeoMosaik
// (https://github.com/Makuna/NeoPixelBus/wiki/Matrix-Panels-Support)
NeoTiles<ColumnMajorAlternatingLayout, ColumnMajorLayout> topo(
  32,  // Panel width
  8,   // Panel Height
  1,   // # of panels wide
  4);  // # of panels high

//////////////////////////////////////////////////////
uint16_t ScrollPixels;  // adjust this to get the scrolling text all the way

//////////////////////////////////////////////////////
// This could also be defined as matrix->color(255,0,0) but those defines
// are meant to work for adafruit_gfx backends that are lacking color()
#define LED_BLACK 0

//////////////////////////////////////////////////////
// where the magic happens
//  modify a non black pixel to be random RGB value
void Sparkle(void) {
  // sparkle
  for (uint16_t Width = 0; Width < mw; Width++) {
    for (uint16_t Height = 0; Height < mh; Height++) {
      if (matrix->GetPixelColor(topo.Map(Width, Height)) != LED_BLACK) {
        matrix->SetPixelColor(topo.Map(Width, Height), RgbColor(random(0, 255), random(0, 255), random(0, 255)));
      }
    }
  }
}

void display_scrollText() {
  matrix->clear();
  matrix->setTextWrap(false);  // we don't wrap text so it scrolls nicely
  matrix->setTextSize(2);
  matrix->setRotation(0);

  for (int16_t Index = mw; Index >= -ScrollPixels; Index--) {
    Serial.printf("Offset: %d\n", Index);

    matrix->clear();
    matrix->setCursor(Index, 0);
    matrix->print(MessageTop);
    matrix->setCursor(Index, 16);
    matrix->print(MessageBot);

    Sparkle();

    matrix->Show();
    delay(100);
  }
}

void loop() {
  display_scrollText();
}

// use a remap function to remap based on the topology, tile or mosaik
// this function is passed as remap function to the matrix
uint16_t remap(uint16_t x, uint16_t y) {
  return topo.Map(x, y);
}

void setup() {
  Serial.begin(115200);

  // pass the remap function
  matrix->setRemapFunction(&remap);

  matrix->Begin();
  matrix->setTextWrap(false);
  matrix->SetBrightness(BRIGHTNESS);

  // Test full bright of all LEDs. If brightness is too high
  // for your current limit (i.e. USB), decrease it.
  matrix->fillScreen(LED_BLACK);
  matrix->Show();
  delay(1000);
  matrix->clear();

  // determine the message pinxel length
  int16_t tx1, ty1, bx1, by1;
  uint16_t tw, th, bw, bh;
  matrix->getTextBounds(MessageTop, 0, 0, &tx1, &ty1, &tw, &th);
  matrix->getTextBounds(MessageBot, 0, 0, &bx1, &by1, &bw, &bh);

  ScrollPixels = max(tw, bw) * 2;
  Serial.printf("ScrollPixels: %d\n", ScrollPixels);
}

// vim:sts=4:sw=4