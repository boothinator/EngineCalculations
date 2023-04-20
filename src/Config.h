// Config
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

#ifndef CONFIG_H_
#define CONFIG_H_

#pragma once

#ifdef TICKS_PER_SECOND

static constexpr float ticksPerSecond = static_cast<float>(TICKS_PER_SECOND);

#define TICKS_PER_SECOND_ATTR static constexpr

#else // TICKS_PER_SECOND

#define TICKS_PER_SECOND_ATTR

extern float ticksPerSecond;

#endif // TICKS_PER_SECOND

#endif // CONFIG_H_