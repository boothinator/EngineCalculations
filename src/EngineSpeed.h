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

#include "Types.h"

void configureEngineSpeedCalculations(ticks_t ticksPerSecond);

float calculateRpm(float crankSpeedDegreesPerTick);

ticks_t getTicksFromAngle(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float inverseCrankSpeedTicksPerDegree, angle_t angle);

ticks_t getTicksFromAngleHalfCycle(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float inverseCrankSpeedTicksPerDegree, angle_t angle);

angle_t getAngle(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float crankSpeedDegreesPerTick, ticks_t ticks);

angle_t getAngleHalfCycle(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float crankSpeedDegreesPerTick, ticks_t ticks);


angle_t getAngleInPast(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float crankSpeedDegreesPerTick, ticks_t ticks);

angle_t getAngleInPastHalfCycle(angle_t lastCrankEventAngle, ticks_t lastCrankEventTicks,
  float crankSpeedDegreesPerTick, ticks_t ticks);

#endif