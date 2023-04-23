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

template<typename Z, typename X, typename Y, 
  typename XArray, typename YArray, typename ZArray, typename ReturnType = Z,
  typename DeltaXMulZ = float, typename DeltaYMulZ = float, typename SlopeType = float>
ReturnType linearInterp2d(X x, Y y, size_t xLength, size_t yLength,
                          ZArray outputArray, XArray xScale, YArray yScale)
{
  size_t xLowIndex;
  X xLow;
  X yHigh;

  FindOnScaleResult bottomResult = findOnScale(x, xScale, xLength,  xLowIndex, xLow, yHigh);

  size_t yLowIndex;
  Y yLow;
  Y yHigh;

  FindOnScaleResult leftResult = findOnScale(y, yScale, yLength, yLowIndex, yLow, yHigh);

  if (FindOnScaleResult::InBetween == leftResult
    && FindOnScaleResult::InBetween == bottomResult)
  {
    size_t xHighIndex = xLowIndex + 1;

    size_t output0Index = yLowIndex + xLowIndex  * xScale.getLength();
    size_t output1Index = yLowIndex + xHighIndex * xScale.getLength();

    Z output00 = outputArray[output0Index];
    Z output10 = outputArray[output0Index + 1];
    Z output01 = outputArray[output1Index];
    Z output11 = outputArray[output1Index + 1];

    return linearInterpolation2D<Z, X, Y, ReturnType, DeltaXMulZ, DeltaYMulZ>(
        x, xLow, yHigh,
        y, yLow, yHigh,
        output00, output10, output01, output11);
  }
  else if (FindOnScaleResult::InBetween == leftResult)
  {
    // We're in-between rows, but fully left, right, or on a column exactly
    // Need to interpolate between rows in a single column
    size_t leftHighIndex = yLowIndex + 1;
    size_t output0Index = yLowIndex  * xScale.getLength() + xLowIndex;
    size_t output1Index = leftHighIndex * xScale.getLength() + xLowIndex;

    Z output00 = outputArray[output0Index];
    Z output10 = outputArray[output1Index];

    SlopeType slope = calculateSlope<SlopeType>(output00, output10, yLow, yHigh);

    return static_cast<ReturnType>(linpoly(slope, leftInput - yLow, output00));
  }
  else if (FindOnScaleResult::InBetween == bottomResult)
  {
    // We're in-between columns, but fully top, bottom, or on a row exactly
    // Need to interpolate between columns in a single row
    size_t output0Index = yLowIndex * xScale.getLength() + xLowIndex;

    Z output00 = outputArray[output0Index];
    Z output01 = outputArray[output0Index + 1];

    SlopeType slope = calculateSlope<SlopeType>(output00, output01, xLow, yHigh);

    return static_cast<ReturnType>(linpoly(slope, bottomInput - xLow, output00));
  }
  else
  {
    size_t output0Index = yLowIndex * xScale.getLength() + xLowIndex;
    return static_cast<ReturnType>(outputGetter.getIndexed(output0Index));
  }
}

#endif