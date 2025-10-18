#pragma once

#include <Arduino.h>

// Constants
#define WIDTH 128
#define HEIGHT 128

// Function declarations
void drawFramebuffer();
void pset(int x, int y, int col);
uint8_t pget(int x, int y);
void spr(int n, int dx, int dy, int w, int h, bool flip_x, bool flip_y);
void cls(int col);
void flip();
float rnd();          // No argument
float rnd(float n);   // Single number

// External framebuffer and palette
extern uint8_t pico8_memory[];
extern const uint16_t pico8_palette[16];
