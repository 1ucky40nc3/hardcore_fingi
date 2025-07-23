#ifndef UNIT_TEST    // Or a specific macro defined for your native test env
#include <WString.h> // For Arduino compilation
#include <stdlib_noniso.h>
#else
#include <ArduinoFake.h> // For native compilation
#include <stdlib.h>

#endif

String formatFloat(float value)
{
    // We normalize between [-999.9,999.9] to avoid measurements outside of the display range
    // The display output should have a max of 3 chars before the decimal point and one after
    // Note: The normalization is also in the max contraints of the load cell of about [-200.0,200.0]
    float normalizedValue = value;
    if (normalizedValue > 999.9)
        normalizedValue = 999.9;
    else if (normalizedValue < -999.9)
    {
        normalizedValue = -999.9;
    }

    // Buffer size of 7 bytes to hold 5 characters + null terminator + optional negtive sign
    char buffer[7];
    // Allow more width for negative numbers
    signed char width = 5;
    if (normalizedValue < 0)
    {
        width = 6;
    }
    dtostrf(normalizedValue, width, 1, buffer);
    String formattedString = String(buffer);
    return buffer;
}