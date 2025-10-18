#ifndef PICO8_MEM_H
#define PICO8_MEM_H
#include <Arduino.h>
#pragma once

#define PICO8_MEM_SIZE 0x8000
extern uint8_t pico8_memory[PICO8_MEM_SIZE];

#define GFX_MEM      0x0000
#define MAP_MEM      0x2000
#define SCREEN_MEM   0x6000
#define DRAW_COLOR   0x5f25

#define currentColor pico8_memory[DRAW_COLOR]

#endif