#pragma once

#include <stdint.h>

float rnd();          // No argument
float rnd(float n);   // Single number

float _rnd(float x = 1.0f);
void _srand(uint32_t seed);