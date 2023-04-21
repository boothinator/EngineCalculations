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


#include "Load.h"

#include <math.h>

namespace
{

float loadFractionMultiplierCylSDegreePerGTick = 0.0;

// NIST STP at 20 C and 101.325 kPa
constexpr float airDensityAtNtpKgPerCubicMeter = 1.2041;

constexpr float airDensityAtNtpGramsPerCc =
  airDensityAtNtpKgPerCubicMeter
  * 1000.0 /* g / kg */
  * 1.0 / (100.0 * 100.0 * 100.0); /* cubic meter / cubic centimeter */

} // namespace

void configureLoadCalculation(float ticksPerSecond, int cylindersPerAirflowSensor, float cylinderBoreCm, float cylinderStrokeCm)
{
  float cylinderRadiusCm = cylinderBoreCm / 2.0;
  float cylinderAreaSqCm =  M_PI * cylinderRadiusCm * cylinderRadiusCm;

  // Load = Airflow / Cylinder Avg Airflow
  // Cylinder Avg Airflow = Piston avg velocity cm/s * Cylinder area cm^2 * Air density at STP g/cm^3

  float cylinderAvgAirflowCylGPerDegree = 
    (1.0 / 180.0) /* stroke / degrees */
    * cylindersPerAirflowSensor / 4 /* [cylinder / airflow sensor] / [stroke / airflow sensor] */
    * cylinderStrokeCm * cylinderAreaSqCm /* cc / stroke */
    * airDensityAtNtpGramsPerCc;

  loadFractionMultiplierCylSDegreePerGTick =
    (1.0 / ticksPerSecond) /* seconds / tick */
    * (1.0 / cylinderAvgAirflowCylGPerDegree); /* degrees / g */
}

bool isLoadConfigured()
{
  return loadFractionMultiplierCylSDegreePerGTick > 0.0;
}

float calculateLoadFraction(float inverseCrankSpeedTicksPerDegree, float airflowGramsPerSecond)
{
  return airflowGramsPerSecond
    * inverseCrankSpeedTicksPerDegree
    * loadFractionMultiplierCylSDegreePerGTick;
}
