// Types
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

#ifndef ENGINE_CALCULATIONS_TYPES_H_
#define ENGINE_CALCULATIONS_TYPES_H_

#pragma once

#include <stdint.h>

#ifdef ANGLE_TYPE

using angle_t = (ANGLE_TYPE);

#else // ANGLE_TYPE

using angle_t = float;

#endif // ANGLE_TYPE

using ticks_t = uint32_t;

#endif