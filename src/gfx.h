#pragma once

#include <Arduino.h>

extern int cameraX;
extern int cameraY;
extern int cursorX;
extern int cursorY;

// Constants
#define WIDTH 128
#define HEIGHT 128



// Function declarations
void drawFramebuffer();
void pset(int x, int y, int col);
uint8_t pget(int x, int y);
void spr(int n, int dx, int dy, int w, int h, bool flip_x, bool flip_y);
void clip(int x, int y, int w, int h);
void color(int col);
uint8_t sign(int x);
void cls(int col);
void flip();
bool btn(int i, int p);
bool btnp(int i, int p);
void pal(int c0, int c1);
void camera(int x, int y);
void circfill(int x0, int y0, int r, int col);
void rectfill(int x1, int y1, int x2, int y2, int col);
void rect(int x1, int y1, int x2, int y2, int col);
void line(int x0, int y0, int x1, int y1, int col);
void print(const char* str,int x, int y,int col );

// External framebuffer and palette
extern uint8_t memory[];
extern const uint16_t palette[16];
