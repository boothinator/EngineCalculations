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

#define TIME_START {noInterrupts();start = TCNT1;}
#define TIME_END {end = TCNT1;interrupts();}
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

  RpmCalculator calculateRpm = RpmCalculator(ticksPerSecond);

  TIME_START
  volatile float actual = calculateRpm(crankSpeedDegreesPerTick);
  TIME_END

  TEST_ASSERT_EQUAL_FLOAT(expected, actual);

#ifdef __AVR_ATmega2560__
  TEST_ASSERT_UINT16_WITHIN(10, 177, TIME_DIFF);
#endif
}

void test_getTicksFromAngle()
{
  volatile uint32_t lastCrankEventTicks = 13338414ul;
  uint32_t expected = 3333ul + lastCrankEventTicks; // 333.333 ticks/degree
  float rpm = 1000.0; 
  volatile float inverseCrankSpeedTicksPerDegree = 1.0 / getCrankSpeedDegreesPerTick(rpm);
  volatile float lastCrankEventAngle = 25.0;
  volatile float angle = 35.0;

  TIME_START
  volatile uint32_t actual = getTicksFromAngle(lastCrankEventAngle, lastCrankEventTicks, inverseCrankSpeedTicksPerDegree, angle);
  TIME_END

  TEST_ASSERT_UINT32_WITHIN(2, expected, actual);

#ifdef __AVR_ATmega2560__
  TEST_ASSERT_UINT16_WITHIN(150, 550, TIME_DIFF);
#endif
}

void test_calculateInjectionLength()
{
  uint32_t expected = 18054;
  float rpm = 4000.0; 
  volatile float targetFuelAirRatio = 1.0 / 14.7;
  volatile float inverseCrankSpeedTicksPerDegree = 1.0 / getCrankSpeedDegreesPerTick(rpm);
  volatile float airflowGramsPerSecond = 59.0;

  InjectionLengthCalculator calculateInjectionLengthTicks = InjectionLengthCalculator(ticksPerSecond, 265.0, 4);

  TIME_START
  volatile uint32_t actual = calculateInjectionLengthTicks(targetFuelAirRatio, inverseCrankSpeedTicksPerDegree, airflowGramsPerSecond);
  TIME_END

  TEST_ASSERT_EQUAL_UINT16(expected, actual);

#ifdef __AVR_ATmega2560__
  TEST_ASSERT_UINT16_WITHIN(10, 517, TIME_DIFF);
#endif
}

void test_load()
{
  float rpm = 4000.0; 
  float expected = 0.7990716;
  volatile float inverseCrankSpeedTicksPerDegree = 1.0 / getCrankSpeedDegreesPerTick(rpm);
  volatile float airflowGramsPerSecond = 59.0;

  LoadFractionCalculator calculateLoadFraction = LoadFractionCalculator(ticksPerSecond, 4, 8.3, 8.5);

  TIME_START
  volatile float actual = calculateLoadFraction(inverseCrankSpeedTicksPerDegree, airflowGramsPerSecond);
  TIME_END

  TEST_ASSERT_EQUAL_FLOAT(expected, actual);

#ifdef __AVR_ATmega2560__
  TEST_ASSERT_UINT_WITHIN(20, 319, TIME_DIFF);
#endif
}

template<uint8_t valueBits>
void test_expSmooth(uint16_t cur, uint16_t prev, float alphaF, uint16_t ticks)
{
  static constexpr uint16_t alphaFactor = 1 << 6;
  volatile uint16_t actual;
  volatile uint16_t alpha16 = alphaF * alphaFactor;
  volatile uint8_t alpha = static_cast<uint8_t>(alpha16);
  volatile uint8_t oneMinusAlpha = static_cast<uint8_t>(alphaFactor - alpha16);
  uint16_t start, end;

  uint16_t expected = static_cast<uint16_t>((static_cast<float>(cur) * alpha + static_cast<float>(prev) * oneMinusAlpha)/alphaFactor + 0.5);

  TIME_START
  actual = expSmooth<6, uint16_t, valueBits>(cur, prev, alpha, oneMinusAlpha);
  TIME_END

  TEST_ASSERT_EQUAL_UINT16(expected, actual);

  TEST_ASSERT_UINT16_WITHIN(10, ticks, TIME_DIFF);
}

void test_expSmooth()
{
  volatile uint16_t cur = 800, prev = 900;

  test_expSmooth<10>(cur, prev, 0.3, 66);
  test_expSmooth<16>(cur, prev, 0.3, 78);
  test_expSmooth<26>(cur, prev, 0.3, 127);
}

void test_inAscendingOrder()
{
  uint16_t advanceRpmArr[] = {
    700,  950,  1200, 1500, 2000, 2600, 3100, 3700, 4300, 4900, 5000, 6000, 6500, 7000, 7200, 7500};

  size_t len = sizeof(advanceRpmArr) / sizeof(advanceRpmArr[0]);

  TEST_ASSERT_TRUE(inAscendingOrder(advanceRpmArr, len));
}

void test_inAscendingOrder_False()
{
  uint16_t advanceRpmArr[] = {
    700,  950,  1200, 2000, 1500, 2600, 3100, 3700, 4300, 4900, 5000, 6000, 6500, 7000, 7200, 7500};

  size_t len = sizeof(advanceRpmArr) / sizeof(advanceRpmArr[0]);

  TEST_ASSERT_FALSE(inAscendingOrder(advanceRpmArr, len));
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
  RUN_TEST(test_expSmooth);
  RUN_TEST(test_inAscendingOrder);

  UNITY_END(); // stop unit testing
}

void loop() {
}

