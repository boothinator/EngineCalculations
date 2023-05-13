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

RpmCalculator::RpmCalculator(float ticksPerSecond)
{
  _calculateRpmMultiplierRevTicksPerDegreeMinute =
    ticksPerSecond
    * ( 60.0 / 1.0 ) /* seconds/minute */
    * ( 1.0 / 360.0 ) /* rev/degree */;
}

float RpmCalculator::calculate(float crankSpeedDegreesPerTick)
{
  return _calculateRpmMultiplierRevTicksPerDegreeMinute * crankSpeedDegreesPerTick;
}

float RpmCalculator::operator() (float crankSpeedDegreesPerTick)
{
  return calculate(crankSpeedDegreesPerTick);
}