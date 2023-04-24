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

#ifndef ENGINE_CALCULATIONS_LINEAR_ITERPOLATION_H_
#define ENGINE_CALCULATIONS_LINEAR_ITERPOLATION_H_

#pragma once

#include <stddef.h>
#include <stdint.h>


enum class FindOnScaleResult: uint8_t {InBetween, Exact, OffScaleHigh, OffScaleLow};

/**
 * @brief Find the value on the scale
 * 
 * @tparam T Type of value
 * @tparam ScaleArrayType Array-like type. Must be indexable []
 * @param start Array-like object that stores the scale values. Must be indexable []
 * @param length Length of scale
 * @param input input value
 * @param outLowIndex Index of value below the input, or 0 if off the scale low
 * @param outLow Value less than the input
 * @param outHigh Value greater than the input
 * @return FindOnScaleResult 
 */
template<typename T, typename ScaleArrayType>
FindOnScaleResult findOnScale(T input, ScaleArrayType start, size_t length, size_t &outLowIndex, T &outLow, T &outHigh)
{
  // Start search at end of scale
  // Optimizes for high RPM values, since they'll be found more first and in less time
  uint16_t index = length - 1;
  
  T curValue = start[index];

  // Input out of range high
  if (input > curValue)
  {
    outLow = curValue;
    outLowIndex = index;
    return FindOnScaleResult::OffScaleHigh;
  }

  // Found the exact value
  if (input == curValue)
  {
    outHigh = curValue;
    outLow = curValue;
    outLowIndex = index;
    return FindOnScaleResult::Exact;
  }

  T highValue;

  // Search for value in scale that is lower than input
  do
  {
    highValue = curValue;

    // Get current value
    index--;
    curValue = start[index];

    // Found the exact value
    if (input == curValue)
    {
      outHigh = curValue;
      outLow = curValue;
      outLowIndex = index;
      return FindOnScaleResult::Exact;
    }

    // Found the lower value on scale
    if (curValue < input)
    {
      outHigh = highValue;
      outLow = curValue;
      outLowIndex = index;
      return FindOnScaleResult::InBetween;
    }
  } while (index > 0);

  // Input is out of range low
  outHigh = curValue;
  outLowIndex = index;
  return FindOnScaleResult::OffScaleLow;
}

template<typename Z = float, typename X = float, typename Y = float, typename ReturnType = float,
  typename DeltaXMulZ = float, typename DeltaYMulZ = float>
ReturnType interpolateBilinearXFirst(X x, X x0, X x1, Y y, Y y0, Y y1, Z z00, Z z10, Z z01, Z z11)
{
  // Generally: y_interpolated = [ y0 * (x1 - x) + y1 * (x - x0) ] / (x1 - x0)

  // We're interpolating the rows first
  // Note: z subscripts are x,y. Ex: z01 is at x = 0, y = 1

  // Upcast so we don't overflow fixed-point math when we multiply
  ReturnType deltaX = static_cast<ReturnType>(x1 - x0);
  DeltaXMulZ deltaX0 = static_cast<DeltaXMulZ>(x - x0);
  DeltaXMulZ deltaX1 = static_cast<DeltaXMulZ>(x1 - x);

  // z_row0_interpolated = [ z00 * (x1 - x) + z10 * (x - x0) ] / (x1 - x0)
  DeltaYMulZ z_row0_num = static_cast<DeltaYMulZ>(static_cast<DeltaXMulZ>(z00) * deltaX1 + static_cast<DeltaXMulZ>(z10) * deltaX0);

  // z_row1_interpolated = [ z01 * (x1 - x) + z11 * (x - x0) ] / (x1 - x0)
  DeltaYMulZ z_row1_num = static_cast<DeltaYMulZ>(static_cast<DeltaXMulZ>(z01) * deltaX1 + static_cast<DeltaXMulZ>(z11) * deltaX0);

  // Denominators are the same
  ReturnType z_row_denom = deltaX;

  // Upcast so we don't overflow fixed-point math when we multiply
  ReturnType deltaY = static_cast<ReturnType>(y1 - y0);
  DeltaYMulZ deltaY0 = static_cast<DeltaYMulZ>(y - y0);
  DeltaYMulZ deltaY1 = static_cast<DeltaYMulZ>(y1 - y);

  // z_interpolated = [ z_row0_interpolated * (y1 - y) + z_row1_interpolated * (y - y0) ] / (y1 - y0)
  DeltaYMulZ z_num = z_row0_num * deltaY1 + z_row1_num * deltaY0;

  // Need to include the Z row denomizator
  ReturnType z_denom = deltaY * z_row_denom;
  
  return static_cast<ReturnType>(z_num) / z_denom;
}

