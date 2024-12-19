# TimeDisplayLibrary
A library for controlling adafruit NeoPixel (x4) seven segment time displays









#include <Adafruit_NeoPixel.h>
#include <TimeDisplayLibrary.h>

// Pin and number of NeoPixels
#define PIN 6
#define NUMPIXELS 30

Adafruit_NeoPixel time_display(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); // Declare and initialize the NeoPixel object

// Create an instance of your library
TimeDisplayLibrary displayLibrary(time_display);

void setup() {
    time_display.begin(); // Initialize the NeoPixel strip
    time_display.show();  // Turn off all pixels
    displayLibrary.twentyfive(); // Example function call
}

void loop() {
    // Add your logic here
}
