// Load
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

#ifndef ENGINE_CALCULATIONS_LOAD_H_
#define ENGINE_CALCULATIONS_LOAD_H_

#pragma once

#include "Types.h"

ticks_t calculateInjectionLengthTicks(float targetFuelAirRatio, float inverseRpm, float airflowGramsPerSecond);

float calculateLoadFraction(float inverseRpm, float airflowGramsPerSecond);

#endif