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

#define TIME_START {noInterrupts(); start = TCNT1;}
#define TIME_END {end = TCNT1; interrupts();}
#define TIME_DIFF (end - start)

#else

#define TIME_START
#define TIME_END
#define TIME_DIFF (0)

#endif

void test_interpolateBilinearXFirstYFirst()
{
  volatile uint8_t x, x0, x1;
  volatile uint8_t y, y0, y1;
  volatile uint8_t z00, z10;
  volatile uint8_t z01, z11;
  uint64_t expected;
  volatile uint64_t actual;

  x0 = 0;
  x1 = 255;
  y0 = 0;
  y1 = 255;

  z00 = 0;
  z10 = 255;
  z01 = 127;
  z11 = 127;

  x = 127;
  y = 127;
  expected = 127;

  TIME_START
  actual = interpolateBilinearXFirst<uint16_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "X First");
  TEST_ASSERT_UINT16_WITHIN(10, 910, TIME_DIFF);

  TIME_START
  actual = static_cast<uint64_t>(interpolateBilinearXFirst<float, float, float>(x, x0, x1, y, y0, y1, z00, z10, z01, z11) + 0.5);
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "X First Float");
  TEST_ASSERT_UINT16_WITHIN(10, 2555, TIME_DIFF);

  TIME_START
  actual = interpolateBilinearYFirst<uint16_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "Y First");
  TEST_ASSERT_UINT16_WITHIN(10, 910, TIME_DIFF);

  //actual = interpolateBilinearXFirst<uint16_t, uint8_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  //TEST_ASSERT_NOT_EQUAL_MESSAGE(expected, actual, "X First DeltaYMulZ should be too small for result");
  //actual = interpolateBilinearYFirst<uint8_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  //TEST_ASSERT_NOT_EQUAL_MESSAGE(expected, actual, "Y First DeltaXMulZ should be too small for result");

  x = 63;
  y = 127;
  expected = 95;

  TIME_START
  actual = interpolateBilinearXFirst<uint16_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "X First");
  TEST_ASSERT_UINT16_WITHIN(10, 910, TIME_DIFF);
  
  TIME_START
  actual = static_cast<uint64_t>(interpolateBilinearXFirst<float, float, float>(x, x0, x1, y, y0, y1, z00, z10, z01, z11) + 0.5);
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "X First Float");
  TEST_ASSERT_UINT16_WITHIN(10, 2623, TIME_DIFF);
  
  TIME_START
  actual = interpolateBilinearYFirst<uint16_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "Y First");
  TEST_ASSERT_UINT16_WITHIN(10, 910, TIME_DIFF);
  
  //actual = interpolateBilinearXFirst<uint16_t, uint8_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  //TEST_ASSERT_NOT_EQUAL_MESSAGE(expected, actual, "X First DeltaYMulZ should be too small for result");
  
  //actual = interpolateBilinearYFirst<uint8_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  //TEST_ASSERT_NOT_EQUAL_MESSAGE(expected, actual, "Y First DeltaXMulZ should be too small for result");

  x = 63;
  y = 63;
  expected = 79;

  TIME_START
  actual = interpolateBilinearXFirst<uint16_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "X First");
  TEST_ASSERT_UINT16_WITHIN(10, 910, TIME_DIFF);

  TIME_START
  actual = static_cast<uint64_t>(interpolateBilinearXFirst<float, float, float>(x, x0, x1, y, y0, y1, z00, z10, z01, z11) + 0.5);
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "X First Float");
  TEST_ASSERT_UINT16_WITHIN(10, 2597, TIME_DIFF);

  TIME_START
  actual = interpolateBilinearYFirst<uint16_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "Y First");
  TEST_ASSERT_UINT16_WITHIN(10, 910, TIME_DIFF);

  //actual = interpolateBilinearXFirst<uint16_t, uint8_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  //TEST_ASSERT_NOT_EQUAL_MESSAGE(expected, actual, "X First DeltaYMulZ should be too small for result");
  //actual = interpolateBilinearYFirst<uint8_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  //TEST_ASSERT_NOT_EQUAL_MESSAGE(expected, actual, "Y First DeltaXMulZ should be too small for result");
}

template<typename X, typename Y, typename Z, int expectedTicks, int ticksWithin,
  Z (*intBilin)(X, X, X, Y, Y, Y, Z, Z, Z, Z) = interpolateBilinear<X, Y, Z>>
