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

float _loadFractionMultiplier = 0;

} // namespace

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
