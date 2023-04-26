// Linear Interpolation
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

#ifndef ENGINE_CALCULATIONS_INTERPOLATE_BILINEAR_H_
#define ENGINE_CALCULATIONS_INTERPOLATE_BILINEAR_H_

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "scale.h"
#include "interpolateLinear.h"

template<typename T>
inline T divRound(T num, T denom)
{
  return num/denom;
}

template<>
uint8_t divRound(uint8_t num, uint8_t denom);

template<>
uint16_t divRound(uint16_t num, uint16_t denom);

template<>
uint32_t divRound(uint32_t num, uint32_t denom);

template<>
uint64_t divRound(uint64_t num, uint64_t denom);

template<typename DeltaXMulZ, typename DeltaYMulZ, typename DivType,
  typename X, typename Y, typename Z>
DivType interpolateBilinearXFirst(X x, X x0, X x1, Y y, Y y0, Y y1, Z z00, Z z10, Z z01, Z z11)
{
  // Generally: y_interpolated = [ y0 * (x1 - x) + y1 * (x - x0) ] / (x1 - x0)

  // We're interpolating the rows first
  // Note: z subscripts are x,y. Ex: z01 is at x = 0, y = 1

  // Upcast so we don't overflow fixed-point math when we multiply
  DivType deltaX = static_cast<DivType>(x1 - x0);
  DeltaXMulZ deltaX0 = static_cast<DeltaXMulZ>(x - x0);
  DeltaXMulZ deltaX1 = static_cast<DeltaXMulZ>(x1 - x);

  // z_row0_interpolated = [ z00 * (x1 - x) + z10 * (x - x0) ] / (x1 - x0)
  DivType z_row0_num = static_cast<DivType>(static_cast<DeltaXMulZ>(z00) * deltaX1 + static_cast<DeltaXMulZ>(z10) * deltaX0);

  // z_row1_interpolated = [ z01 * (x1 - x) + z11 * (x - x0) ] / (x1 - x0)
  DivType z_row1_num = static_cast<DivType>(static_cast<DeltaXMulZ>(z01) * deltaX1 + static_cast<DeltaXMulZ>(z11) * deltaX0);

  // Denominators are the same
  DivType z_row_denom = deltaX;

  // Upcast so we don't overflow fixed-point math when we multiply
  DivType deltaY = static_cast<DivType>(y1 - y0);
  DeltaYMulZ deltaY0 = static_cast<DeltaYMulZ>(y - y0);
  DeltaYMulZ deltaY1 = static_cast<DeltaYMulZ>(y1 - y);

  // z_interpolated = [ z_row0_interpolated * (y1 - y) + z_row1_interpolated * (y - y0) ] / (y1 - y0)
  DivType z_num = z_row0_num * deltaY1 + z_row1_num * deltaY0;

  // Need to include the Z row denomizator
  DivType z_denom = deltaY * z_row_denom;
  
  return divRound(z_num, z_denom);
}

template<typename DeltaXMulZ, typename DeltaYMulZ, typename DivType,
  typename X, typename Y, typename Z>
DivType interpolateBilinearYFirst(X x, X x0, X x1, Y y, Y y0, Y y1, Z z00, Z z10, Z z01, Z z11)
{
  // Rotate 90 degrees
  return interpolateBilinearXFirst<DeltaYMulZ, DeltaXMulZ, DivType>(y, y0, y1, x, x0, x1, z00, z01, z10, z11);
}

template<typename T>
float roundingFactor()
{
  return 0.0;
}

template<>
float roundingFactor<uint8_t>();

template<>
float roundingFactor<uint16_t>();

template<>
float roundingFactor<uint32_t>();

template<>
float roundingFactor<uint64_t>();

template<typename X, typename Y, typename Z, typename DeltaXMulZ = float, typename DeltaYMulZ = float, typename DivType = float>
Z interpolateBilinear(X x, X x0, X x1, Y y, Y y0, Y y1, Z z00, Z z10, Z z01, Z z11)
{
  return static_cast<Z>(interpolateBilinearXFirst<DeltaXMulZ, DeltaYMulZ, DivType>(x, x0, x1, y, y0, y1, z00, z10, z01, z11) + roundingFactor<Z>());
}

