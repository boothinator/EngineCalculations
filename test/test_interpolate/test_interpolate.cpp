// Test template
// Copyright (C) 2021  Joshua Booth

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <Arduino.h>
#include <unity.h>

#include "EngineCalculations.h"

#define MAX_MESSAGE_LEN 255

char message[MAX_MESSAGE_LEN];

void setUp(void) {
}

void tearDown(void) {
// clean stuff up here
}

#ifdef __AVR_ATmega2560__

uint16_t start, end;

#define TIME_START {start = TCNT1;}
#define TIME_END {end = TCNT1;}
#define TIME_DIFF (end - start)

#else

#define TIME_START
#define TIME_END
#define TIME_DIFF (0)

#endif

void test_interpolateBilinearTable()
{
  const uint8_t xScale[] = {63, 127, 191};
  const uint8_t yScale[] = {63, 127, 191};
  const uint8_t zValues[] = {254, 127, 254,  //  63
                             127,   0, 127,  // 127
                             254, 127, 254}; // 191
  
  const size_t xLength = sizeof(xScale) / sizeof(xScale[0]);
  const size_t yLength = sizeof(yScale) / sizeof(yScale[0]);

  uint8_t x;
  uint8_t y;
  float expected, actual;
  
  x = 127;
  y = 127;
  expected = 0;
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);

  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Exact");

  x = 126;
  y = 127;
  expected = 1.984375;
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center x below");

  x = 128;
  y = 127;
  expected = 1.984375;
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center x above");

  x = 127;
  y = 126;
  expected = 1.984375;
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center y below");

  x = 127;
  y = 128;
  expected = 1.984375;
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center y above");

  x = 126;
  y = 126;
  expected = 3.96875;
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center x below, y below");

  x = 128;
  y = 126;
  expected = 3.96875;
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center x above y below");

  x = 126;
  y = 128;
  expected = 3.96875;
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center x below y above");

  x = 128;
  y = 128;
  expected = 3.96875;
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center x above y above");
}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(200);

  UNITY_BEGIN();    // IMPORTANT LINE!

#ifdef __AVR_ATmega2560__
  // Set Timer 1 to run in normal counting mode (no PWM, rollover to 0)
  TCCR1A = 0;
  // Set Timer 1 to use no prescaling (run in sync with system clock)
  TCCR1B = 1;
#endif

  RUN_TEST(test_interpolateBilinearTable);

  UNITY_END(); // stop unit testing
}

void loop() {
}

