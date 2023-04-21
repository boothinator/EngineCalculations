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

constexpr float ticksPerSecond = 2000000;

void setUp(void) {
  configureEngineSpeedCalculations(ticksPerSecond);
  configureInjectionLengthCalculation(ticksPerSecond, 265.0);
  configureLoadCalculation(ticksPerSecond, 8.3, 8.5);
}

void tearDown(void) {
// clean stuff up here
}

float getCrankSpeedDegreesPerTick(float rpm)
{
  return rpm
    * ( 1.0 / 60 ) /* min / s */
    * ( 1.0 / ticksPerSecond) /* s / tick */
    * 360; /* degrees / rev */
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

void test_calculateRpm()
{
  float expected = 1000.0;
  volatile float crankSpeedDegreesPerTick = getCrankSpeedDegreesPerTick(expected);

  TIME_START
  volatile float actual = calculateRpm(crankSpeedDegreesPerTick);
  TIME_END

  TEST_ASSERT_EQUAL_FLOAT(expected, actual);

#ifdef __AVR_ATmega2560__
  TEST_ASSERT_EQUAL(181, TIME_DIFF);
#endif
}

void test_getTicksFromAngle()
{
  uint32_t expected = 3433; // 333.333 ticks/degree
  float rpm = 1000.0; 
  volatile float inverseCrankSpeedTicksPerDegree = 1.0 / getCrankSpeedDegreesPerTick(rpm);
  volatile float lastCrankEventAngle = 25.0;
  volatile uint32_t lastCrankEventTicks = 100;
  volatile float angle = 35.0;

  TIME_START
  volatile uint32_t actual = getTicksFromAngle(lastCrankEventAngle, lastCrankEventTicks, inverseCrankSpeedTicksPerDegree, angle);
  TIME_END

  TEST_ASSERT_EQUAL_UINT16(expected, actual);

#ifdef __AVR_ATmega2560__
  TEST_ASSERT_EQUAL(629, TIME_DIFF);
#endif
}

void test_calculateInjectionLength()
{
  uint32_t expected = 18054;
  float rpm = 4000.0; 
  volatile float targetFuelAirRatio = 1.0 / 14.7;
  volatile float inverseCrankSpeedTicksPerDegree = 1.0 / getCrankSpeedDegreesPerTick(rpm);
  volatile float airflowGramsPerSecond = 59.0;

  TIME_START
  volatile uint32_t actual = calculateInjectionLengthTicks(targetFuelAirRatio, inverseCrankSpeedTicksPerDegree, airflowGramsPerSecond);
  TIME_END

  TEST_ASSERT_EQUAL_UINT16(expected, actual);

#ifdef __AVR_ATmega2560__
  TEST_ASSERT_EQUAL(517, TIME_DIFF);
#endif
}

void test_load()
{
  float rpm = 4000.0; 
  float expected = 0.7990716;
  volatile float inverseCrankSpeedTicksPerDegree = 1.0 / getCrankSpeedDegreesPerTick(rpm);
  volatile float airflowGramsPerSecond = 59.0;

  TIME_START
  volatile float actual = calculateLoadFraction(inverseCrankSpeedTicksPerDegree, airflowGramsPerSecond);
  TIME_END

  TEST_ASSERT_EQUAL_FLOAT(expected, actual);

#ifdef __AVR_ATmega2560__
  TEST_ASSERT_UINT_WITHIN(20, 319, TIME_DIFF);
#endif
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

  RUN_TEST(test_calculateRpm);
  RUN_TEST(test_getTicksFromAngle);
  RUN_TEST(test_calculateInjectionLength);
  RUN_TEST(test_load);

  UNITY_END(); // stop unit testing
}

void loop() {
}

