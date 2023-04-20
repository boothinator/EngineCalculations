// Injection
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


#include "Injection.h"

#include <math.h>

#include <Arduino.h>

namespace
{

float injectionLengthMultiplierSecDegTicksPerGramStroke = 0;

} // namespace

void configureInjectionLengthCalculation(ticks_t ticksPerSecond, float injectorFlowCcPerMin, float fuelDensityGramPerCc)
{
  // Injection length formula:
  //   injectionCc = (airflow * injectionCcMultiplier) / (rpm * targetAfr)
  //   injLengthTicks = injectionCc * injectorFlowTicksPerCc

  injectionLengthMultiplierSecDegTicksPerGramStroke = /* seconds degrees ticks/[g stroke] */
    ( 1.0 / injectorFlowCcPerMin )
    * 10800
    * ( 1.0 / fuelDensityGramPerCc );
}

ticks_t calculateInjectionLengthTicks(float targetFuelAirRatio, float crankSpeedInverseTicksPerDegree, float airflowGramsPerSecond)
{
  float injLengthTicks =
    airflowGramsPerSecond
    * crankSpeedInverseTicksPerDegree
    * targetFuelAirRatio
    * injectionLengthMultiplierSecDegTicksPerGramStroke;  /* seconds degrees ticks/[g stroke] */

  return static_cast<ticks_t>(injLengthTicks + 0.5);
}
