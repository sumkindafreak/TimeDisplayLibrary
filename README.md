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

### Hex colours

Colours can be copied directly from an RGB/hex colour chart using standard `0xRRGGBB` notation:

```cpp
display.setColorHex(0x9900FF); // purple
display.showNumber(1842);

display.setColorHex(0x00FFFF); // cyan
display.setColorHex(0xFF9900); // orange
display.setColorHex(0x33FF00); // bright green
display.setColorHex(0x0033FF); // blue
display.setColorHex(0xFFFFFF); // white
```

Per-digit hex colours are also supported:

```cpp
display.setDigitColorHex(0, 0xFF0000);
display.setDigitColorHex(1, 0xFF9900);
display.setDigitColorHex(2, 0xFFFF00);
display.setDigitColorHex(3, 0x00FF00);
display.showNumber(1842);
```

### Random palette colours

```cpp
display.randomColor();
display.showNumber(1842);

display.randomDigitColors();
display.showNumber(1842);
```

The random palette uses the same six RGB levels found on common colour charts: `00`, `33`, `66`, `99`, `CC` and `FF`. Pure black is skipped so a randomly selected display colour never disappears completely.

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
