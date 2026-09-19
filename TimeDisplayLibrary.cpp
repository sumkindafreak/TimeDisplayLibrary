#include "TimeDisplayLibrary.h"

// Seven-segment bit order:
// bit 0=A (top), bit 1=B (upper-right), bit 2=C (lower-right),
// bit 3=D (bottom), bit 4=E (lower-left), bit 5=F (upper-left),
// bit 6=G (middle).
//
// The original TimeDisplayLibrary patterns reveal that each digit occupies
// seven consecutive NeoPixels. The physical segment order within each group
// is G, F, E, D, C, B, A, so the lookup below converts normal seven-segment
// digits into the existing 28-pixel wiring.
static const uint8_t DIGIT_SEGMENTS[10] = {
    0b00111111, // 0: A B C D E F
    0b00000110, // 1: B C
    0b01011011, // 2: A B D E G
    0b01001111, // 3: A B C D G
    0b01100110, // 4: B C F G
    0b01101101, // 5: A C D F G
    0b01111101, // 6: A C D E F G
    0b00000111, // 7: A B C
    0b01111111, // 8: A B C D E F G
    0b01101111  // 9: A B C D F G
};

// Maps logical A..G segments to the pixel offset inside each seven-pixel digit.
// Existing hardware uses pixel offsets: A=6, B=5, C=4, D=3, E=2, F=1, G=0.
static const uint8_t SEGMENT_TO_PIXEL[7] = {6, 5, 4, 3, 2, 1, 0};

TimeDisplayLibrary::TimeDisplayLibrary(Adafruit_NeoPixel &display)
    : _time_display(display), _color(display.Color(255, 0, 0)) {
}

void TimeDisplayLibrary::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    _color = _time_display.Color(red, green, blue);
}

void TimeDisplayLibrary::setColor(uint32_t color) {
    _color = color;
}

void TimeDisplayLibrary::setBrightness(uint8_t brightness) {
    _time_display.setBrightness(brightness);
    _time_display.show();
}

void TimeDisplayLibrary::clearDigit(uint8_t position) {
    if (position >= DIGIT_COUNT) {
        return;
    }

    const uint16_t firstPixel = position * SEGMENTS_PER_DIGIT;
    for (uint8_t i = 0; i < SEGMENTS_PER_DIGIT; ++i) {
        _time_display.setPixelColor(firstPixel + i, 0);
    }
}

void TimeDisplayLibrary::drawDigit(uint8_t position, uint8_t digit) {
    if (position >= DIGIT_COUNT || digit > 9) {
        return;
    }

    clearDigit(position);

    const uint16_t firstPixel = position * SEGMENTS_PER_DIGIT;
    const uint8_t segments = DIGIT_SEGMENTS[digit];

    for (uint8_t segment = 0; segment < 7; ++segment) {
        if (segments & (1U << segment)) {
            _time_display.setPixelColor(
                firstPixel + SEGMENT_TO_PIXEL[segment],
                _color
            );
        }
    }
}

bool TimeDisplayLibrary::showDigit(uint8_t position, uint8_t digit) {
    if (position >= DIGIT_COUNT || digit > 9) {
        return false;
    }

    drawDigit(position, digit);
    _time_display.show();
    return true;
}

bool TimeDisplayLibrary::showNumber(uint16_t number, bool leadingZeros) {
    if (number > 9999) {
        return false;
    }

    uint8_t digits[DIGIT_COUNT] = {
        static_cast<uint8_t>((number / 1000) % 10),
        static_cast<uint8_t>((number / 100) % 10),
        static_cast<uint8_t>((number / 10) % 10),
        static_cast<uint8_t>(number % 10)
    };

    bool started = leadingZeros;

    for (uint8_t position = 0; position < DIGIT_COUNT; ++position) {
        // Always draw the final digit, so zero is displayed as "0".
        if (!started && digits[position] == 0 && position < DIGIT_COUNT - 1) {
            clearDigit(position);
        } else {
            started = true;
            drawDigit(position, digits[position]);
        }
    }

    _time_display.show();
    return true;
}

void TimeDisplayLibrary::clear() {
    _time_display.clear();
    _time_display.show();
}

// -----------------------------------------------------------------------------
// Legacy v1.x calls
// -----------------------------------------------------------------------------

void TimeDisplayLibrary::twentyfive() {
    showNumber(25);
}

void TimeDisplayLibrary::five_ten() {
    showNumber(510);
}

void TimeDisplayLibrary::zero() {
    showNumber(0, true);
}

void TimeDisplayLibrary::ninefourtwo() {
    showNumber(942);
}

void TimeDisplayLibrary::oneeightfourtwo() {
    showNumber(1842);
}
