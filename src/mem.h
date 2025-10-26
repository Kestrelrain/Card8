#ifndef MEM_H
#define MEM_H
#include <Arduino.h>
#pragma once

#define CARD_MEM_SIZE 0x8000
extern uint8_t memory[CARD_MEM_SIZE];


#define GFX_MEM         0x0000   // 0x0000 - 0x0FFF (Sprite sheet 0)
#define GFX2_MEM        0x1000   // 0x1000 - 0x1FFF (Shared: Sprite sheet 1 or Map segment 2)
#define MAP_MEM         0x2000   // 0x2000 - 0x2FFF (Map 128x64 = 8192 bytes)
#define GFX_FLAGS_MEM   0x3000   // 0x3000 - 0x30FF (Sprite flags, 256 bytes)
#define SONG_MEM        0x3100   // 0x3100 - 0x31FF (Music data)
#define SFX_MEM         0x3200   // 0x3200 - 0x42FF (Sound effects)
#define USER_MEM        0x4300   // 0x4300 - 0x5DFF (Reserved for user data)
#define PERSIST_MEM     0x5E00   // 0x5E00 - 0x5EFF (Persistent cart data, 256 bytes)
#define DRAW_STATE_MEM  0x5F00   // 0x5F00 - 0x5F3F (Draw state variables)
#define DRAW_COLOR      0x5f25
#define RNG_MEM         0x5F44   // RNG state (PICO-8 stores this here)
#define GPIO_MEM        0x5F80   // 0x5F80 - 0x5FFF (GPIO pins, 128 bytes)
#define SCREEN_MEM      0x6000   // 0x6000 - 0x7FFF (Screen buffer, 128x128, 4bpp = 8K)

#define currentColor memory[DRAW_COLOR]
//#define spriteFlags memory[GFX_FLAGS_MEM]



uint16_t read_u16(uint16_t addr);
void write_u16(uint16_t addr, uint16_t val);
uint32_t read_u32(uint16_t addr);
void write_u32(uint16_t addr, uint32_t val);


#endif