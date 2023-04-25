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

#include "interpolateLinear.h"

template<>
uint8_t interpolateLinear<uint8_t, uint8_t>(uint8_t input, uint8_t inputLow, uint8_t inputHigh, uint8_t output0, uint8_t output1)
{
  return static_cast<uint8_t>(interpolateLinearFixedUnsigned<uint16_t, 8>(input, inputLow, inputHigh, output0, output1));
}

template<>
uint16_t interpolateLinear<uint8_t, uint16_t>(uint8_t input, uint8_t inputLow, uint8_t inputHigh, uint16_t output0, uint16_t output1)
{
  return static_cast<uint16_t>(interpolateLinearFixedUnsigned<uint32_t, 8>(input, inputLow, inputHigh, output0, output1));
}

template<>
uint32_t interpolateLinear<uint8_t, uint32_t>(uint8_t input, uint8_t inputLow, uint8_t inputHigh, uint32_t output0, uint32_t output1)
{
  // Floating point is faster than fixed point
  return static_cast<uint32_t>(interpolateLinearUnsigned<uint8_t, uint32_t, float>(input, inputLow, inputHigh, output0, output1) + 0.5);
}

template<>
uint8_t interpolateLinear<uint16_t, uint8_t>(uint16_t input, uint16_t inputLow, uint16_t inputHigh, uint8_t output0, uint8_t output1)
{
  return static_cast<uint16_t>(interpolateLinearFixedUnsigned<uint32_t, 8>(input, inputLow, inputHigh, output0, output1));
}

template<>
uint16_t interpolateLinear<uint16_t, uint16_t>(uint16_t input, uint16_t inputLow, uint16_t inputHigh, uint16_t output0, uint16_t output1)
{
  return static_cast<uint16_t>(interpolateLinearFixedUnsigned<uint32_t, 8>(input, inputLow, inputHigh, output0, output1));
}

template<>
uint32_t interpolateLinear<uint16_t, uint32_t>(uint16_t input, uint16_t inputLow, uint16_t inputHigh, uint32_t output0, uint32_t output1)
{
  // Floating point is faster than fixed point
  return static_cast<uint32_t>(interpolateLinearUnsigned<uint16_t, uint32_t, float>(input, inputLow, inputHigh, output0, output1) + 0.5);
}

template<>
uint8_t interpolateLinear<uint32_t, uint8_t>(uint32_t input, uint32_t inputLow, uint32_t inputHigh, uint8_t output0, uint8_t output1)
{
  // Floating point is faster than fixed point
  return static_cast<uint8_t>(interpolateLinearUnsigned<uint32_t, uint8_t, float>(input, inputLow, inputHigh, output0, output1) + 0.5);
}

template<>
uint16_t interpolateLinear<uint32_t, uint16_t>(uint32_t input, uint32_t inputLow, uint32_t inputHigh, uint16_t output0, uint16_t output1)
{
  // Floating point is faster than fixed point
  return static_cast<uint16_t>(interpolateLinearUnsigned<uint32_t, uint16_t, float>(input, inputLow, inputHigh, output0, output1) + 0.5);
}

template<>
uint32_t interpolateLinear<uint32_t, uint32_t>(uint32_t input, uint32_t inputLow, uint32_t inputHigh, uint32_t output0, uint32_t output1)
{
  // Floating point is faster than fixed point
  return static_cast<uint32_t>(interpolateLinearUnsigned<uint32_t, uint32_t, float>(input, inputLow, inputHigh, output0, output1) + 0.5);
}
