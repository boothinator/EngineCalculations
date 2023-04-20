// Test template
// Copyright (C) 2021  Joshua Booth

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

#include <Arduino.h>
#include <unity.h>

#include "EngineCalculations.h"

#define MAX_MESSAGE_LEN 255

char message[MAX_MESSAGE_LEN];

void setUp(void) {
}

void tearDown(void) {
// clean stuff up here
}


void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(200);

  UNITY_BEGIN();    // IMPORTANT LINE!

  // Set Timer 1 to run in normal counting mode (no PWM, rollover to 0)
  TCCR1A = 0;
  // Set Timer 1 to use no prescaling (run in sync with system clock)
  TCCR1B = 1;

  UNITY_END(); // stop unit testing
}

void loop() {
}

