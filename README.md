# TimeDisplayLibrary

Arduino library for a four-digit, 28-NeoPixel seven-segment display.

Version 2 turns the original fixed-pattern library into a general number display and adds non-blocking display effects. Any integer from **0 to 9999** can be rendered without manually defining an LED pattern.

## Dependencies

- Arduino
- Adafruit NeoPixel

## Basic example

```cpp
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <TimeDisplayLibrary.h>

#define DISPLAY_PIN 6
#define DISPLAY_PIXELS 28

Adafruit_NeoPixel pixels(DISPLAY_PIXELS, DISPLAY_PIN, NEO_GRB + NEO_KHZ800);
TimeDisplayLibrary display(pixels);

void setup() {
    Serial.begin(115200);
    pixels.begin();
    pixels.clear();
    pixels.show();

    display.setBrightness(80);
    display.setColor(255, 0, 0);
    display.showNumber(1842);
}

void loop() {
    // Required while a non-blocking effect is running.
    display.update();
}
```

## Numbers

```cpp
display.showNumber(0);
display.showNumber(25);
display.showNumber(510);
display.showNumber(1842);
display.showNumber(9999);
display.showNumber(25, true); // 0025
```

## Colours and brightness

```cpp
display.setColor(255, 0, 0);
display.setBrightness(100);
display.showNumber(1234);
```

## Electric buzz

Keeps the current number readable while rapidly arcing between electric blue, icy blue-white and hard white, with occasional brief dropouts.

```cpp
display.showNumber(1842);
display.startElectricBuzz(2000);

void loop() {
    display.update();
}
```

## Shuffle to a result

Randomises all four digits, slows down, then locks the digits from left to right onto the requested number.

```cpp
display.shuffleTo(1842, 2500);

void loop() {
    display.update();
}
```

## Shuffle to a random result

```cpp
display.shuffleRandom(2500);

void loop() {
    display.update();

    if (!display.isEffectRunning()) {
        uint16_t result = display.getEffectResult();
    }
}
```

For genuinely unpredictable results on ESP32, seed Arduino's random generator from an appropriate entropy source in your sketch before starting the shuffle.

## Flicker

```cpp
display.showNumber(1985);
display.startFlicker(1500);
```

## Flash

```cpp
display.startFlash(255, 255, 255, 3, 80);
```

All effects are non-blocking. Keep calling `display.update()` from `loop()`; the rest of your program remains free to handle buttons, sensors, networking and other jobs.

You can cancel an effect at any time:

```cpp
display.stopEffect();
```

## API

- `showNumber(number, leadingZeros)`
- `showDigit(position, digit)`
- `setColor(r, g, b)`
- `setBrightness(brightness)`
- `clear()`
- `startElectricBuzz(durationMs)`
- `shuffleTo(finalNumber, durationMs, leadingZeros)`
- `shuffleRandom(durationMs, leadingZeros)`
- `startFlicker(durationMs)`
- `startFlash(r, g, b, flashes, intervalMs)`
- `update()`
- `isEffectRunning()`
- `stopEffect()`
- `getEffectResult()`

The original v1.x named functions are retained for source compatibility.
