#include "TimeDisplayLibrary.h"

static const uint8_t DIGIT_SEGMENTS[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

// Existing 28-pixel hardware mapping: A=6, B=5, C=4, D=3, E=2, F=1, G=0.
static const uint8_t SEGMENT_TO_PIXEL[7] = {6, 5, 4, 3, 2, 1, 0};

TimeDisplayLibrary::TimeDisplayLibrary(Adafruit_NeoPixel &display)
    : _time_display(display),
      _color(display.Color(255, 0, 0)),
      _brightness(255),
      _currentNumber(0),
      _currentLeadingZeros(false),
      _hasCurrentNumber(false),
      _effect(EFFECT_NONE),
      _effectStartMs(0),
      _effectDurationMs(0),
      _nextFrameMs(0),
      _effectResult(0),
      _effectLeadingZeros(false),
      _flashColor(0),
      _flashCount(0),
      _flashIntervalMs(80),
      _flashTransitions(0),
      _flashOn(false) {
    for (uint8_t i = 0; i < DIGIT_COUNT; ++i) {
        _shuffleDigits[i] = 0;
        _shuffleStopped[i] = false;
    }
}

void TimeDisplayLibrary::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    _color = _time_display.Color(red, green, blue);
}

void TimeDisplayLibrary::setColor(uint32_t color) {
    _color = color;
}

void TimeDisplayLibrary::setBrightness(uint8_t brightness) {
    _brightness = brightness;
    _time_display.setBrightness(_brightness);
    _time_display.show();
}

void TimeDisplayLibrary::clearDigit(uint8_t position) {
    if (position >= DIGIT_COUNT) return;
    const uint16_t firstPixel = position * SEGMENTS_PER_DIGIT;
    for (uint8_t i = 0; i < SEGMENTS_PER_DIGIT; ++i) {
        _time_display.setPixelColor(firstPixel + i, 0);
    }
}

void TimeDisplayLibrary::drawDigit(uint8_t position, uint8_t digit) {
    if (position >= DIGIT_COUNT || digit > 9) return;

    clearDigit(position);
    const uint16_t firstPixel = position * SEGMENTS_PER_DIGIT;
    const uint8_t segments = DIGIT_SEGMENTS[digit];

    for (uint8_t segment = 0; segment < 7; ++segment) {
        if (segments & (1U << segment)) {
            _time_display.setPixelColor(firstPixel + SEGMENT_TO_PIXEL[segment], _color);
        }
    }
}

bool TimeDisplayLibrary::showDigit(uint8_t position, uint8_t digit) {
    if (position >= DIGIT_COUNT || digit > 9) return false;
    stopEffect();
    drawDigit(position, digit);
    _time_display.show();
    return true;
}

void TimeDisplayLibrary::drawNumberInternal(uint16_t number, bool leadingZeros, bool remember) {
    uint8_t digits[DIGIT_COUNT] = {
        static_cast<uint8_t>((number / 1000) % 10),
        static_cast<uint8_t>((number / 100) % 10),
        static_cast<uint8_t>((number / 10) % 10),
        static_cast<uint8_t>(number % 10)
    };

    bool started = leadingZeros;
    for (uint8_t position = 0; position < DIGIT_COUNT; ++position) {
        if (!started && digits[position] == 0 && position < DIGIT_COUNT - 1) {
            clearDigit(position);
        } else {
            started = true;
            drawDigit(position, digits[position]);
        }
    }

    _time_display.show();

    if (remember) {
        _currentNumber = number;
        _currentLeadingZeros = leadingZeros;
        _hasCurrentNumber = true;
    }
}

bool TimeDisplayLibrary::showNumber(uint16_t number, bool leadingZeros) {
    if (number > 9999) return false;
    stopEffect();
    drawNumberInternal(number, leadingZeros, true);
    return true;
}

void TimeDisplayLibrary::drawStoredNumber() {
    if (_hasCurrentNumber) {
        drawNumberInternal(_currentNumber, _currentLeadingZeros, false);
    } else {
        _time_display.clear();
        _time_display.show();
    }
}

