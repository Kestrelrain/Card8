#include "gfx.h"
#include <M5Cardputer.h>
#include "pico8_mem.h"


const uint16_t pico8_palette[16] = {
    0x0000, 0x1D34, 0x2589, 0x5AC9,
    0xA4D1, 0xD4C1, 0xC5EC, 0xFFF5,
    0xF800, 0xFA60, 0xFFEC, 0x5C2C,
    0x6CF0, 0xEF00, 0xFBA0, 0xD5AD
};

void drawFramebuffer() {
    const int xOffset = (240 - WIDTH) / 2;
    const int yOffset = (135 - HEIGHT) / 2;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            uint8_t color = pget(x, y);
            M5.Lcd.drawPixel(x + xOffset, y + yOffset, pico8_palette[color]);
        }
    }
}


void pset(int x, int y, int color) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;

    int index = y * WIDTH + x;
    int byteIndex = index / 2;
    bool highNibble = (index % 2 == 0);

    if (highNibble) {
        pico8_memory[SCREEN_MEM + byteIndex] = (pico8_memory[SCREEN_MEM + byteIndex] & 0x0F) | (color << 4);
    } else {
        pico8_memory[SCREEN_MEM + byteIndex] = (pico8_memory[SCREEN_MEM + byteIndex] & 0xF0) | (color & 0x0F);
    }
}

uint8_t pget(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return 0;

    int index = y * WIDTH + x;
    int byteIndex = index / 2;
    bool highNibble = (index % 2 == 0);

    uint8_t byte = pico8_memory[SCREEN_MEM + byteIndex];
    return highNibble ? (byte >> 4) & 0x0F : byte & 0x0F;
}

void spr(int n, int dx, int dy, int w, int h, bool flip_x, bool flip_y) {
    int sx = (n % 16) * 8;
    int sy = (n / 16) * 8;

    for (int tileY = 0; tileY < h; tileY++) {
        for (int tileX = 0; tileX < w; tileX++) {
            for (int y = 0; y < 8; y++) {
                for (int x = 0; x < 8; x++) {
                    int srcX = sx + tileX * 8 + (flip_x ? 7 - x : x);
                    int srcY = sy + tileY * 8 + (flip_y ? 7 - y : y);

                    int srcIndex = srcY * 128 + srcX;
                    int byteIndex = srcIndex / 2;
                    bool highNibble = (srcIndex % 2 == 0);

                    uint8_t byte = pico8_memory[GFX_MEM + byteIndex];
                    uint8_t color = highNibble ? (byte >> 4) & 0x0F : byte & 0x0F;

                    if (color != 0) {
                        pset(dx + tileX * 8 + x, dy + tileY * 8 + y, color);
                    }
                }
            }
        }
    }
}

void flip(){
    drawFramebuffer();   // Update the display
    delay(33);           // ~30 FPS (1000ms / 30 = ~33.3ms)
}

float rnd() {
    return (float)random(0, 0x10000) / 0x10000;  // ~0.000015 resolution
}

void cls(int col) {
    col = col & 0x0F;
    uint8_t packed = (col << 4) | col;

    for (int i = 0; i < 0x2000; i++) {
        pico8_memory[SCREEN_MEM + i] = packed;
    }
}

float rnd(float n) {
    return rnd() * n;
}