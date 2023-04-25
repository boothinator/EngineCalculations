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

template<typename ArrayT>
bool inAscendingOrder(ArrayT arr, size_t length)
{
  auto prev = arr[0];
  for (uint16_t i = 1; i < length; i++)
  {
    auto cur = arr[i];

    if (prev > cur)
    {
      return false;
    }
  }

  return true;
}

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

// Base case for float, double, or signed custom types that support + - / *
template<typename InputType, typename OutputType, typename SlopeType = OutputType>
SlopeType interpolateLinear(InputType input, InputType inputLow, InputType inputHigh, OutputType output0, OutputType output1)
{
  SlopeType slope = static_cast<SlopeType>(output1 - output0) / static_cast<SlopeType>(inputHigh - inputLow);
  return slope * static_cast<SlopeType>(input - inputLow) + static_cast<SlopeType>(output0);
}

// Base case for float, double, or signed custom types that support + - / *
template<typename InputType, typename OutputType, typename SlopeType = OutputType>
SlopeType interpolateLinearUnsigned(InputType input, InputType inputLow, InputType inputHigh, OutputType output0, OutputType output1)
{
  if (output1 > output0)
  {
    return interpolateLinear<InputType, OutputType, SlopeType>(input, inputLow, inputHigh, output0, output1);
  }

  SlopeType slope = static_cast<SlopeType>(output0 - output1) / static_cast<SlopeType>(inputHigh - inputLow);
  return static_cast<SlopeType>(output0) - slope * static_cast<SlopeType>(input - inputLow);
}

template<>
uint8_t interpolateLinear<uint8_t, uint8_t>(uint8_t input, uint8_t inputLow, uint8_t inputHigh, uint8_t output0, uint8_t output1);

template<>
uint16_t interpolateLinear<uint8_t, uint16_t>(uint8_t input, uint8_t inputLow, uint8_t inputHigh, uint16_t output0, uint16_t output1);

template<>
uint32_t interpolateLinear<uint8_t, uint32_t>(uint8_t input, uint8_t inputLow, uint8_t inputHigh, uint32_t output0, uint32_t output1);

template<>
uint8_t interpolateLinear<uint16_t, uint8_t>(uint16_t input, uint16_t inputLow, uint16_t inputHigh, uint8_t output0, uint8_t output1);

template<>
uint16_t interpolateLinear<uint16_t, uint16_t>(uint16_t input, uint16_t inputLow, uint16_t inputHigh, uint16_t output0, uint16_t output1);

template<>
uint32_t interpolateLinear<uint16_t, uint32_t>(uint16_t input, uint16_t inputLow, uint16_t inputHigh, uint32_t output0, uint32_t output1);

template<>
uint8_t interpolateLinear<uint32_t, uint8_t>(uint32_t input, uint32_t inputLow, uint32_t inputHigh, uint8_t output0, uint8_t output1);

template<>
uint16_t interpolateLinear<uint32_t, uint16_t>(uint32_t input, uint32_t inputLow, uint32_t inputHigh, uint16_t output0, uint16_t output1);

template<>
uint32_t interpolateLinear<uint32_t, uint32_t>(uint32_t input, uint32_t inputLow, uint32_t inputHigh, uint32_t output0, uint32_t output1);


template<typename SlopeType = float,
  typename InputType, typename InputArray, typename OutputArray>
SlopeType interpolateLinearTable(InputType input, size_t length, InputArray inputScale, OutputArray outputArray)
{
  size_t index;
  InputType inputLow, inputHigh;

  FindOnScaleResult result = findOnScale(input, inputScale, length, index, inputLow, inputHigh);

  // Need to interpolate
  if (FindOnScaleResult::InBetween == result)
  {
    auto output0 = outputArray[index];
    auto output1 = outputArray[index + 1];

    return interpolateLinear<InputType, decltype(output0), SlopeType>(input, inputLow, inputHigh, output0, output1);
  }
  else
  {
    return static_cast<SlopeType>(outputArray[index]);
  }
}

template<typename ReturnType = float, typename DeltaXMulZ = float, typename DeltaYMulZ = float,
  typename X = float, typename Y = float, typename Z = float>
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

template<typename ReturnType = float, typename DeltaXMulZ = float, typename DeltaYMulZ = float,
  typename X = float, typename Y = float, typename Z = float>
ReturnType interpolateBilinearYFirst(X x, X x0, X x1, Y y, Y y0, Y y1, Z z00, Z z10, Z z01, Z z11)
{
  // Rotate 90 degrees
  return interpolateBilinearXFirst<ReturnType, DeltaYMulZ, DeltaXMulZ>(y, y0, y1, x, x0, x1, z00, z01, z10, z11);
}

/*
For fixed point math, DeltaXMulZ needs to be big enough to hold any deltaX * Z, and DeltaYMulZ needs
to be big enough to hold any deltaY * Z. The larger of the two must also be able to hold 
deltaX * deltaY * z
 */
template<typename ReturnType = float, typename DeltaXMulZ = float, typename DeltaYMulZ = float,
  typename X = float, typename Y = float, typename Z = float>
ReturnType interpolateBilinear(X x, X x0, X x1, Y y, Y y0, Y y1, Z z00, Z z10, Z z01, Z z11)
{
  if (sizeof(DeltaYMulZ) >= sizeof(DeltaXMulZ))
  {
    return interpolateBilinearXFirst<ReturnType, DeltaXMulZ, DeltaYMulZ>(
      x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  }
  else
  {
    return interpolateBilinearYFirst<ReturnType, DeltaXMulZ, DeltaYMulZ>(
      x, x0, x1, y, y0, y1, z00, z10, z01, z11);
  }
}

template<typename Z, typename ReturnType = float,
  typename DeltaXMulZ = float, typename DeltaYMulZ = float,
  typename SlopeType = float, uint8_t slopeShift = 0,
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

    Z output00 = outputArray[yLowIndex     + xLowIndex  * xLength];
    Z output10 = outputArray[yLowIndex     + xHighIndex * xLength];
    Z output01 = outputArray[yLowIndex + 1 + xLowIndex  * xLength];
    Z output11 = outputArray[yLowIndex + 1 + xHighIndex * xLength];

    return interpolateBilinear<ReturnType, DeltaXMulZ, DeltaYMulZ>(
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

    return interpolateLinear<Y, Z, SlopeType>(y, yLow, yHigh, output0, output1);
  }
  else if (FindOnScaleResult::InBetween == xResult)
  {
    // We're in-between columns, but fully top, bottom, or on a row exactly
    // Need to interpolate between columns in a single row
    size_t output0Index = yLowIndex * xLength + xLowIndex;

    Z output0 = outputArray[output0Index];
    Z output1 = outputArray[output0Index + 1];

    return interpolateLinear<X, Z, SlopeType>(x, xLow, xHigh, output0, output1);
  }
  else
  {
    size_t output0Index = yLowIndex * xLength + xLowIndex;
    return static_cast<ReturnType>(outputArray[output0Index]);
  }
}

#endif