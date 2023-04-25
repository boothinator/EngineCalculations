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

#ifndef ENGINE_CALCULATIONS_INTERPOLATE_LINEAR_H_
#define ENGINE_CALCULATIONS_INTERPOLATE_LINEAR_H_

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "scale.h"

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
    SlopeType slope = static_cast<SlopeType>(output1 - output0) / static_cast<SlopeType>(inputHigh - inputLow);
    return static_cast<SlopeType>(slope * static_cast<SlopeType>(input - inputLow) + static_cast<SlopeType>(output0));
  }
  else 
  {
    SlopeType slope = static_cast<SlopeType>(output0 - output1) / static_cast<SlopeType>(inputHigh - inputLow);
    return static_cast<SlopeType>(output0) - slope * static_cast<SlopeType>(input - inputLow);
  }
}

#define max(a,b) ((a)>(b)?(a):(b))

template<typename SlopeType, uint8_t slopeShift = 0, typename InputType, typename OutputType>
SlopeType interpolateLinearFixedSigned(InputType input, InputType inputLow, InputType inputHigh, OutputType output0, OutputType output1)
{
  // SlopeType needs to be big enough to handle OutputType / InputType
  static_assert(sizeof(SlopeType) * 8 >= max(sizeof(OutputType) * 8 + slopeShift, sizeof(InputType) * 8), "SlopeType too small");

  // Shift for fixed-point math
  constexpr SlopeType shiftMul = static_cast<SlopeType>(1ul << slopeShift);

  SlopeType slope = (static_cast<SlopeType>(output1 - output0) * shiftMul) / static_cast<SlopeType>(inputHigh - inputLow);
  return (slope * static_cast<SlopeType>(input - inputLow)) / static_cast<SlopeType>(shiftMul) + static_cast<SlopeType>(output0);
}

template<typename SlopeType, uint8_t slopeShift = 0, typename InputType, typename OutputType>
SlopeType interpolateLinearFixedUnsigned(InputType input, InputType inputLow, InputType inputHigh, OutputType output0, OutputType output1)
{
  // SlopeType needs to be big enough to handle OutputType / InputType
  static_assert(sizeof(SlopeType) * 8 >= max(sizeof(OutputType) * 8 + slopeShift, sizeof(InputType) * 8), "SlopeType too small");

  // Shift for fixed-point math
  constexpr SlopeType shiftMul = static_cast<SlopeType>(1ul << slopeShift);

  // Add 1 to the highest digit that we're thowing away to round instead of truncating
  constexpr SlopeType round = slopeShift > 0 ? static_cast<SlopeType>(1ul << (slopeShift - 1)) : 0;

  if (output1 > output0)
  {
    SlopeType slope = (static_cast<SlopeType>(output1 - output0) * shiftMul) / static_cast<SlopeType>(inputHigh - inputLow);
    return (slope * static_cast<SlopeType>(input - inputLow) + round) / static_cast<SlopeType>(shiftMul) + static_cast<SlopeType>(output0);
  }
  else
  {
    // Assume we're dealing with unsigned fixed-point math
    SlopeType slope = (static_cast<SlopeType>(output0 - output1) * shiftMul) / static_cast<SlopeType>(inputHigh - inputLow);
    return static_cast<SlopeType>(output0) - (slope * static_cast<SlopeType>(input - inputLow) + round) / static_cast<SlopeType>(shiftMul);
  }
}

#undef max

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

#endif