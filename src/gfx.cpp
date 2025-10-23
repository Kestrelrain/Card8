#include "gfx.h"
#include <M5Cardputer.h>
#include "mem.h"


int cameraX = 0;
int cameraY = 0;

int cursorX = 0;
int cursorY = 0;

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

uint8_t paletteRemap[16] = {
    0, 1, 2, 3, 4, 5, 6, 7,
    8, 9, 10, 11, 12, 13, 14, 15
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
void camera(int x, int y) {
    cameraX = x;
    cameraY = y;
}
void pal(int c0, int c1) {
    if (c0 < 0 || c0 >= 16) return;
    if (c1 < 0 || c1 >= 16) return;
    paletteRemap[c0] = c1;
}
void pal_reset() {
    for (int i = 0; i < 16; i++) {
        paletteRemap[i] = i;
    }
}


void drawFramebuffer() {
    static uint16_t lcdBuffer[240 * 135];  // RGB565 buffer for full screen

    int i = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            // Convert screen coordinates (x, y) to world coordinates
            int srcX = x;
            int srcY = y;

            // Read from framebuffer using world coordinates
            uint8_t color = 0;

            if (srcX >= 0 && srcX < WIDTH && srcY >= 0 && srcY < HEIGHT) {
                color = pget(srcX, srcY);
            }

            // Remap and convert color
            uint8_t remappedColor = paletteRemap[color];
            //lcdBuffer[i++] = palette[color];
            uint16_t rgb = palette[remappedColor];
            lcdBuffer[y * WIDTH + x] = (rgb >> 8) | (rgb << 8);  // Swap bytes

        }
    }

    // Center the image on 240x135 screen
    int xOffset = (240 - WIDTH) / 2;
    int yOffset = (135 - HEIGHT) / 2;

    // Push the buffer to the screen
    M5.Lcd.pushImage(0, 0, WIDTH, HEIGHT, lcdBuffer);
}

/*
void drawFramebuffer() {
    const int scale = 1; // or whatever scale you want
    const int scaledWidth = WIDTH * scale;
    const int scaledHeight = HEIGHT * scale;
    static uint16_t lcdBuffer[240 * 135];  // RGB565 buffer for full screen

    const int xOffset = (240 - scaledWidth) / 2;
    const int yOffset = (135 - scaledHeight) / 2;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // Calculate source pixel from framebuffer based on camera
            int srcX = x + cameraX;
            int srcY = y + cameraY;

            uint8_t color = 0;
            if (srcX >= 0 && srcX < WIDTH && srcY >= 0 && srcY < HEIGHT) {
                color = pget(srcX, srcY);
            }

            uint16_t rgb = palette[color];
            M5.Lcd.fillRect(
                x * scale,
                y * scale,
                scale, scale,
                rgb
            );
        }
    }
}
*/
/*
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
*/
void pset(int worldX, int worldY, int color) {
    int screenX = worldX - cameraX;
    int screenY = worldY - cameraY;

    if (screenX < 0 || screenX >= WIDTH || screenY < 0 || screenY >= HEIGHT) return;

    int index = screenY * WIDTH + screenX;
    int byteIndex = index / 2;
    bool highNibble = (index % 2 == 0);

    if (highNibble) {
        memory[SCREEN_MEM + byteIndex] = (memory[SCREEN_MEM + byteIndex] & 0x0F) | (color << 4);
    } else {
        memory[SCREEN_MEM + byteIndex] = (memory[SCREEN_MEM + byteIndex] & 0xF0) | (color & 0x0F);
    }
}

