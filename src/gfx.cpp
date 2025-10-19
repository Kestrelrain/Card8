#include "gfx.h"
#include <M5Cardputer.h>
#include "mem.h"


extern const unsigned char font_data[];

const uint16_t palette[16] = {
    0x0000, // 0: Black
    0x112B, // 1: Dark Blue
    0x4912, // 2: Dark Purple
    0x042A, // 3: Dark Green
    0xA145, // 4: Brown
    0x7392, // 5: Dark Gray
    0xDEDB, // 6: Light Gray
    0xFFF3, // 7: White (PICO-8 white)
    0xF809, // 8: Red
    0xFD20, // 9: Orange
    0xFFE4, // 10: Yellow
    0x07F0, // 11: Green
    0x5EDB, // 12: Blue
    0x8413, // 13: Indigo
    0xFB52, // 14: Pink
    0xFD55  // 15: Peach
};
/*S
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
*/
void drawFramebuffer() {
    const int scale = 1;  // DEBUG ONLY
    const int scaledWidth = WIDTH * scale;
    const int scaledHeight = HEIGHT * scale;

    // Center the framebuffer on the 240x135 screen
    const int xOffset = (240 - scaledWidth) / 2;
    const int yOffset = (135 - scaledHeight) / 2;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            uint8_t color = pget(x, y);
            uint16_t rgb = palette[color];

            M5.Lcd.fillRect(
                xOffset + x * scale,
                yOffset + y * scale,
                scale, scale,
                rgb
            );
        }
    }
}

void pset(int x, int y, int color) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;

    int index = y * WIDTH + x;
    int byteIndex = index / 2;
    bool highNibble = (index % 2 == 0);

    if (highNibble) {
        memory[SCREEN_MEM + byteIndex] = (memory[SCREEN_MEM + byteIndex] & 0x0F) | (color << 4);
    } else {
        memory[SCREEN_MEM + byteIndex] = (memory[SCREEN_MEM + byteIndex] & 0xF0) | (color & 0x0F);
    }
}

uint8_t pget(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return 0;

    int index = y * WIDTH + x;
    int byteIndex = index / 2;
    bool highNibble = (index % 2 == 0);

    uint8_t byte = memory[SCREEN_MEM + byteIndex];
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

                    uint8_t byte = memory[GFX_MEM + byteIndex];
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



void cls(int col = 0) {
    Serial.print("cls() called with color index: "); //DEBUG
    Serial.println(col);
    col = col & 0x0F;
    uint8_t packed = (col << 4) | col;

    for (int i = 0; i < 0x2000; i++) {
        memory[SCREEN_MEM + i] = packed;
    }
}

