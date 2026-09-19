# TimeDisplayLibrary

A four-digit / 28-NeoPixel seven-segment Arduino display library with a built-in non-blocking FX engine.

## Core display

```cpp
display.setColor(TimeDisplayLibrary::PURPLE);
display.setBrightness(100);
display.showNumber(1842);
```

Numbers 0-9999 are supported. Pass `true` for leading zeros:

```cpp
display.showNumber(25, true); // 0025
```

## Named colours

`RED`, `GREEN`, `BLUE`, `WHITE`, `PURPLE`, `CYAN`, `ORANGE`, `YELLOW`, `PINK`, `ELECTRIC_BLUE`.

Custom RGB remains supported:

```cpp
display.setColor(120, 10, 255);
```

Each digit can have its own colour:

```cpp
display.setDigitColor(0, TimeDisplayLibrary::RED);
display.setDigitColor(1, TimeDisplayLibrary::ORANGE);
display.setDigitColor(2, TimeDisplayLibrary::YELLOW);
display.setDigitColor(3, TimeDisplayLibrary::GREEN);
display.showNumber(1842);
```

## FX

Every effect is non-blocking. Always call:

```cpp
void loop() {
  display.update();
}
```

Available effects include:

- Electric buzz: `startElectricBuzz()`
- Shuffle / random shuffle: `shuffleTo()`, `shuffleRandom()`
- Slot machine: `startSlotMachine()`
- Odometer / counter: `startOdometer()`, `startCountTo()`
- Flicker / shimmer / brownout: `startFlicker()`, `startShimmer()`, `startBrownout()`
- Glitch: `startGlitch()`
- Flash / strobe: `startFlash()`, `startStrobe()`
- Breathe / pulse: `startBreathe()`, `startPulse()`
- Fade: `startFadeIn()`, `startFadeOut()`
- Rainbow / colour cycle: `startRainbow()`, `startColorCycle()`
- Segment chase: `startSegmentChase()`
- Power up / power down: `startPowerUp()`, `startPowerDown()`
- Left/right reveal: `startRevealLeft()`, `startRevealRight()`

Electrical effects support intensity where appropriate:

```cpp
display.startElectricBuzz(2500, TimeDisplayLibrary::WILD);
display.startGlitch(1500, TimeDisplayLibrary::SUBTLE);
```

## Complete example

```cpp
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <TimeDisplayLibrary.h>

#define DISPLAY_PIN 5
#define DISPLAY_PIXELS 28

Adafruit_NeoPixel pixels(DISPLAY_PIXELS, DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
TimeDisplayLibrary display(pixels);

void setup() {
  pixels.begin();
  pixels.clear();
  pixels.show();

  randomSeed(esp_random());

  display.setBrightness(100);
  display.setColor(TimeDisplayLibrary::ELECTRIC_BLUE);
  display.showNumber(1842);
  display.startElectricBuzz(2000, TimeDisplayLibrary::WILD);
}

void loop() {
  display.update();
}
```

Use `isEffectRunning()`, `stopEffect()` and `getEffectResult()` to integrate effects into larger projects.

The original v1 named methods remain available for source compatibility.
