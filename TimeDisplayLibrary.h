#ifndef TIME_DISPLAY_LIBRARY_H
#define TIME_DISPLAY_LIBRARY_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

/**
 * @brief Reusable controller for a 28-pixel, four-digit NeoPixel clock.
 *
 * Each digit uses seven pixels. The default physical segment order is
 * A, B, C, D, E, F, G for each consecutive group of seven pixels.
 */
class NeoClock {
public:
    static constexpr uint8_t DIGIT_COUNT = 4;
    static constexpr uint8_t SEGMENTS_PER_DIGIT = 7;
    static constexpr uint16_t REQUIRED_PIXEL_COUNT = DIGIT_COUNT * SEGMENTS_PER_DIGIT;

    NeoClock();
    explicit NeoClock(Adafruit_NeoPixel &display);

    // Attach and initialise the NeoPixel strip. This enables the simple pattern:
    // NeoClock neoClock; neoClock.begin(pixels); neoClock.showTime(1900);
    bool begin(Adafruit_NeoPixel &display, bool initialisePixels = true);
    bool isReady() const;

    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void setColor(uint32_t packedColor);
    uint32_t getColor() const;

    void setBrightness(uint8_t brightness);
    uint8_t getBrightness() const;

    // Display any four-digit value. Leading zeroes are shown by default.
    bool showNumber(uint16_t value, bool leadingZeroes = true);

    // Display a clock value such as showTime(1900) or showTime(19, 0).
    // Invalid times are rejected and leave the current display unchanged.
    bool showTime(uint16_t hhmm);
    bool showTime(uint8_t hours, uint8_t minutes);

    // Display four explicit digits from left to right.
    bool showDigits(uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4);

    void setReverseDigitOrder(bool reversed);
    bool getReverseDigitOrder() const;

    void clear();
    bool hasValidPixelCount() const;

    // Original named patterns retained for older sketches.
    void displayTwentyFive();
    void displayFiveTen();
    void displayZero();
    void displayNineFourTwo();
    void displayOneEightFourTwo();

    // Original method names retained as aliases.
    void twentyfive();
    void five_ten();
    void zero();
    void ninefourtwo();
    void oneeightfourtwo();

private:
    void renderDigits(const uint8_t digits[DIGIT_COUNT], bool leadingZeroes);
    void renderDigit(uint8_t logicalPosition, uint8_t digit);
    void renderPattern(const uint8_t *pixelIndexes, size_t pixelCount);

    Adafruit_NeoPixel *_display;
    uint32_t _color;
    uint8_t _brightness;
    bool _reverseDigitOrder;
};

/**
 * @brief Backward-compatible class name for existing projects.
 */
class TimeDisplayLibrary : public NeoClock {
public:
    TimeDisplayLibrary() : NeoClock() {}
    explicit TimeDisplayLibrary(Adafruit_NeoPixel &display) : NeoClock(display) {}
};

#endif // TIME_DISPLAY_LIBRARY_H
