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

#include "EngineSpeed.h"

#include "Config.h"

TICKS_PER_SECOND_ATTR float calculateRpm(float crankSpeedDegreesPerTick)
{
  TICKS_PER_SECOND_ATTR float multiplierRevTicksPerDegreeMinute
    = ticksPerSecond
      * ( 60.0 / 1.0 ) /* seconds/minute */
      * ( 1.0 / 360.0 ) /* rev/degree */;

  return multiplierRevTicksPerDegreeMinute * crankSpeedDegreesPerTick;
}


ticks_t getTicksFromAngle(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float crankSpeedInverseTicksPerDegree, angle_t angle, angle_t cycleAngle)
{
  while (angle < lastCrankEventAngle)
  {
    angle += cycleAngle;
  }

  angle_t angleDiff = angle - lastCrankEventAngle;

  return static_cast<ticks_t>(crankSpeedInverseTicksPerDegree * angleDiff + lastCrankEventTicks);
}


ticks_t getTicksFromAngle(angle_t crankSpeedInverseTicksPerDegree, ticks_t lastCrankEventTicks,
  float crankSpeedInverse, angle_t angle)
{
  return getTicksFromAngle(crankSpeedInverseTicksPerDegree, lastCrankEventTicks,
    crankSpeedInverse, angle, 720.0);
}

ticks_t getTicksFromAngleHalfCycle(angle_t crankSpeedInverseTicksPerDegree, ticks_t lastCrankEventTicks,
  float crankSpeedInverse, angle_t angle)
{
  return getTicksFromAngle(crankSpeedInverseTicksPerDegree, lastCrankEventTicks,
    crankSpeedInverse, angle, 360.0);
}

// Angle after cylinder 1 TDC
angle_t getAngleFloat(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
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

angle_t getAngleFloat(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float crankSpeedDegreesPerTick, ticks_t ticks)
{
  return getAngleFloat(lastCrankEventAngle, lastCrankEventTicks, crankSpeedDegreesPerTick, ticks, 720.0);
}

angle_t getAngleHalfCycleFloat(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float crankSpeedDegreesPerTick, ticks_t ticks)
{
  return getAngleFloat(lastCrankEventAngle, lastCrankEventTicks, crankSpeedDegreesPerTick, ticks, 360.0);
}


// Angle after cylinder 1 TDC. Assumes that ticks is before lastCrankEventTicks
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

angle_t getAngleInPast(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float crankSpeedDegreesPerTick, ticks_t ticks)
{
  return getAngleInPast(lastCrankEventAngle, lastCrankEventTicks, crankSpeedDegreesPerTick, ticks, 720.0);
}

angle_t getAngleInPastHalfCycle(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float crankSpeedDegreesPerTick, ticks_t ticks)
{
  return getAngleInPast(lastCrankEventAngle, lastCrankEventTicks, crankSpeedDegreesPerTick, ticks, 360.0);
}