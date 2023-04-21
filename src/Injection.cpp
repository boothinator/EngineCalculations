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

float injectionLengthMultiplierSecDegTicksPerGramStrokeCylinder = 0.0;

} // namespace

void configureInjectionLengthCalculation(float ticksPerSecond, float injectorFlowCcPerMin, int cylindersPerAirflowSensor, float fuelDensityGramPerCc)
{
  // Injection length formula:
  //   injectionCc = (airflow * injectionCcMultiplier) / (rpm * targetAfr)
  //   injLengthTicks = injectionCc * injectorFlowTicksPerCc

  injectionLengthMultiplierSecDegTicksPerGramStrokeCylinder = /* seconds degrees ticks/[g stroke cylinder] */
    ( 1.0 / injectorFlowCcPerMin )
    * 60.0 / 1.0 /* seconds / minute */
    * 180.0 / 1.0 /* degrees / stroke */
    * 4 / cylindersPerAirflowSensor /* [stroke / airflow sensor] / [cylinder / airflow sensor] */
    * ( 1.0 / fuelDensityGramPerCc );
}

bool isInjectionLengthConfigured()
{
  return injectionLengthMultiplierSecDegTicksPerGramStrokeCylinder > 0.0;
}

float calculateInjectionLengthTicks(float targetFuelAirRatio, float inverseCrankSpeedTicksPerDegree, float airflowGramsPerSecond)
{
  float injLengthTicks =
    airflowGramsPerSecond
    * inverseCrankSpeedTicksPerDegree
    * targetFuelAirRatio
    * injectionLengthMultiplierSecDegTicksPerGramStrokeCylinder;  /* seconds degrees ticks/[g stroke] */

  return injLengthTicks;
}
