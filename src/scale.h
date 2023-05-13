// Scale
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

#ifndef ENGINE_CALCULATIONS_SCALE_H_
#define ENGINE_CALCULATIONS_SCALE_H_

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

    prev = cur;
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


#endif