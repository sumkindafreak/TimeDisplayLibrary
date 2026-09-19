#ifndef TIMEDISPLAYLIBRARY_H
#define TIMEDISPLAYLIBRARY_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class TimeDisplayLibrary {
public:
    explicit TimeDisplayLibrary(Adafruit_NeoPixel &display);

    bool showNumber(uint16_t number, bool leadingZeros = false);
    bool showDigit(uint8_t position, uint8_t digit);

    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void setColor(uint32_t color);
    void setBrightness(uint8_t brightness);
    void clear();

    // Non-blocking effects. Call update() repeatedly from loop().
    void startElectricBuzz(uint32_t durationMs = 1500);
    void shuffleTo(uint16_t finalNumber, uint32_t durationMs = 2000, bool leadingZeros = false);
    void shuffleRandom(uint32_t durationMs = 2000, bool leadingZeros = false);
    void startFlicker(uint32_t durationMs = 1000);
    void startFlash(uint8_t red = 255, uint8_t green = 255, uint8_t blue = 255,
                    uint8_t flashes = 3, uint16_t intervalMs = 80);

    void update();
    bool isEffectRunning() const;
    void stopEffect();

    // The final value chosen by shuffleRandom(), or the target supplied to shuffleTo().
    uint16_t getEffectResult() const;

    // Legacy API retained for sketches written against v1.x.
    void twentyfive();
    void five_ten();
    void zero();
    void ninefourtwo();
    void oneeightfourtwo();

private:
    static const uint8_t DIGIT_COUNT = 4;
    static const uint8_t SEGMENTS_PER_DIGIT = 7;

    enum EffectType : uint8_t {
        EFFECT_NONE,
        EFFECT_ELECTRIC_BUZZ,
        EFFECT_SHUFFLE,
        EFFECT_FLICKER,
        EFFECT_FLASH
    };

    Adafruit_NeoPixel &_time_display;
    uint32_t _color;
    uint8_t _brightness;
    uint16_t _currentNumber;
    bool _currentLeadingZeros;
    bool _hasCurrentNumber;

    EffectType _effect;
    uint32_t _effectStartMs;
    uint32_t _effectDurationMs;
    uint32_t _nextFrameMs;
    uint16_t _effectResult;
    bool _effectLeadingZeros;

    // Shuffle state.
    uint8_t _shuffleDigits[DIGIT_COUNT];
    bool _shuffleStopped[DIGIT_COUNT];

    // Flash state.
    uint32_t _flashColor;
    uint8_t _flashCount;
    uint16_t _flashIntervalMs;
    uint8_t _flashTransitions;
    bool _flashOn;

    void drawDigit(uint8_t position, uint8_t digit);
    void clearDigit(uint8_t position);
    void drawNumberInternal(uint16_t number, bool leadingZeros, bool remember);
    void drawStoredNumber();
    void finishEffect();
    void updateElectricBuzz(uint32_t now);
    void updateShuffle(uint32_t now);
    void updateFlicker(uint32_t now);
    void updateFlash(uint32_t now);
};

#endif