template<>
uint8_t interpolateBilinear(uint8_t x, uint8_t x0, uint8_t x1, uint8_t y, uint8_t y0, uint8_t y1, uint8_t z00, uint8_t z10, uint8_t z01, uint8_t z11);

template<>
uint16_t interpolateBilinear(uint8_t x, uint8_t x0, uint8_t x1, uint8_t y, uint8_t y0, uint8_t y1, uint16_t z00, uint16_t z10, uint16_t z01, uint16_t z11);

template<>
uint8_t interpolateBilinear(uint8_t x, uint8_t x0, uint8_t x1, uint16_t y, uint16_t y0, uint16_t y1, uint8_t z00, uint8_t z10, uint8_t z01, uint8_t z11);

template<>
uint16_t interpolateBilinear(uint8_t x, uint8_t x0, uint8_t x1, uint16_t y, uint16_t y0, uint16_t y1, uint16_t z00, uint16_t z10, uint16_t z01, uint16_t z11);

template<>
uint8_t interpolateBilinear(uint16_t x, uint16_t x0, uint16_t x1, uint8_t y, uint8_t y0, uint8_t y1, uint8_t z00, uint8_t z10, uint8_t z01, uint8_t z11);

template<>
uint16_t interpolateBilinear(uint16_t x, uint16_t x0, uint16_t x1, uint8_t y, uint8_t y0, uint8_t y1, uint16_t z00, uint16_t z10, uint16_t z01, uint16_t z11);

template<>
uint8_t interpolateBilinear(uint16_t x, uint16_t x0, uint16_t x1, uint16_t y, uint16_t y0, uint16_t y1, uint8_t z00, uint8_t z10, uint8_t z01, uint8_t z11);

template<>
uint16_t interpolateBilinear(uint16_t x, uint16_t x0, uint16_t x1, uint16_t y, uint16_t y0, uint16_t y1, uint16_t z00, uint16_t z10, uint16_t z01, uint16_t z11);

template<typename Z, typename X, typename Y, typename XArray, typename YArray, typename ZArray>
Z interpolateBilinearTable(X x, Y y, size_t xLength, size_t yLength,
                                    XArray xScale, YArray yScale, ZArray outputArray)
{
  size_t xLowIndex;
  X xLow;
  X xHigh;

  FindOnScaleResult xResult = findOnScale(x, xScale, xLength,  xLowIndex, xLow, xHigh);

  size_t yLowIndex;
  Y yLow;
  Y yHigh;

  FindOnScaleResult yResult = findOnScale(y, yScale, yLength, yLowIndex, yLow, yHigh);

  if (FindOnScaleResult::InBetween == yResult
    && FindOnScaleResult::InBetween == xResult)
  {
    size_t xHighIndex = xLowIndex + 1;

    Z output00 = outputArray[yLowIndex     + xLowIndex  * xLength];
    Z output10 = outputArray[yLowIndex     + xHighIndex * xLength];
    Z output01 = outputArray[yLowIndex + 1 + xLowIndex  * xLength];
    Z output11 = outputArray[yLowIndex + 1 + xHighIndex * xLength];

    return interpolateBilinear(
        x, xLow, xHigh,
        y, yLow, yHigh,
        output00, output10, output01, output11);
  }
  else if (FindOnScaleResult::InBetween == yResult)
  {
    // We're in-between rows, but fully left, right, or on a column exactly
    // Need to interpolate between rows in a single column
    size_t yHighIndex = yLowIndex + 1;
    size_t output0Index = yLowIndex  * xLength + xLowIndex;
    size_t output1Index = yHighIndex * xLength + xLowIndex;

    Z output0 = outputArray[output0Index];
    Z output1 = outputArray[output1Index];

    return interpolateLinear(y, yLow, yHigh, output0, output1);
  }
  else if (FindOnScaleResult::InBetween == xResult)
  {
    // We're in-between columns, but fully top, bottom, or on a row exactly
    // Need to interpolate between columns in a single row
    size_t output0Index = yLowIndex * xLength + xLowIndex;

    Z output0 = outputArray[output0Index];
    Z output1 = outputArray[output0Index + 1];

    return interpolateLinear(x, xLow, xHigh, output0, output1);
  }
  else
  {
    size_t output0Index = yLowIndex * xLength + xLowIndex;
    return outputArray[output0Index];
  }
}

#endif