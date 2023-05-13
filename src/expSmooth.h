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

template<uint8_t alphaFracBits, typename TMul, typename TVal>
TVal expSmoothImpl(TVal cur, TVal prev, uint8_t alpha, uint8_t oneMinusAlpha)
{
  static constexpr TMul alphaFactor = 1u << alphaFracBits;
  static constexpr TMul roundingFactor = alphaFactor / 2u;

  return static_cast<TVal>(
    (static_cast<TMul>(cur) * alpha + static_cast<TMul>(prev) * oneMinusAlpha + roundingFactor)
    / alphaFactor);
}

template<uint8_t alphaFracBits = 6, typename TVal, uint8_t valueBits = sizeof(TVal) * 8 - 6>
TVal expSmooth(TVal cur, TVal prev, uint8_t alpha, uint8_t oneMinusAlpha)
{
  static_assert(valueBits + alphaFracBits <= 32);
  
  if (valueBits + alphaFracBits <= 16)
  {
    return expSmoothImpl<alphaFracBits, uint16_t>(cur, prev, alpha, oneMinusAlpha);
  }
#ifdef __AVR_ARCH__
  else if (valueBits + alphaFracBits <= 24)
  {
    return expSmoothImpl<alphaFracBits, __uint24>(cur, prev, alpha, oneMinusAlpha);
  }
#endif
  else if (valueBits + alphaFracBits <= 32)
  {
    return expSmoothImpl<alphaFracBits, uint32_t>(cur, prev, alpha, oneMinusAlpha);
  }
  else if (valueBits + alphaFracBits <= 64)
  {
    return expSmoothImpl<alphaFracBits, uint64_t>(cur, prev, alpha, oneMinusAlpha);
  }
  else
  {
    return 0;
  }
}

template<uint8_t alphaFracBits = 6, typename TVal, uint8_t valueBits = sizeof(TVal) * 8 - 6>
TVal expSmooth(TVal cur, TVal prev, uint8_t alpha)
{
  return expSmooth<alphaFracBits, TVal, valueBits>(cur, prev, alpha, (1u << alphaFracBits) - alpha);
}

uint8_t calculateAlphaFixed(float alphaFloat, uint8_t fractionBits = 6u);

#endif