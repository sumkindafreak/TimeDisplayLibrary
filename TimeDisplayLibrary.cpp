#include "TimeDisplayLibrary.h"

namespace {

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
      _color(display.Color(255, 0, 0)) {
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

void TimeDisplayLibrary::clear() {
    _display.clear();
    _display.show();
}

bool TimeDisplayLibrary::hasValidPixelCount() const {
    return _display.numPixels() >= REQUIRED_PIXEL_COUNT;
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

// -----------------------------------------------------------------------------
// Legacy API aliases
// -----------------------------------------------------------------------------

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