uint8_t pget(int x, int y) {



    //if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return 0;

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


void cursor(int x, int y) {
    cursorX = x;
    cursorY = y;
}


void color(uint8_t col) {
    if (col < 16) {
        currentColor = col & 0x0F;
    }
}


void oval(int x0, int y0, int x1, int y1, int col) {

    int a = abs(x1 - x0) / 2;
    int b = abs(y1 - y0) / 2;
    int xc = (x0 + x1) / 2;
    int yc = (y0 + y1) / 2;

    for (float t = 0; t < 2 * PI; t += 0.01f) {
        int x = xc + a * cos(t);
        int y = yc + b * sin(t);
        pset(x, y, col);
    }
}


void ovalfill(int x0, int y0, int x1, int y1, int col) {

    int a = abs(x1 - x0) / 2;
    int b = abs(y1 - y0) / 2;
    int xc = (x0 + x1) / 2;
    int yc = (y0 + y1) / 2;

    for (int y = -b; y <= b; y++) {
        for (int x = -a; x <= a; x++) {
            if ((x * x * b * b + y * y * a * a) <= a * a * b * b) {
                pset(xc + x, yc + y, col);
            }
        }
    }
}


void circ(int x0, int y0, int r, int col) {

    int x = r;
    int y = 0;
    int err = 0;

    while (x >= y) {
        pset(x0 + x, y0 + y, col);
        pset(x0 + y, y0 + x, col);
        pset(x0 - y, y0 + x, col);
        pset(x0 - x, y0 + y, col);
        pset(x0 - x, y0 - y, col);
        pset(x0 - y, y0 - x, col);
        pset(x0 + y, y0 - x, col);
        pset(x0 + x, y0 - y, col);

        y++;
        err += 1 + 2*y;
        if (2*(err - x) + 1 > 0) {
            x--;
            err += 1 - 2*x;
        }
    }
}


// filled circle (Midpoint circle algorithm or simple fill)
void circfill(int x0, int y0, int r, int col) {

    for (int y = -r; y <= r; y++) {
        int dx = (int)sqrt(r*r - y*y);
        for (int x = -dx; x <= dx; x++) {
            pset(x0 + x, y0 + y, col);
        }
    }
}


void rectfill(int x1, int y1, int x2, int y2, int col) {

    for (int y = y1; y <= y2; y++)
        for (int x = x1; x <= x2; x++)
            pset(x, y, col);
}

void rect(int x1, int y1, int x2, int y2, int col) {

    for (int x = x1; x <= x2; x++) {
        pset(x, y1, col);
        pset(x, y2, col);
    }
    for (int y = y1; y <= y2; y++) {
        pset(x1, y, col);
        pset(x2, y, col);
    }
}

void line(int x0, int y0, int x1, int y1, int col) {


    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        pset(x0, y0, col);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void flip(){
    drawFramebuffer();   // Update the display
    M5Cardputer.update();
    delay(33);           // ~30 FPS (1000ms / 30 = ~33.3ms)
}
struct BtnState {
  bool pressedLastFrame = false;
  int holdFrames = 0;
};
const int NUM_BUTTONS = 6;

BtnState btnStates[NUM_BUTTONS];

//bool prevBtnStates[6] = {false, false, false, false, false, false};
// btnp: returns true only on the frame the button was pressed
const int REPEAT_DELAY = 4;  // frames before repeat starts
const int REPEAT_INTERVAL = 4; // repeat 

bool btnp(int i, int p = 0) {
  if (p != 0) return false;

  bool pressedNow = btn(i, p);
  BtnState& state = btnStates[i];

  bool result = false;

  if (pressedNow) {
    if (!state.pressedLastFrame) {
      result = true;
      state.holdFrames = 0;
    } else {
      state.holdFrames++;
      if (state.holdFrames > REPEAT_DELAY && ((state.holdFrames - REPEAT_DELAY) % REPEAT_INTERVAL == 0)) {
        result = true;
      }
    }
  } else {
    state.holdFrames = 0;
  }

  state.pressedLastFrame = pressedNow;
  return result;
}
/*
bool btnp(int i, int p = 0) {
    if (p != 0) return false;

    bool current = btn(i, p);
    bool pressed = false;

    if (i >= 0 && i < 6) {
        // btnp is true if currently pressed AND was not pressed previous frame
        pressed = (current && !prevBtnStates[i]);
        // update previous state for next frame
        prevBtnStates[i] = current;
    }
    return pressed;
}
*/
bool btn(int i, int p = 0){
    // Only support player 0 for now
    if (p != 0) return false;

    // Map button indices to key codes
    switch (i) {
        case 0: return M5Cardputer.Keyboard.isKeyPressed('a'); // Left
        case 1: return M5Cardputer.Keyboard.isKeyPressed('d'); // Right
        case 2: return M5Cardputer.Keyboard.isKeyPressed('e'); // Up
        case 3: return M5Cardputer.Keyboard.isKeyPressed('s'); // Down
        case 4: return M5Cardputer.Keyboard.isKeyPressed('k'); // Button O
        case 5: return M5Cardputer.Keyboard.isKeyPressed('l'); // Button X
        default: return false;
    }
    
}


void cls(int col = -1) {
    if (col == -1) col = currentColor;
    col &= 0x0F;
    uint8_t packed = (col << 4) | col;

    for (int i = 0; i < 0x2000; i++) {
        memory[SCREEN_MEM + i] = packed;
    }
}

