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
void print(const char* str,int x, int y,int col );

// External framebuffer and palette
extern uint8_t memory[];
extern const uint16_t palette[16];
