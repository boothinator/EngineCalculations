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

void test_interpolateLinearFloat()
{
  volatile uint8_t input;
  volatile uint8_t inputLow;
  volatile uint8_t inputHigh;
  volatile uint8_t outputLow;
  volatile uint8_t outputHigh;

  float expected;
  volatile float actual;

  inputLow = 0;
  inputHigh = 127;
  outputLow = 0;
  outputHigh = 190;

  input = 63;
  expected = 94.25197;
  TIME_START
  actual = interpolateLinear<uint8_t, uint8_t, float>(input, inputLow, inputHigh, outputLow, outputHigh);
  TIME_END
  TEST_ASSERT_EQUAL_FLOAT(expected, actual);
  TEST_ASSERT_UINT16_WITHIN(10, 994, TIME_DIFF);

  inputLow = 0;
  inputHigh = 127;
  outputLow = 190;
  outputHigh = 0;

  input = 64;
  expected = 94.25197;
  TIME_START
  actual = interpolateLinear<uint8_t, uint8_t, float>(input, inputLow, inputHigh, outputLow, outputHigh);
  TIME_END
  TEST_ASSERT_EQUAL_FLOAT(expected, actual);
  TEST_ASSERT_UINT16_WITHIN(10, 1043, TIME_DIFF);

  inputLow = 0;
  inputHigh = 127;
  outputLow = 0;
  outputHigh = 190;

  input = 60;
  expected = 89.76378;
  TIME_START
  actual = interpolateLinear<uint8_t, uint8_t, float>(input, inputLow, inputHigh, outputLow, outputHigh);
  TIME_END
  TEST_ASSERT_EQUAL_FLOAT(expected, actual);
  TEST_ASSERT_UINT16_WITHIN(10, 994, TIME_DIFF);

  inputLow = 0;
  inputHigh = 127;
  outputLow = 190;
  outputHigh = 0;

  input = 67;
  expected = 89.76378;
  TIME_START
  actual = interpolateLinear<uint8_t, uint8_t, float>(input, inputLow, inputHigh, outputLow, outputHigh);
  TIME_END
  TEST_ASSERT_EQUAL_FLOAT(expected, actual);
  TEST_ASSERT_UINT16_WITHIN(10, 1043, TIME_DIFF);
}

template<typename InputType, typename OutputType, int expectedTicks, int ticksWithin,
  OutputType (*intLin)(InputType, InputType, InputType, OutputType, OutputType) = interpolateLinear<InputType, OutputType>>
void test_interpolateLinear()
{
  volatile InputType input;
  volatile InputType inputLow;
  volatile InputType inputHigh;
  volatile OutputType outputLow;
  volatile OutputType outputHigh;

  OutputType expected;
  volatile OutputType actual;

  inputLow = 0;
  inputHigh = 127;
  outputLow = 0;
  outputHigh = 190;

  input = 63;
  expected = 94; // 94.25197

  TIME_START
  actual = intLin(input, inputLow, inputHigh, outputLow, outputHigh);
  TIME_END
  TEST_ASSERT_EQUAL(expected, actual);
  TEST_ASSERT_UINT16_WITHIN(ticksWithin, expectedTicks, TIME_DIFF);

  inputLow = 0;
  inputHigh = 127;
  outputLow = 190;
  outputHigh = 0;

  input = 64;
  expected = 94; // 94.25197
  TIME_START
  actual = intLin(input, inputLow, inputHigh, outputLow, outputHigh);
  TIME_END
  TEST_ASSERT_EQUAL(expected, actual);
  TEST_ASSERT_UINT16_WITHIN(ticksWithin, expectedTicks, TIME_DIFF);

  inputLow = 0;
  inputHigh = 127;
  outputLow = 0;
  outputHigh = 190;

  input = 60;
  expected = 90; // 89.76378

  TIME_START
  actual = intLin(input, inputLow, inputHigh, outputLow, outputHigh);
  TIME_END
  TEST_ASSERT_EQUAL(expected, actual);
  TEST_ASSERT_UINT16_WITHIN(ticksWithin, expectedTicks, TIME_DIFF);

  inputLow = 0;
  inputHigh = 127;
  outputLow = 190;
  outputHigh = 0;

  input = 67;
  expected = 90; // 89.76378
  TIME_START
  actual = intLin(input, inputLow, inputHigh, outputLow, outputHigh);
  TIME_END
  TEST_ASSERT_EQUAL(expected, actual);
  TEST_ASSERT_UINT16_WITHIN(ticksWithin, expectedTicks, TIME_DIFF);
}

