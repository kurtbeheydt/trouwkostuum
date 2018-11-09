#include <Adafruit_NeoPixel.h>

const uint8_t rgbPin = 0;
const uint8_t totalLeds = 5;
const uint16_t brightness = 200;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(totalLeds, 0, NEO_GRB + NEO_KHZ800);

uint16_t energy = 3200;
const uint16_t maxEnergy = 5000;
const uint16_t energyIncrease = 200;
const uint16_t energyDecrease = 10;

uint8_t ledCount = 3;
boolean fadeDirection = true;
uint8_t color = 0;

const uint8_t sensorPin = 2;
uint8_t sensorInput = 0;

unsigned long lastMillis = 0;

void setup() {
  strip.begin();
  strip.show(); 
  
  pinMode(sensorPin, INPUT);
  pinMode(1, OUTPUT);
  
  ledCount = totalLeds;
}

void loop() {
  // energy verliezen

  if (millis() - lastMillis > 1000) {
    useEnergy();
    lastMillis = millis();
  }
  // lezen of er nergy gewonnen mag worden
  sensorInput = digitalRead(sensorPin);
  
  // als er energie gewonnen mag worden, doe dat dan, anders toon standbymodus
  if (sensorInput == 0) {
    digitalWrite(1, HIGH);
    loadingSequence(strip.Color(brightness, brightness, brightness));
  } else {
    digitalWrite(1, LOW);
    standbymodus();
  }
  
  // geef de neopixels een kans om te ademen
  delay(50);
}

void useEnergy() {
  if (energy > 0) {
    energy = energy - energyDecrease;
  }
}

void standbymodus() {
  ledCount = ((float)energy / (float)maxEnergy) * (float)(totalLeds + 1);
  
  if (fadeDirection == true) {
    color = color + 2;    
    if (color >= brightness) {
      fadeDirection = false;
    } 
  } else {
    color = color - 2;    
    if (color <= 30) {
      fadeDirection = true;
    }   
  } 

  for (int i = 0; i < ledCount; i++) {
    strip.setPixelColor(i, strip.Color(color, color, color));
  }
  for (int j = ledCount; j < totalLeds; j++) {
    strip.setPixelColor(j, strip.Color(0, 0, 0));
  }
  strip.show();    

}

void loadingSequence(uint32_t color) {
  energy = energy + energyIncrease;
  
  if (energy >= maxEnergy) {
    energy = maxEnergy;
    flashAllPixels(color);
  } else {
    for (int i = 0; i < totalLeds; i++) {
      flashPixel(i, color);
    }
  }
}

void flashPixel(uint16_t i, uint32_t color) {
  strip.setPixelColor(i, color);
  strip.show();
  delay(50);
  strip.setPixelColor(i, strip.Color(0, 0, 0));
  strip.show();  
}

void flashAllPixels(uint32_t color) {
  for (int i = 0; i < totalLeds; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
  delay(50);
  for (int i = 0; i < totalLeds; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();  
}
