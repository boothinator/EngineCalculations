// Engine Speed
// Copyright (C) 2023  Joshua Booth

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

#ifndef ENGINE_CALCULATIONS_ENGINE_SPEED_H_
#define ENGINE_CALCULATIONS_ENGINE_SPEED_H_

#pragma once

void configureEngineSpeedCalculations(float ticksPerSecond);

bool isEngineSpeedConfigured();

float calculateRpm(float crankSpeedDegreesPerTick);

template<typename angle_t, typename ticks_t>
ticks_t getTicksFromAngle(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float inverseCrankSpeedTicksPerDegree, angle_t angle, angle_t cycleAngle)
{
  while (angle < lastCrankEventAngle)
  {
    angle += cycleAngle;
  }

  angle_t angleDiff = angle - lastCrankEventAngle;

  return static_cast<ticks_t>(inverseCrankSpeedTicksPerDegree * angleDiff) + lastCrankEventTicks;
}

template<typename angle_t, typename ticks_t>
ticks_t getTicksFromAngle(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float inverseCrankSpeedTicksPerDegree, angle_t angle)
{
  return getTicksFromAngle(lastCrankEventAngle, lastCrankEventTicks,
    inverseCrankSpeedTicksPerDegree, angle, static_cast<angle_t>(720));
}

template<typename angle_t, typename ticks_t>
ticks_t getTicksFromAngleHalfCycle(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float inverseCrankSpeedTicksPerDegree, angle_t angle)
{
  return getTicksFromAngle(lastCrankEventAngle, lastCrankEventTicks,
    inverseCrankSpeedTicksPerDegree, angle, static_cast<angle_t>(360));
}

// Angle after cylinder 1 TDC
template<typename angle_t, typename ticks_t>
angle_t getAngle(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float crankSpeedDegreesPerTick, ticks_t ticks, angle_t cycleAngle)
{
  ticks_t ticksDiff = ticks - lastCrankEventTicks;

  angle_t angleSinceLastCrankEvent = static_cast<angle_t>(crankSpeedDegreesPerTick * ticksDiff);

  angle_t angle = lastCrankEventAngle + angleSinceLastCrankEvent;

  while (angle >= cycleAngle)
  {
    angle -= cycleAngle;
  }

  return angle;
}

template<typename angle_t, typename ticks_t>
angle_t getAngle(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float crankSpeedDegreesPerTick, ticks_t ticks)
{
  return getAngle(lastCrankEventAngle, lastCrankEventTicks, crankSpeedDegreesPerTick, ticks, static_cast<angle_t>(720));
}

template<typename angle_t, typename ticks_t>
angle_t getAngleHalfCycle(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float crankSpeedDegreesPerTick, ticks_t ticks)
{
  return getAngle(lastCrankEventAngle, lastCrankEventTicks, crankSpeedDegreesPerTick, ticks, static_cast<angle_t>(360));
}


// Angle after cylinder 1 TDC. Assumes that ticks is before lastCrankEventTicks
template<typename angle_t, typename ticks_t>
angle_t getAngleInPast(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float crankSpeedDegreesPerTick, ticks_t ticks, angle_t cycleAngle)
{
  ticks_t ticksDiff = lastCrankEventTicks - ticks;

  angle_t angleBeforeLastCrankEvent = static_cast<angle_t>(crankSpeedDegreesPerTick * ticksDiff);

  angle_t angle = lastCrankEventAngle - angleBeforeLastCrankEvent;

  while (angle >= cycleAngle)
  {
    angle += cycleAngle;
  }

  return angle;
}

template<typename angle_t, typename ticks_t>
angle_t getAngleInPast(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float crankSpeedDegreesPerTick, ticks_t ticks)
{
  return getAngleInPast(lastCrankEventAngle, lastCrankEventTicks, crankSpeedDegreesPerTick, ticks, static_cast<angle_t>(720));
}

template<typename angle_t, typename ticks_t>
angle_t getAngleInPastHalfCycle(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float crankSpeedDegreesPerTick, ticks_t ticks)
{
  return getAngleInPast(lastCrankEventAngle, lastCrankEventTicks, crankSpeedDegreesPerTick, ticks, static_cast<angle_t>(360));
}

#endif