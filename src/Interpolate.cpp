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

#include "Interpolate.h"

template<typename SlopeType, uint8_t slopeShift = 0, typename InputType, typename OutputType>
SlopeType interpolateLinearFixedSigned(InputType input, InputType inputLow, InputType inputHigh, OutputType output0, OutputType output1)
{
  static_assert(sizeof(SlopeType) * 8 >= sizeof(OutputType) * 8 + slopeShift, "SlopeType too small");

  // Shift for fixed-point math
  constexpr SlopeType shiftMul = static_cast<SlopeType>(1ul << slopeShift);

  SlopeType slope = (static_cast<SlopeType>(output1 - output0) * shiftMul) / static_cast<SlopeType>(inputHigh - inputLow);
  return (slope * static_cast<SlopeType>(input - inputLow)) / static_cast<SlopeType>(shiftMul) + static_cast<SlopeType>(output0);
}

template<typename SlopeType, uint8_t slopeShift = 0, typename InputType, typename OutputType>
SlopeType interpolateLinearFixedUnsigned(InputType input, InputType inputLow, InputType inputHigh, OutputType output0, OutputType output1)
{
  static_assert(sizeof(SlopeType) * 8 >= sizeof(OutputType) * 8 + slopeShift, "SlopeType too small");

  // Shift for fixed-point math
  constexpr SlopeType shiftMul = static_cast<SlopeType>(1ul << slopeShift);

  if (output1 > output0)
  {
    SlopeType slope = (static_cast<SlopeType>(output1 - output0) * shiftMul) / static_cast<SlopeType>(inputHigh - inputLow);
    return (slope * static_cast<SlopeType>(input - inputLow)) / static_cast<SlopeType>(shiftMul) + static_cast<SlopeType>(output0);
  }
  else
  {
    // Assume we're dealing with unsigned fixed-point math
    SlopeType slope = (static_cast<SlopeType>(output0 - output1) * shiftMul) / static_cast<SlopeType>(inputHigh - inputLow);
    return static_cast<SlopeType>(output0) - (slope * static_cast<SlopeType>(input - inputLow)) / static_cast<SlopeType>(shiftMul);
  }
}

template<>
uint8_t interpolateLinear<uint8_t, uint8_t, uint8_t>(uint8_t input, uint8_t inputLow, uint8_t inputHigh, uint8_t output0, uint8_t output1)
{
  return interpolateLinearFixedUnsigned<uint16_t, 8>(input, inputLow, inputHigh, output0, output1);
}

template<>
float interpolateLinear<float, uint8_t, uint8_t>(uint8_t input, uint8_t inputLow, uint8_t inputHigh, uint8_t output0, uint8_t output1)
{
  return interpolateLinearFixedUnsigned<float>(input, inputLow, inputHigh, output0, output1);
}

/*
uint8_t interpolateLinear(uint8_t input, uint8_t inputLow, uint8_t inputHigh, uint8_t output0, uint8_t output1)
{
  return interpolateLinearFixedUnsigned<uint32_t, 8>(input, inputLow, inputHigh, output0, output1);
}

uint8_t interpolateLinearFloat(uint8_t input, uint8_t inputLow, uint8_t inputHigh, uint8_t output0, uint8_t output1)
{
  return interpolateLinearFixedUnsigned<float>(input, inputLow, inputHigh, output0, output1);
}

float interpolateLinear(float input, float inputLow, float inputHigh, float output0, float output1)
{
  float slope = (output1 - output0) / (inputHigh - inputLow);
  return slope * (input - inputLow) + output0;
}
*/