void test_interpolateLinearTable()
{
  uint8_t input;
  const uint8_t inputScale[] = {0, 127, 255};
  const uint8_t outputArray[] = {0, 190, 255};

  const size_t length = sizeof(inputScale) / sizeof(inputScale[0]);

  uint32_t expected;
  uint32_t actual;

  input = 63;
  expected = 94;
  actual = interpolateLinearTable(input, length, inputScale, outputArray);
  TEST_ASSERT_EQUAL(expected, actual);
}

void test_interpolateBilinear()
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
  actual = interpolateBilinearXFirst<uint64_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "X First");
  TEST_ASSERT_UINT16_WITHIN(10, 944, TIME_DIFF);

  TIME_START
  actual = static_cast<uint64_t>(interpolateBilinearXFirst(x, x0, x1, y, y0, y1, z00, z10, z01, z11));
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "X First Float");
  TEST_ASSERT_UINT16_WITHIN(10, 2407, TIME_DIFF);

  TIME_START
  actual = interpolateBilinearYFirst<uint64_t, uint32_t, uint16_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "Y First");
  TEST_ASSERT_UINT16_WITHIN(10, 946, TIME_DIFF);

  actual = interpolateBilinearXFirst<uint64_t, uint32_t, uint16_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TEST_ASSERT_NOT_EQUAL_MESSAGE(expected, actual, "X First DeltaYMulZ should be too small for result");
  actual = interpolateBilinearYFirst<uint64_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TEST_ASSERT_NOT_EQUAL_MESSAGE(expected, actual, "Y First DeltaXMulZ should be too small for result");

  x = 63;
  y = 127;
  expected = 94;

  TIME_START
  actual = interpolateBilinearXFirst<uint64_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "X First");
  TEST_ASSERT_UINT16_WITHIN(10, 928, TIME_DIFF);
  
  TIME_START
  actual = static_cast<uint64_t>(interpolateBilinearXFirst(x, x0, x1, y, y0, y1, z00, z10, z01, z11));
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "X First Float");
  TEST_ASSERT_UINT16_WITHIN(10, 2475, TIME_DIFF);
  
  TIME_START
  actual = interpolateBilinearYFirst<uint64_t, uint32_t, uint16_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "Y First");
  TEST_ASSERT_UINT16_WITHIN(10, 928, TIME_DIFF);
  
  actual = interpolateBilinearXFirst<uint64_t, uint32_t, uint16_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TEST_ASSERT_NOT_EQUAL_MESSAGE(expected, actual, "X First DeltaYMulZ should be too small for result");
  
  actual = interpolateBilinearYFirst<uint64_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TEST_ASSERT_NOT_EQUAL_MESSAGE(expected, actual, "Y First DeltaXMulZ should be too small for result");

  x = 63;
  y = 63;
  expected = 78;

  TIME_START
  actual = interpolateBilinearXFirst<uint64_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "X First");
  TEST_ASSERT_UINT16_WITHIN(10, 920, TIME_DIFF);

  TIME_START
  actual = static_cast<uint64_t>(interpolateBilinearXFirst(x, x0, x1, y, y0, y1, z00, z10, z01, z11));
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "X First Float");
  TEST_ASSERT_UINT16_WITHIN(10, 2449, TIME_DIFF);

  TIME_START
  actual = interpolateBilinearYFirst<uint64_t, uint32_t, uint16_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TIME_END
  TEST_ASSERT_EQUAL_MESSAGE(expected, actual, "Y First");
  TEST_ASSERT_UINT16_WITHIN(10, 920, TIME_DIFF);

  actual = interpolateBilinearXFirst<uint64_t, uint32_t, uint16_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TEST_ASSERT_NOT_EQUAL_MESSAGE(expected, actual, "X First DeltaYMulZ should be too small for result");
  actual = interpolateBilinearYFirst<uint64_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  TEST_ASSERT_NOT_EQUAL_MESSAGE(expected, actual, "Y First DeltaXMulZ should be too small for result");
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

template<typename InputType, typename OutputType, typename SlopeType>
OutputType interpolateLinearReturnOutputType(InputType input, InputType inputLow, InputType inputHigh, OutputType output0, OutputType output1)
{
  return static_cast<OutputType>(interpolateLinearUnsigned<InputType, OutputType, SlopeType>(input, inputLow, inputHigh, output0, output1) + 0.5);
}