void TimeDisplayLibrary::clear() {
    stopEffect();
    _hasCurrentNumber = false;
    _time_display.clear();
    _time_display.show();
}

void TimeDisplayLibrary::startElectricBuzz(uint32_t durationMs) {
    if (durationMs == 0) return;
    stopEffect();
    _effect = EFFECT_ELECTRIC_BUZZ;
    _effectStartMs = millis();
    _effectDurationMs = durationMs;
    _nextFrameMs = 0;
}

void TimeDisplayLibrary::shuffleTo(uint16_t finalNumber, uint32_t durationMs, bool leadingZeros) {
    if (finalNumber > 9999) finalNumber = 9999;
    if (durationMs < 250) durationMs = 250;

    stopEffect();
    _effect = EFFECT_SHUFFLE;
    _effectStartMs = millis();
    _effectDurationMs = durationMs;
    _nextFrameMs = 0;
    _effectResult = finalNumber;
    _effectLeadingZeros = leadingZeros;

    for (uint8_t i = 0; i < DIGIT_COUNT; ++i) {
        _shuffleDigits[i] = static_cast<uint8_t>(random(0, 10));
        _shuffleStopped[i] = false;
    }
}

void TimeDisplayLibrary::shuffleRandom(uint32_t durationMs, bool leadingZeros) {
    shuffleTo(static_cast<uint16_t>(random(0, 10000)), durationMs, leadingZeros);
}

void TimeDisplayLibrary::startFlicker(uint32_t durationMs) {
    if (durationMs == 0) return;
    stopEffect();
    _effect = EFFECT_FLICKER;
    _effectStartMs = millis();
    _effectDurationMs = durationMs;
    _nextFrameMs = 0;
}

void TimeDisplayLibrary::startFlash(uint8_t red, uint8_t green, uint8_t blue,
                                    uint8_t flashes, uint16_t intervalMs) {
    if (flashes == 0) return;
    if (intervalMs < 20) intervalMs = 20;

    stopEffect();
    _effect = EFFECT_FLASH;
    _effectStartMs = millis();
    _nextFrameMs = 0;
    _flashColor = _time_display.Color(red, green, blue);
    _flashCount = flashes;
    _flashIntervalMs = intervalMs;
    _flashTransitions = 0;
    _flashOn = false;
}

bool TimeDisplayLibrary::isEffectRunning() const {
    return _effect != EFFECT_NONE;
}

uint16_t TimeDisplayLibrary::getEffectResult() const {
    return _effectResult;
}

void TimeDisplayLibrary::stopEffect() {
    if (_effect == EFFECT_NONE) return;
    _effect = EFFECT_NONE;
    _time_display.setBrightness(_brightness);
    drawStoredNumber();
}

void TimeDisplayLibrary::finishEffect() {
    _effect = EFFECT_NONE;
    _time_display.setBrightness(_brightness);
    drawStoredNumber();
}

void TimeDisplayLibrary::update() {
    if (_effect == EFFECT_NONE) return;

    const uint32_t now = millis();
    switch (_effect) {
        case EFFECT_ELECTRIC_BUZZ: updateElectricBuzz(now); break;
        case EFFECT_SHUFFLE:       updateShuffle(now); break;
        case EFFECT_FLICKER:       updateFlicker(now); break;
        case EFFECT_FLASH:         updateFlash(now); break;
        default:                   _effect = EFFECT_NONE; break;
    }
}

void TimeDisplayLibrary::updateElectricBuzz(uint32_t now) {
    if (now - _effectStartMs >= _effectDurationMs) {
        finishEffect();
        return;
    }
    if (now < _nextFrameMs) return;

    _nextFrameMs = now + static_cast<uint32_t>(random(18, 70));

    // Preserve the current number shape while making it look electrically unstable.
    const uint8_t choice = static_cast<uint8_t>(random(0, 10));
    if (choice == 0) {
        _time_display.clear(); // occasional arc dropout
        _time_display.show();
        return;
    }

    const uint32_t savedColor = _color;
    if (choice <= 4) {
        _color = _time_display.Color(30, 120, 255);   // electric blue
    } else if (choice <= 7) {
        _color = _time_display.Color(150, 210, 255);  // icy blue-white
    } else {
        _color = _time_display.Color(255, 255, 255);  // hard white arc
    }

    if (_hasCurrentNumber) {
        drawNumberInternal(_currentNumber, _currentLeadingZeros, false);
    } else {
        // No stored number: flash the entire 28-pixel display like an electrical arc.
        for (uint16_t i = 0; i < _time_display.numPixels(); ++i) {
            _time_display.setPixelColor(i, _color);
        }
        _time_display.show();
    }
    _color = savedColor;
}