template<typename Z = float, typename X = float, typename Y = float, typename ReturnType = float,
  typename DeltaXMulZ = float, typename DeltaYMulZ = float>
ReturnType interpolateBilinearYFirst(X x, X x0, X x1, Y y, Y y0, Y y1, Z z00, Z z10, Z z01, Z z11)
{
  // Rotate 90 degrees
  return interpolateBilinearXFirst(y, y0, y1, x, x0, x1, z00, z01, z10, z11);
}

/*
For fixed point math, DeltaXMulZ needs to be big enough to hold any deltaX * Z, and DeltaYMulZ needs
to be big enough to hold any deltaY * Z. The larger of the two must also be able to hold 
deltaX * deltaY * z
 */
template<typename Z = float, typename X = float, typename Y = float, typename ReturnType = float,
  typename DeltaXMulZ = float, typename DeltaYMulZ = float>
ReturnType interpolateBilinear(X x, X x0, X x1, Y y, Y y0, Y y1, Z z00, Z z10, Z z01, Z z11)
{
  if (sizeof(DeltaYMulZ) >= sizeof(DeltaXMulZ))
  {
    return interpolateBilinearXFirst<Z, X, Y, ReturnType, DeltaXMulZ, DeltaYMulZ>(
      x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  }
  else
  {
    return interpolateBilinearYFirst<Z, X, Y, ReturnType, DeltaXMulZ, DeltaYMulZ>(
      x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  }
}

template<typename Z, typename ReturnType = float,
  typename DeltaXMulZ = float, typename DeltaYMulZ = float, typename SlopeType = float,
  typename X, typename Y, 
  typename XArray, typename YArray, typename ZArray>
ReturnType interpolateBilinearTable(X x, Y y, size_t xLength, size_t yLength,
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

    size_t output0Index = yLowIndex + xLowIndex  * xLength;
    size_t output1Index = yLowIndex + xHighIndex * xLength;

    Z output00 = outputArray[output0Index];
    Z output10 = outputArray[output0Index + 1];
    Z output01 = outputArray[output1Index];
    Z output11 = outputArray[output1Index + 1];

    return interpolateBilinear<Z, X, Y, ReturnType, DeltaXMulZ, DeltaYMulZ>(
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

    Z output00 = outputArray[output0Index];
    Z output01 = outputArray[output1Index];

    SlopeType slope = static_cast<SlopeType>(output01 - output00) / static_cast<SlopeType>(yHigh - yLow);

    return static_cast<ReturnType>(slope) * static_cast<ReturnType>(y - yLow) + static_cast<ReturnType>(output00);
  }
  else if (FindOnScaleResult::InBetween == xResult)
  {
    // We're in-between columns, but fully top, bottom, or on a row exactly
    // Need to interpolate between columns in a single row
    size_t output0Index = yLowIndex * xLength + xLowIndex;

    Z output00 = outputArray[output0Index];
    Z output10 = outputArray[output0Index + 1];

    SlopeType slope = static_cast<SlopeType>(output10 - output00) / static_cast<SlopeType>(xHigh - xLow);

    return static_cast<ReturnType>(slope) * static_cast<ReturnType>(x - xLow) + static_cast<ReturnType>(output00);
  }
  else
  {
    size_t output0Index = yLowIndex * xLength + xLowIndex;
    return static_cast<ReturnType>(outputArray[output0Index]);
  }
}

#endif