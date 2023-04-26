// Interpolate
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

#include "interpolateBilinear.h"

template<>
uint8_t divRound(uint8_t num, uint8_t denom)
{
  return (num + denom / 2) / denom;
}

template<>
uint16_t divRound(uint16_t num, uint16_t denom)
{
  return (num + denom / 2) / denom;
}

template<>
uint32_t divRound(uint32_t num, uint32_t denom)
{
  return (num + denom / 2) / denom;
}
template<>
uint64_t divRound(uint64_t num, uint64_t denom)
{
  return (num + denom / 2) / denom;
}

/*
For fixed point math, DeltaXMulZ needs to be big enough to hold any deltaX * Z, DeltaYMulZ needs
to be big enough to hold any deltaY * Z, and DivType must be able to hold deltaX * deltaY * z
 */
template<>
uint8_t interpolateBilinear(uint8_t x, uint8_t x0, uint8_t x1, uint8_t y, uint8_t y0, uint8_t y1, uint8_t z00, uint8_t z10, uint8_t z01, uint8_t z11)
{
  return interpolateBilinearXFirst<uint16_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
}

template<>
uint16_t interpolateBilinear(uint8_t x, uint8_t x0, uint8_t x1, uint8_t y, uint8_t y0, uint8_t y1, uint16_t z00, uint16_t z10, uint16_t z01, uint16_t z11)
{
  return interpolateBilinearXFirst<uint32_t, uint32_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
}

template<>
uint8_t interpolateBilinear(uint8_t x, uint8_t x0, uint8_t x1, uint16_t y, uint16_t y0, uint16_t y1, uint8_t z00, uint8_t z10, uint8_t z01, uint8_t z11)
{
  return interpolateBilinearXFirst<uint16_t, uint32_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
}

template<>
uint16_t interpolateBilinear(uint8_t x, uint8_t x0, uint8_t x1, uint16_t y, uint16_t y0, uint16_t y1, uint16_t z00, uint16_t z10, uint16_t z01, uint16_t z11)
{
  return interpolateBilinearXFirst<uint16_t, uint32_t, uint64_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
}

template<>
uint8_t interpolateBilinear(uint16_t x, uint16_t x0, uint16_t x1, uint8_t y, uint8_t y0, uint8_t y1, uint8_t z00, uint8_t z10, uint8_t z01, uint8_t z11)
{
  // Interpolate Y first since Y * Z is smaller than X * Z
  return interpolateBilinearYFirst<uint32_t, uint16_t, uint32_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
}

template<>
uint16_t interpolateBilinear(uint16_t x, uint16_t x0, uint16_t x1, uint8_t y, uint8_t y0, uint8_t y1, uint16_t z00, uint16_t z10, uint16_t z01, uint16_t z11)
{
  // Interpolate Y first since Y * Z is smaller than X * Z
  return interpolateBilinearYFirst<uint32_t, uint32_t, uint64_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
}

template<>
uint8_t interpolateBilinear(uint16_t x, uint16_t x0, uint16_t x1, uint16_t y, uint16_t y0, uint16_t y1, uint8_t z00, uint8_t z10, uint8_t z01, uint8_t z11)
{
  return interpolateBilinearXFirst<uint32_t, uint32_t, uint64_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
}

template<>
uint16_t interpolateBilinear(uint16_t x, uint16_t x0, uint16_t x1, uint16_t y, uint16_t y0, uint16_t y1, uint16_t z00, uint16_t z10, uint16_t z01, uint16_t z11)
{
  return interpolateBilinearXFirst<uint32_t, uint32_t, uint64_t>(x, x0, x1, y, y0, y1, z00, z10, z01, z11);
}