void TimeDisplayLibrary::updateShuffle(uint32_t now) {
    const uint32_t elapsed = now - _effectStartMs;
    if (elapsed >= _effectDurationMs) {
        _currentNumber = _effectResult;
        _currentLeadingZeros = _effectLeadingZeros;
        _hasCurrentNumber = true;
        finishEffect();
        return;
    }
    if (now < _nextFrameMs) return;

    // Starts fast and progressively slows as the result approaches.
    const uint32_t interval = 35 + (elapsed * 145UL / _effectDurationMs);
    _nextFrameMs = now + interval;

    uint8_t target[DIGIT_COUNT] = {
        static_cast<uint8_t>((_effectResult / 1000) % 10),
        static_cast<uint8_t>((_effectResult / 100) % 10),
        static_cast<uint8_t>((_effectResult / 10) % 10),
        static_cast<uint8_t>(_effectResult % 10)
    };

    // Digits lock from left to right during the final 40% of the effect.
    const uint32_t lockStart = (_effectDurationMs * 60UL) / 100UL;
    const uint32_t lockWindow = _effectDurationMs - lockStart;
    const uint32_t perDigit = lockWindow / DIGIT_COUNT;

    for (uint8_t i = 0; i < DIGIT_COUNT; ++i) {
        const uint32_t digitLockTime = lockStart + perDigit * (i + 1);
        if (elapsed >= digitLockTime) {
            _shuffleDigits[i] = target[i];
            _shuffleStopped[i] = true;
        } else if (!_shuffleStopped[i]) {
            uint8_t nextDigit = static_cast<uint8_t>(random(0, 10));
            if (nextDigit == _shuffleDigits[i]) nextDigit = (nextDigit + 1) % 10;
            _shuffleDigits[i] = nextDigit;
        }
    }

    for (uint8_t i = 0; i < DIGIT_COUNT; ++i) {
        drawDigit(i, _shuffleDigits[i]);
    }
    _time_display.show();
}

void TimeDisplayLibrary::updateFlicker(uint32_t now) {
    if (now - _effectStartMs >= _effectDurationMs) {
        finishEffect();
        return;
    }
    if (now < _nextFrameMs) return;

    _nextFrameMs = now + static_cast<uint32_t>(random(25, 110));
    const uint8_t low = (_brightness > 80) ? 45 : 10;
    const uint8_t high = (_brightness > low) ? _brightness : static_cast<uint8_t>(low + 1);
    _time_display.setBrightness(static_cast<uint8_t>(random(low, static_cast<long>(high) + 1)));
    drawStoredNumber();
}

void TimeDisplayLibrary::updateFlash(uint32_t now) {
    if (now < _nextFrameMs) return;

    if (_flashTransitions >= static_cast<uint8_t>(_flashCount * 2)) {
        finishEffect();
        return;
    }

    _flashOn = !_flashOn;
    ++_flashTransitions;
    _nextFrameMs = now + _flashIntervalMs;

    if (_flashOn) {
        for (uint16_t i = 0; i < _time_display.numPixels(); ++i) {
            _time_display.setPixelColor(i, _flashColor);
        }
        _time_display.show();
    } else {
        drawStoredNumber();
    }
}

// Legacy v1.x calls.
void TimeDisplayLibrary::twentyfive()    { showNumber(25); }
void TimeDisplayLibrary::five_ten()      { showNumber(510); }
void TimeDisplayLibrary::zero()          { showNumber(0, true); }
void TimeDisplayLibrary::ninefourtwo()   { showNumber(942); }
void TimeDisplayLibrary::oneeightfourtwo(){ showNumber(1842); }
