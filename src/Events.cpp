// Events
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


#include "Types.h"

angle_t getAngleTdc(int cylinder, uint8_t *firingOrder, uint8_t cylinderCount)
{
  // Assumes TDC angles are evenly spaced

  for (int i = 0; i < cylinderCount; i++)
  {
    if (cylinder == firingOrder[i])
    {
      return i * (720.0 / cylinderCount);
    }
  }

  return static_cast<angle_t>(-1.0);
}

angle_t getAngleTdcHalfCycle(int cylinder, uint8_t *firingOrder, uint8_t cylinderCount)
{
  angle_t angle = getAngleTdc(cylinder, firingOrder, cylinderCount);
  
  while (angle >= 360.0)
  {
    angle -= 360.0;
  }

  return angle;
}
