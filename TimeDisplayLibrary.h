#ifndef TIMEDISPLAYLIBRARY_H
#define TIMEDISPLAYLIBRARY_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class TimeDisplayLibrary {
public:
    enum NamedColor : uint8_t { RED, GREEN, BLUE, WHITE, PURPLE, CYAN, ORANGE, YELLOW, PINK, ELECTRIC_BLUE };
    enum Intensity : uint8_t { SUBTLE = 1, NORMAL = 2, WILD = 3 };

    explicit TimeDisplayLibrary(Adafruit_NeoPixel &display);

    bool showNumber(uint16_t number, bool leadingZeros = false);
    bool showDigit(uint8_t position, uint8_t digit);
    void blankDigit(uint8_t position);
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    void setColor(uint32_t color);
    void setColor(NamedColor color);
    // Hex colours use the familiar 0xRRGGBB format, e.g. 0x9900FF.
    void setColorHex(uint32_t hexColor);
    void randomColor();
    void setDigitColor(uint8_t position, uint8_t red, uint8_t green, uint8_t blue);
    void setDigitColor(uint8_t position, NamedColor color);
    void setDigitColorHex(uint8_t position, uint32_t hexColor);
    void randomDigitColors();
    void clearDigitColors();
    void setBrightness(uint8_t brightness);
    void clear();

    // Core FX
    void startElectricBuzz(uint32_t durationMs = 1500, Intensity intensity = NORMAL);
    void shuffleTo(uint16_t finalNumber, uint32_t durationMs = 2000, bool leadingZeros = false);
    void shuffleRandom(uint32_t durationMs = 2000, bool leadingZeros = false);
    void startFlicker(uint32_t durationMs = 1000, Intensity intensity = NORMAL);
    void startFlash(uint8_t red = 255, uint8_t green = 255, uint8_t blue = 255, uint8_t flashes = 3, uint16_t intervalMs = 80);

    // Expanded FX suite
    void startGlitch(uint32_t durationMs = 1200, Intensity intensity = NORMAL);
    void startBrownout(uint32_t durationMs = 1800);
    void startStrobe(uint32_t durationMs = 1000, uint16_t intervalMs = 60);
    void startBreathe(uint32_t durationMs = 3000, uint16_t cycleMs = 1200);
    void startPulse(uint32_t durationMs = 1500, uint16_t cycleMs = 350);
    void startRainbow(uint32_t durationMs = 5000, uint16_t frameMs = 30);
    void startColorCycle(uint32_t durationMs = 4000, uint16_t frameMs = 80);
    void startShimmer(uint32_t durationMs = 2000);
    void startSegmentChase(uint32_t durationMs = 1800, uint16_t frameMs = 70);
    void startPowerUp(uint16_t finalNumber, uint32_t durationMs = 1800, bool leadingZeros = false);
    void startPowerDown(uint32_t durationMs = 1200);
    void startRevealLeft(uint16_t finalNumber, uint32_t durationMs = 1200, bool leadingZeros = false);
    void startRevealRight(uint16_t finalNumber, uint32_t durationMs = 1200, bool leadingZeros = false);
    void startSlotMachine(uint16_t finalNumber, uint32_t durationMs = 3000, bool leadingZeros = false);
    void startOdometer(uint16_t finalNumber, uint32_t durationMs = 2500, bool leadingZeros = false);
    void startCountTo(uint16_t finalNumber, uint32_t durationMs = 2500, bool leadingZeros = false);
    void startFadeIn(uint32_t durationMs = 1000);
    void startFadeOut(uint32_t durationMs = 1000);

    void update();
    bool isEffectRunning() const;
    void stopEffect();
    uint16_t getEffectResult() const;

    // Legacy v1 API
    void twentyfive();
    void five_ten();
    void zero();
    void ninefourtwo();
    void oneeightfourtwo();

private:
    static const uint8_t DIGIT_COUNT = 4;
    static const uint8_t SEGMENTS_PER_DIGIT = 7;

    enum EffectType : uint8_t {
        EFFECT_NONE, EFFECT_ELECTRIC_BUZZ, EFFECT_SHUFFLE, EFFECT_FLICKER, EFFECT_FLASH,
        EFFECT_GLITCH, EFFECT_BROWNOUT, EFFECT_STROBE, EFFECT_BREATHE, EFFECT_PULSE,
        EFFECT_RAINBOW, EFFECT_COLOR_CYCLE, EFFECT_SHIMMER, EFFECT_SEGMENT_CHASE,
        EFFECT_POWER_UP, EFFECT_POWER_DOWN, EFFECT_REVEAL_LEFT, EFFECT_REVEAL_RIGHT,
        EFFECT_ODOMETER, EFFECT_COUNT_TO, EFFECT_FADE_IN, EFFECT_FADE_OUT
    };

    Adafruit_NeoPixel &_time_display;
    uint32_t _color;
    uint32_t _digitColors[DIGIT_COUNT];
    bool _digitColorEnabled[DIGIT_COUNT];
    uint8_t _brightness;
    uint16_t _currentNumber;
    bool _currentLeadingZeros;
    bool _hasCurrentNumber;

    EffectType _effect;
    Intensity _intensity;
    uint32_t _effectStartMs, _effectDurationMs, _nextFrameMs;
    uint16_t _effectResult;
    bool _effectLeadingZeros;
    uint16_t _effectIntervalMs;
    uint8_t _shuffleDigits[DIGIT_COUNT];
    bool _shuffleStopped[DIGIT_COUNT];
    uint32_t _flashColor;
    uint8_t _flashCount, _flashTransitions;
    bool _flashOn;

    uint32_t namedColor(NamedColor color) const;
    uint32_t hexToColor(uint32_t hexColor) const;
    uint32_t randomPaletteColor() const;
    uint32_t wheel(uint8_t pos) const;
    void drawDigit(uint8_t position, uint8_t digit);
    void drawDigitWithColor(uint8_t position, uint8_t digit, uint32_t color);
    void clearDigit(uint8_t position);
    void drawNumberInternal(uint16_t number, bool leadingZeros, bool remember);
    void drawStoredNumber();
    void beginEffect(EffectType effect, uint32_t durationMs);
    void finishEffect();
    void updateEffect(uint32_t now);
};

#endif
