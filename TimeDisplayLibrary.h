#ifndef TIME_DISPLAY_LIBRARY_H
#define TIME_DISPLAY_LIBRARY_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

/**
 * @brief Controls a 28-pixel, four-digit NeoPixel seven-segment display.
 *
 * The library stores the original display masks used by the project while
 * providing readable method names, configurable colour, and compatibility
 * aliases for existing sketches.
 */
class TimeDisplayLibrary {
public:
    static constexpr uint16_t REQUIRED_PIXEL_COUNT = 28;

    explicit TimeDisplayLibrary(Adafruit_NeoPixel &display);

    // Set the colour used by all pattern-rendering methods.
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void setColor(uint32_t packedColor);
    uint32_t getColor() const;

    // Readable pattern names for new code.
    void displayTwentyFive();
    void displayFiveTen();
    void displayZero();
    void displayNineFourTwo();
    void displayOneEightFourTwo();

    // Turn every display pixel off and immediately update the strip.
    void clear();

    // Returns true when the attached strip contains at least 28 pixels.
    bool hasValidPixelCount() const;

    // Backward-compatible aliases retained for existing sketches.
    void twentyfive();
    void five_ten();
    void zero();
    void ninefourtwo();
    void oneeightfourtwo();

private:
    void renderPattern(const uint8_t *pixelIndexes, size_t pixelCount);

    Adafruit_NeoPixel &_display;
    uint32_t _color;
};

#endif // TIME_DISPLAY_LIBRARY_H
