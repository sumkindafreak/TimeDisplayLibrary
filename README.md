# TimeDisplayLibrary

Arduino library for a four-digit, 28-NeoPixel seven-segment display.

Version 2 turns the original fixed-pattern library into a general number display. Any integer from **0 to 9999** can be rendered without manually defining the LED pattern.

## Dependencies

- Arduino
- Adafruit NeoPixel

## Basic example

```cpp
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

    // Displays 1842.
    display.showNumber(1842);
}

void loop() {
}
```

## Display any number

```cpp
display.showNumber(0);
display.showNumber(25);
display.showNumber(510);
display.showNumber(1842);
display.showNumber(9999);
```

By default unused digits are blank. To force leading zeros:

```cpp
display.showNumber(25, true); // 0025
```

## Colours

```cpp
display.setColor(255, 0, 0);   // red
display.showNumber(1234);

display.setColor(0, 255, 0);   // green
display.showNumber(5678);

display.setColor(0, 0, 255);   // blue
display.showNumber(42);
```

## Individual digits

Positions run from 0 (left-most) to 3 (right-most).

```cpp
display.showDigit(0, 9);
display.showDigit(1, 4);
display.showDigit(2, 2);
display.showDigit(3, 1);
```

## API

- `showNumber(number, leadingZeros)` - display 0-9999.
- `showDigit(position, digit)` - display a single digit.
- `setColor(r, g, b)` - set the display colour.
- `setColor(color)` - set a packed NeoPixel colour.
- `setBrightness(brightness)` - set brightness from 0-255.
- `clear()` - turn all pixels off.

The original v1.x named functions are retained for source compatibility.
