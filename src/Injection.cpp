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

namespace
{

float _injectionLengthMultiplierRevSecondPerMinRadG = 0;

} // namespace

void configureInjectionLengthCalculation(ticks_t ticksPerSecond, float injectorFlowCcPerMin, float fuelDensityGramPerCc)
{
  // Injection length formula:
  //   injectionCc = (airflow * injectionCcMultiplier) / (rpm * targetAfr)
  //   injLengthTicks = injectionCc * injectorFlowTicksPerCc

  // approx value: 452,830
  float injectorFlowTicksPerCc =
    ticksPerSecond
    * ( 60.0 / 1.0 ) /* seconds / minute */
    * ( 1.0 / injectorFlowCcPerMin );

  _injectionLengthMultiplierRevSecondPerMinRadG = /* rev s / [min rad g] */
    injectorFlowTicksPerCc
    * ( 1.0 / ( M_PI ) ) /* rev / radian */
    * ( 1.0 / fuelDensityGramPerCc);
}

ticks_t calculateInjectionLengthTicks(float targetFuelAirRatio, float inverseRpm, float airflowGramsPerSecond)
{
  float injLengthTicks =
    airflowGramsPerSecond
    * inverseRpm /* minutes / rev */
    * targetFuelAirRatio
    * _injectionLengthMultiplierRevSecondPerMinRadG; /* rev s / [min rad g] */

  return static_cast<ticks_t>(injLengthTicks);
}

float _loadFractionMultiplier = 0;

void configureLoadCalculation(float cylinderStrokeCm, float pistonAreaSqCm, float airDensityAtStpGramsPerCc)
{

  // Load = Airflow / Cylinder Max Airflow
  // Cylinder Max Airflow = Piston max velocity cm/s * Piston area cm^2 * Air density at STP g/cm^3

  float cylinderMaxAirflowG = 
    (M_PI / 60.0)
    * cylinderStrokeCm
    * pistonAreaSqCm
    * airDensityAtStpGramsPerCc;

  _loadFractionMultiplier = 1.0 / cylinderMaxAirflowG;
}

float calculateLoadFraction(float inverseRpm, float airflowGramsPerSecond)
{
  return airflowGramsPerSecond * inverseRpm * _loadFractionMultiplier;
}
