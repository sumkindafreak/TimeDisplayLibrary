#ifndef TIME_DISPLAY_LIBRARY_H
#define TIME_DISPLAY_LIBRARY_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

/**
 * @brief Controls a 28-pixel, four-digit NeoPixel seven-segment display.
 *
 * Each digit uses seven pixels. New code can display any value from 0000 to
 * 9999, including clock-style HHMM values such as 1900.
 */
class TimeDisplayLibrary {
public:
    static constexpr uint8_t DIGIT_COUNT = 4;
    static constexpr uint8_t SEGMENTS_PER_DIGIT = 7;
    static constexpr uint16_t REQUIRED_PIXEL_COUNT = DIGIT_COUNT * SEGMENTS_PER_DIGIT;

    explicit TimeDisplayLibrary(Adafruit_NeoPixel &display);

    // Set the colour used by all rendered digits and legacy patterns.
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void setColor(uint32_t packedColor);
    uint32_t getColor() const;

    // Display any four-digit value. Leading zeroes are shown by default.
    bool showNumber(uint16_t value, bool leadingZeroes = true);

    // Display a clock value such as showTime(1900) or showTime(19, 0).
    // Invalid times are rejected and leave the current display unchanged.
    bool showTime(uint16_t hhmm);
    bool showTime(uint8_t hours, uint8_t minutes);

    // Display four explicit digits from left to right.
    bool showDigits(uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4);

    // Reverse the physical digit order when the NeoPixel chain is wired from
    // right to left. Segment order inside each digit remains A, B, C, D, E, F, G.
    void setReverseDigitOrder(bool reversed);
    bool getReverseDigitOrder() const;

    // Turn every display pixel off and immediately update the strip.
    void clear();

    // Returns true when the attached strip contains at least 28 pixels.
    bool hasValidPixelCount() const;

    // Original named patterns retained exactly for older Showduino sketches.
    void displayTwentyFive();
    void displayFiveTen();
    void displayZero();
    void displayNineFourTwo();
    void displayOneEightFourTwo();

    // Backward-compatible aliases retained for existing sketches.
    void twentyfive();
    void five_ten();
    void zero();
    void ninefourtwo();
    void oneeightfourtwo();

private:
    void renderDigits(const uint8_t digits[DIGIT_COUNT], bool leadingZeroes);
    void renderDigit(uint8_t logicalPosition, uint8_t digit);
    void renderPattern(const uint8_t *pixelIndexes, size_t pixelCount);

    Adafruit_NeoPixel &_display;
    uint32_t _color;
    bool _reverseDigitOrder;
};

#endif // TIME_DISPLAY_LIBRARY_H
