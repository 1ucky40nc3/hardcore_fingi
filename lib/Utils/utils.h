#pragma once
#ifndef UNIT_TEST    // Or a specific macro defined for your native test env
#include <WString.h> // For Arduino compilation
#else
#include <ArduinoFake.h> // For native compilation
#endif

String formatFloat(float value);