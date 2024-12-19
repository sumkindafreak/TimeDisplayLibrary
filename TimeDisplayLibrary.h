#ifndef TIMEDISPLAYLIBRARY_H
#define TIMEDISPLAYLIBRARY_H

#include <Adafruit_NeoPixel.h> // Include the NeoPixel library

class TimeDisplayLibrary {
public:
    TimeDisplayLibrary(Adafruit_NeoPixel &display); // Constructor
    void twentyfive();
    void five_ten();
    void zero();
    void ninefourtwo();
    void oneeightfourtwo();
    void clear();

private:
    Adafruit_NeoPixel &_time_display; // Reference to the NeoPixel object
};

#endif