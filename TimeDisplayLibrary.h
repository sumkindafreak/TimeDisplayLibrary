#ifndef TIMEDISPLAYLIBRARY_H
#define TIMEDISPLAYLIBRARY_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class TimeDisplayLibrary {
public:
    explicit TimeDisplayLibrary(Adafruit_NeoPixel &display);

    // Display a number from 0 to 9999.
    // leadingZeros=true displays 25 as 0025; false displays it as "  25".
    bool showNumber(uint16_t number, bool leadingZeros = false);

    // Display exactly one digit (0-9) in one of the four positions (0-3).
    bool showDigit(uint8_t position, uint8_t digit);

    // Set the colour used by all subsequent drawing calls.
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void setColor(uint32_t color);

    // Set NeoPixel brightness (0-255).
    void setBrightness(uint8_t brightness);

    // Clear the whole display.
    void clear();

    // Legacy API retained for sketches written against v1.x.
    void twentyfive();
    void five_ten();
    void zero();
    void ninefourtwo();
    void oneeightfourtwo();

private:
    static const uint8_t DIGIT_COUNT = 4;
    static const uint8_t SEGMENTS_PER_DIGIT = 7;

    Adafruit_NeoPixel &_time_display;
    uint32_t _color;

    void drawDigit(uint8_t position, uint8_t digit);
    void clearDigit(uint8_t position);
};

#endif
