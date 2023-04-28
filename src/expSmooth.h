// Exponential Smoothing
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

#ifndef ENGINE_CALCULATIONS_EXP_SMOOTH_H_
#define ENGINE_CALCULATIONS_EXP_SMOOTH_H_

#pragma once

#include <stdint.h>
#include <avr/common.h>

template<typename T, uint8_t alphaFracBits>
uint16_t expSmoothImpl(uint16_t cur, uint16_t prev, uint8_t alpha, uint8_t oneMinusAlpha)
{
  static constexpr uint16_t alphaFactor = 1 << alphaFracBits;
  static constexpr uint16_t roundingFactor = alphaFactor / 2;

  return (static_cast<T>(cur) * alpha + static_cast<T>(prev) * oneMinusAlpha + roundingFactor) / alphaFactor;
}

template<uint8_t valueBits, uint8_t alphaFracBits>
uint16_t expSmooth(uint16_t cur, uint16_t prev, uint8_t alpha, uint8_t oneMinusAlpha)
{
  static_assert(valueBits + alphaFracBits <= 32);
  
  if (valueBits + alphaFracBits <= 16)
  {
    return expSmoothImpl<uint16_t, alphaFracBits>(cur, prev, alpha, oneMinusAlpha);
  }
#ifdef __AVR_ARCH__
  else if (valueBits + alphaFracBits <= 24)
  {
    return expSmoothImpl<__uint24, alphaFracBits>(cur, prev, alpha, oneMinusAlpha);
  }
#endif
  else if (valueBits + alphaFracBits <= 32)
  {
    return expSmoothImpl<uint32_t, alphaFracBits>(cur, prev, alpha, oneMinusAlpha);
  }
  else
  {
    return 0;
  }
}

template<uint8_t valueBits = 10, uint8_t alphaFracBits = 6>
uint16_t expSmooth(uint16_t cur, uint16_t prev, uint8_t alpha)
{
  return expSmooth<valueBits, alphaFracBits>(cur, prev, alpha, (1 << alphaFracBits) - alpha);
}

#endif