#include "TimeDisplayLibrary.h"

namespace {

constexpr uint8_t BLANK_DIGIT = 0xFF;

// Seven-segment bit order: A, B, C, D, E, F, G.
constexpr uint8_t DIGIT_SEGMENTS[10] = {
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

// Original 28-pixel masks preserved exactly from the first library version.
constexpr uint8_t PATTERN_TWENTY_FIVE[] = {
    1, 2, 4, 5, 6, 7, 8, 10, 11, 13, 14, 15, 16, 17, 18, 19, 21, 22, 24, 25, 27
};

constexpr uint8_t PATTERN_FIVE_TEN[] = {
    0, 1, 2, 3, 4, 5, 7, 12, 14, 15, 16, 17, 18, 19, 22, 23, 25, 26, 27
};

constexpr uint8_t PATTERN_ZERO[] = {
    0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 14, 15, 16, 17, 18, 19,
    21, 22, 23, 24, 25, 26
};

constexpr uint8_t PATTERN_NINE_FOUR_TWO[] = {
    0, 1, 3, 4, 6, 7, 9, 12, 13, 14, 15, 16, 18, 19, 20, 21, 26
};

constexpr uint8_t PATTERN_ONE_EIGHT_FOUR_TWO[] = {
    0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
    19, 20, 21, 26
};

template <size_t N>
constexpr size_t patternSize(const uint8_t (&)[N]) {
    return N;
}

} // namespace

TimeDisplayLibrary::TimeDisplayLibrary(Adafruit_NeoPixel &display)
    : _display(display),
      _color(display.Color(255, 0, 0)),
      _reverseDigitOrder(false) {
}

void TimeDisplayLibrary::setColor(uint8_t red, uint8_t green, uint8_t blue) {
    _color = _display.Color(red, green, blue);
}

void TimeDisplayLibrary::setColor(uint32_t packedColor) {
    _color = packedColor;
}

uint32_t TimeDisplayLibrary::getColor() const {
    return _color;
}

bool TimeDisplayLibrary::showNumber(uint16_t value, bool leadingZeroes) {
    if (!hasValidPixelCount() || value > 9999) {
        return false;
    }

    const uint8_t digits[DIGIT_COUNT] = {
        static_cast<uint8_t>((value / 1000) % 10),
        static_cast<uint8_t>((value / 100) % 10),
        static_cast<uint8_t>((value / 10) % 10),
        static_cast<uint8_t>(value % 10)
    };

    renderDigits(digits, leadingZeroes);
    return true;
}

bool TimeDisplayLibrary::showTime(uint16_t hhmm) {
    if (hhmm > 2359) {
        return false;
    }

    const uint8_t hours = static_cast<uint8_t>(hhmm / 100);
    const uint8_t minutes = static_cast<uint8_t>(hhmm % 100);
    return showTime(hours, minutes);
}

bool TimeDisplayLibrary::showTime(uint8_t hours, uint8_t minutes) {
    if (hours > 23 || minutes > 59 || !hasValidPixelCount()) {
        return false;
    }

    const uint8_t digits[DIGIT_COUNT] = {
        static_cast<uint8_t>(hours / 10),
        static_cast<uint8_t>(hours % 10),
        static_cast<uint8_t>(minutes / 10),
        static_cast<uint8_t>(minutes % 10)
    };

    renderDigits(digits, true);
    return true;
}

bool TimeDisplayLibrary::showDigits(
    uint8_t digit1,
    uint8_t digit2,
    uint8_t digit3,
    uint8_t digit4
) {
    if (!hasValidPixelCount() || digit1 > 9 || digit2 > 9 || digit3 > 9 || digit4 > 9) {
        return false;
    }

    const uint8_t digits[DIGIT_COUNT] = {digit1, digit2, digit3, digit4};
    renderDigits(digits, true);
    return true;
}

void TimeDisplayLibrary::setReverseDigitOrder(bool reversed) {
    _reverseDigitOrder = reversed;
}

bool TimeDisplayLibrary::getReverseDigitOrder() const {
    return _reverseDigitOrder;
}

void TimeDisplayLibrary::clear() {
    _display.clear();
    _display.show();
}

bool TimeDisplayLibrary::hasValidPixelCount() const {
    return _display.numPixels() >= REQUIRED_PIXEL_COUNT;
}

void TimeDisplayLibrary::renderDigits(const uint8_t digits[DIGIT_COUNT], bool leadingZeroes) {
    _display.clear();

    bool nonZeroDigitSeen = leadingZeroes;
    for (uint8_t position = 0; position < DIGIT_COUNT; ++position) {
        uint8_t digit = digits[position];

        if (!leadingZeroes && position < DIGIT_COUNT - 1 && digit == 0 && !nonZeroDigitSeen) {
            digit = BLANK_DIGIT;
        } else {
            nonZeroDigitSeen = true;
        }

        renderDigit(position, digit);
    }

    _display.show();
}

void TimeDisplayLibrary::renderDigit(uint8_t logicalPosition, uint8_t digit) {
    if (digit == BLANK_DIGIT || digit > 9) {
        return;
    }

    const uint8_t physicalPosition = _reverseDigitOrder
        ? static_cast<uint8_t>((DIGIT_COUNT - 1) - logicalPosition)
        : logicalPosition;

    const uint16_t firstPixel = physicalPosition * SEGMENTS_PER_DIGIT;
    const uint8_t segmentMask = DIGIT_SEGMENTS[digit];

    for (uint8_t segment = 0; segment < SEGMENTS_PER_DIGIT; ++segment) {
        if (segmentMask & (1U << segment)) {
            _display.setPixelColor(firstPixel + segment, _color);
        }
    }
}

void TimeDisplayLibrary::renderPattern(const uint8_t *pixelIndexes, size_t pixelCount) {
    _display.clear();

    const uint16_t availablePixels = _display.numPixels();
    for (size_t index = 0; index < pixelCount; ++index) {
        const uint8_t pixel = pixelIndexes[index];
        if (pixel < availablePixels) {
            _display.setPixelColor(pixel, _color);
        }
    }

    _display.show();
}

void TimeDisplayLibrary::displayTwentyFive() {
    renderPattern(PATTERN_TWENTY_FIVE, patternSize(PATTERN_TWENTY_FIVE));
}

void TimeDisplayLibrary::displayFiveTen() {
    renderPattern(PATTERN_FIVE_TEN, patternSize(PATTERN_FIVE_TEN));
}

void TimeDisplayLibrary::displayZero() {
    renderPattern(PATTERN_ZERO, patternSize(PATTERN_ZERO));
}

void TimeDisplayLibrary::displayNineFourTwo() {
    renderPattern(PATTERN_NINE_FOUR_TWO, patternSize(PATTERN_NINE_FOUR_TWO));
}

void TimeDisplayLibrary::displayOneEightFourTwo() {
    renderPattern(PATTERN_ONE_EIGHT_FOUR_TWO, patternSize(PATTERN_ONE_EIGHT_FOUR_TWO));
}

void TimeDisplayLibrary::twentyfive() {
    displayTwentyFive();
}

void TimeDisplayLibrary::five_ten() {
    displayFiveTen();
}

void TimeDisplayLibrary::zero() {
    displayZero();
}

void TimeDisplayLibrary::ninefourtwo() {
    displayNineFourTwo();
}

void TimeDisplayLibrary::oneeightfourtwo() {
    displayOneEightFourTwo();
}