void test_interpolateBilinear()
{
  volatile X x, x0, x1;
  volatile Y y, y0, y1;
  volatile Z z00, z10;
  volatile Z z01, z11;
  Z expected;
  volatile Z actual;

  x0 = 0;
  x1 = 255;
  y0 = 0;
  y1 = 255;

  z00 = 0;
  z10 = 255;
  z01 = 127;
  z11 = 127;

  x = 127;
  y = 127;
  expected = 127;

  TIME_START
  actual = intBilin(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TIME_END
  TEST_ASSERT_EQUAL(expected, actual);
  TEST_ASSERT_UINT16_WITHIN(ticksWithin, expectedTicks, TIME_DIFF);

  x = 63;
  y = 127;
  expected = 95;

  TIME_START
  actual = intBilin(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TIME_END
  TEST_ASSERT_EQUAL(expected, actual);
  TEST_ASSERT_UINT16_WITHIN(ticksWithin, expectedTicks, TIME_DIFF);

  x = 63;
  y = 63;
  expected = 79;

  TIME_START
  actual = intBilin(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TIME_END
  TEST_ASSERT_EQUAL(expected, actual);
  TEST_ASSERT_UINT16_WITHIN(ticksWithin, expectedTicks, TIME_DIFF);
}

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
  uint8_t expected, actual;
  
  x = 127;
  y = 127;
  expected = 0;
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);

  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Exact");

  x = 126;
  y = 127;
  expected = 2; // 1.984375
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center x below");
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center x below");

  x = 128;
  y = 127;
  expected = 2; // 1.984375
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center x above");

  x = 127;
  y = 126;
  expected = 2; // 1.984375
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center y below");

  x = 127;
  y = 128;
  expected = 2; // 1.984375
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center y above");

  x = 126;
  y = 126;
  expected = 4; // 3.96875
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center x below, y below");

  x = 128;
  y = 126;
  expected = 4; // 3.96875
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center x above y below");

  x = 126;
  y = 128;
  expected = 4; // 3.96875
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center x below y above");

  x = 128;
  y = 128;
  expected = 4; // 3.96875
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate around center x above y above");

  x = 60;
  y = 127;
  expected = 127;
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate off scale x low");

  x = 200;
  y = 127;
  expected = 127;
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate off scale x high");

  x = 127;
  y = 60;
  expected = 127;
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate off scale y low");

  x = 127;
  y = 200;
  expected = 127;
  actual = interpolateBilinearTable<uint8_t>(x, y, xLength, yLength, xScale, yScale, zValues);
  TEST_ASSERT_EQUAL_FLOAT_MESSAGE(expected, actual, "Interpolate off scale y high");
}

template<typename X, typename Y, typename Z>
Z interpolateBilinearFloat(X x, X x0, X x1, Y y, Y y0, Y y1, Z z00, Z z10, Z z01, Z z11)
{
  return static_cast<Z>(interpolateBilinearXFirst<float, float, float>(x, x0, x1, y, y0, y1, z00, z10, z01, z11) + 0.5);
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

  RUN_TEST(test_interpolateBilinearXFirstYFirst);
  RUN_TEST((test_interpolateBilinear<uint8_t, uint8_t, uint8_t, 895, 10>));
  RUN_TEST((test_interpolateBilinear<uint8_t, uint8_t, uint8_t, 2552, 100, interpolateBilinearFloat>));

  RUN_TEST((test_interpolateBilinear<uint8_t, uint8_t, uint16_t, 1030, 10>));
  RUN_TEST((test_interpolateBilinear<uint8_t, uint8_t, uint16_t, 2573, 100, interpolateBilinearFloat>));

  RUN_TEST((test_interpolateBilinear<uint8_t, uint16_t, uint8_t, 808, 30>));
  RUN_TEST((test_interpolateBilinear<uint8_t, uint16_t, uint8_t, 2545, 100, interpolateBilinearFloat>));

  RUN_TEST((test_interpolateBilinear<uint8_t, uint16_t, uint16_t, 1435, 30>));
  RUN_TEST((test_interpolateBilinear<uint8_t, uint16_t, uint16_t, 2502, 100, interpolateBilinearFloat>));

  RUN_TEST((test_interpolateBilinear<uint16_t, uint8_t, uint8_t, 800, 10>));
  RUN_TEST((test_interpolateBilinear<uint16_t, uint8_t, uint8_t, 2484, 100, interpolateBilinearFloat>));

  RUN_TEST((test_interpolateBilinear<uint16_t, uint8_t, uint16_t, 1572, 30>));
  RUN_TEST((test_interpolateBilinear<uint16_t, uint8_t, uint16_t, 2508, 100, interpolateBilinearFloat>));

  RUN_TEST((test_interpolateBilinear<uint16_t, uint16_t, uint8_t, 1522, 30>));
  RUN_TEST((test_interpolateBilinear<uint16_t, uint16_t, uint8_t, 2480, 100, interpolateBilinearFloat>));

  RUN_TEST((test_interpolateBilinear<uint16_t, uint16_t, uint16_t, 1534, 30>));
  RUN_TEST((test_interpolateBilinear<uint16_t, uint16_t, uint16_t, 2502, 100, interpolateBilinearFloat>));
  RUN_TEST(test_interpolateBilinearTable);

  UNITY_END(); // stop unit testing
}

void loop() {
}