template<typename InputType, typename OutputType, typename SlopeType, uint8_t slopeShift>
OutputType interpolateLinearFixedReturnOutputType(InputType input, InputType inputLow, InputType inputHigh, OutputType output0, OutputType output1)
{
  return static_cast<OutputType>(interpolateLinearFixedUnsigned<SlopeType, slopeShift, InputType, OutputType>(input, inputLow, inputHigh, output0, output1));
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
  RUN_TEST(test_interpolateLinearFloat); //           ~1018
  RUN_TEST((test_interpolateLinear<uint8_t, uint8_t,   1259, 50, interpolateLinearReturnOutputType<uint8_t, uint8_t, float>>));
  RUN_TEST((test_interpolateLinear<uint8_t, uint8_t,    300, 10>));

  RUN_TEST((test_interpolateLinear<uint8_t, uint16_t,  1220, 50, interpolateLinearReturnOutputType<uint8_t, uint16_t, float>>));
  RUN_TEST((test_interpolateLinear<uint8_t, uint16_t,   800, 10>));

  RUN_TEST((test_interpolateLinear<uint8_t, uint32_t,  1230, 100, interpolateLinearReturnOutputType<uint8_t, uint32_t, float>>));
  RUN_TEST((test_interpolateLinear<uint8_t, uint32_t,  1393, 10, interpolateLinearFixedReturnOutputType<uint8_t, uint32_t, uint64_t, 8>>)); // fixed point slower than float
  RUN_TEST((test_interpolateLinear<uint8_t, uint32_t,  1230, 100>));

  RUN_TEST((test_interpolateLinear<uint16_t, uint8_t,  1220, 50, interpolateLinearReturnOutputType<uint16_t, uint8_t, float>>));
  RUN_TEST((test_interpolateLinear<uint16_t, uint8_t,   800, 10>));

  RUN_TEST((test_interpolateLinear<uint16_t, uint16_t, 1270, 100, interpolateLinearReturnOutputType<uint16_t, uint16_t, float>>));
  RUN_TEST((test_interpolateLinear<uint16_t, uint16_t,  800, 10>));

  RUN_TEST((test_interpolateLinear<uint16_t, uint32_t, 1260, 100, interpolateLinearReturnOutputType<uint16_t, uint32_t, float>>));
  RUN_TEST((test_interpolateLinear<uint16_t, uint32_t, 1417, 10, interpolateLinearFixedReturnOutputType<uint16_t, uint32_t, uint64_t, 8>>)); // fixed point slower than float
  RUN_TEST((test_interpolateLinear<uint16_t, uint32_t, 1260, 100>));

  RUN_TEST((test_interpolateLinear<uint32_t, uint8_t,  1290, 50, interpolateLinearReturnOutputType<uint32_t, uint8_t, float>>));
  RUN_TEST((test_interpolateLinear<uint32_t, uint8_t,  1386, 10, interpolateLinearFixedReturnOutputType<uint32_t, uint8_t, uint64_t, 8>>)); // fixed point slower than float
  RUN_TEST((test_interpolateLinear<uint32_t, uint8_t,  1290, 50>));

  RUN_TEST((test_interpolateLinear<uint32_t, uint16_t, 1290, 50, interpolateLinearReturnOutputType<uint32_t, uint16_t, float>>));
  RUN_TEST((test_interpolateLinear<uint32_t, uint16_t, 1400, 10, interpolateLinearFixedReturnOutputType<uint32_t, uint16_t, uint64_t, 8>>)); // fixed point slower than float
  RUN_TEST((test_interpolateLinear<uint32_t, uint16_t, 1290, 50>));

  RUN_TEST((test_interpolateLinear<uint32_t, uint32_t, 1290, 100, interpolateLinearReturnOutputType<uint32_t, uint32_t, float>>));
  RUN_TEST((test_interpolateLinear<uint32_t, uint32_t, 1456, 10, interpolateLinearFixedReturnOutputType<uint32_t, uint32_t, uint64_t, 8>>)); // fixed point slower than float
  RUN_TEST((test_interpolateLinear<uint32_t, uint32_t, 1290, 100>));

  RUN_TEST(test_interpolateLinearTable);
  RUN_TEST(test_interpolateBilinear);
  RUN_TEST(test_interpolateBilinearTable);

  UNITY_END(); // stop unit testing
}

void loop() {
}

