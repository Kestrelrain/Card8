#pragma once

#include <stdint.h>

float rnd();          // No argument
float rnd(float n);   // Single number

float pico8_rnd(float x = 1.0f);
void pico8_srand(uint32_t seed);