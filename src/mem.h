#ifndef MEM_H
#define MEM_H
#include <Arduino.h>
#pragma once

#define CARD_MEM_SIZE 0x8000
extern uint8_t memory[CARD_MEM_SIZE];

#define GFX_MEM      0x0000
#define MAP_MEM      0x2000
#define SCREEN_MEM   0x6000
#define DRAW_COLOR   0x5f25
#define RNG_MEM 0x5f44  // Start of RNG state in PICO-8 memory

#define currentColor memory[DRAW_COLOR]



uint16_t read_u16(uint16_t addr);
void write_u16(uint16_t addr, uint16_t val);
uint32_t read_u32(uint16_t addr);
void write_u32(uint16_t addr, uint32_t val);


#endif