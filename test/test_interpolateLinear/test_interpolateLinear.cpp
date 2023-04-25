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
  TEST_ASSERT_UINT16_WITHIN(10, 938, TIME_DIFF);

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
  TEST_ASSERT_UINT16_WITHIN(10, 987, TIME_DIFF);

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
  TEST_ASSERT_UINT16_WITHIN(10, 938, TIME_DIFF);

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
  TEST_ASSERT_UINT16_WITHIN(10, 987, TIME_DIFF);
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

  UNITY_END(); // stop unit testing
}

void loop() {
}

