#include <Arduino.h>
#define WIDTH 128
#define HEIGHT 128

extern "C" {
  #include "lua.h"
  #include "lauxlib.h"
  #include "lualib.h"
}

#define SD_SPI_SCK_PIN  40
#define SD_SPI_MISO_PIN 39
#define SD_SPI_MOSI_PIN 14
#define SD_SPI_CS_PIN   12

#include <M5Cardputer.h>
#include <SD.h>
#include "pico8_mem.h"
#include "lua_func.h"
#include "gfx.h"

String luaCode = "";

//extern uint8_t pico8_memory[PICO8_MEM_SIZE];
uint8_t pico8_memory[PICO8_MEM_SIZE];


void loadGfxSection(File& file) {
  const int gfxWidth = 128;
  const int gfxHeight = 64;
  int y = 0;

  while (file.available() && y < gfxHeight) {
    String line = file.readStringUntil('\n');

    // Skip empty or too-short lines
    if (line.length() < gfxWidth) continue;

    for (int x = 0; x < gfxWidth; x++) {
      char hexChar = line.charAt(x);

      // Convert hex character to 0–15
      uint8_t color = (hexChar >= '0' && hexChar <= '9') ? (hexChar - '0')
                    : (hexChar >= 'a' && hexChar <= 'f') ? (hexChar - 'a' + 10)
                    : 0;

      pset(x, y, color);
    }

    y++;
  }

  Serial.println("GFX section loaded");
}




void listFiles() {
  File root = SD.open("/");
  if (!root) {
    Serial.println("Failed to open root directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  Serial.println("Files on SD card:");
  File file = root.openNextFile();
  while (file) {
    Serial.print("  ");
    Serial.println(file.name());
    file = root.openNextFile();
  }
}
void run_frame(lua_State* L) {
  // Call _update()
  lua_getglobal(L, "_update");
  if (lua_isfunction(L, -1)) {
    if (lua_pcall(L, 0, 0, 0) != 0) {
      Serial.println(lua_tostring(L, -1));
      lua_pop(L, 1);
    }
  } else {
    lua_pop(L, 1); // remove non-function
  }

  // Call _draw()
  lua_getglobal(L, "_draw");
  if (lua_isfunction(L, -1)) {
    if (lua_pcall(L, 0, 0, 0) != 0) {
      Serial.println(lua_tostring(L, -1));
      lua_pop(L, 1);
    }
  } else {
    lua_pop(L, 1); // remove non-function
  }

  // Flip framebuffer to screen
  drawFramebuffer();

  // Delay for ~60 FPS
  delay(16);
}
void readP8File(const char* filename) {
  File p8file = SD.open(filename);
  if (!p8file) {
    M5.Lcd.printf("Failed to open %s\n", filename);
    return;
  }

  bool inGfxSection = false;
  bool inLuaSection = false;
  int gfxY = 0;

  while (p8file.available()) {
    String line = p8file.readStringUntil('\n');
    line.trim(); // remove newline

    if (line.startsWith("__gfx__")) {
      inGfxSection = true;
      inLuaSection = false;
      gfxY = 0;
      continue;
    }
    if (line.startsWith("__lua__")) {
      M5.Lcd.println("lua");
      inLuaSection = true;
      inGfxSection = false;
      continue;
    }

    if (line.startsWith("__")) {
      inGfxSection = false;
      inLuaSection = false;
    }

    if (inGfxSection && gfxY < 128) {
      // Each line is 128 pixels = 64 bytes (2 pixels per byte)
      size_t len = line.length();
      for (int x = 0; x < (int)std::min(len, (size_t)128); x++) {
        char c = line.charAt(x);
        uint8_t color = (c >= '0' && c <= '9') ? (c - '0')
                      : (c >= 'a' && c <= 'f') ? (c - 'a' + 10)
                      : 0;

        int index = gfxY * 128 + x;
        int byteIndex = index / 2;
        bool highNibble = (index % 2 == 0);

        if (highNibble) {
          pico8_memory[GFX_MEM + byteIndex] = (pico8_memory[GFX_MEM + byteIndex] & 0x0F) | (color << 4);
        } else {
          pico8_memory[GFX_MEM + byteIndex] = (pico8_memory[GFX_MEM + byteIndex] & 0xF0) | (color & 0x0F);
        }
      }

      gfxY++;
    } else if (inLuaSection) {
      luaCode += line + "\n";
    }
  }

  p8file.close();

  Serial.println("Lua code loaded:");
  //M5.Lcd.println(luaCode);
  Serial.println(luaCode);
}

lua_State* L = luaL_newstate();
void setup() {
  M5.begin();
  delay(100);
  Serial.begin(115200);
  SPI.begin(SD_SPI_SCK_PIN, SD_SPI_MISO_PIN, SD_SPI_MOSI_PIN, SD_SPI_CS_PIN);
  if (!SD.begin(SD_SPI_CS_PIN)) {
    M5.Lcd.println("2SD init failed!");
    while(1) delay(1000);
  }
  //if (!SD.begin()) {
  //  M5.Lcd.println("SD init failed!");
  //  return;
  //}
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(TFT_WHITE);

  luaL_openlibs(L); // Load basic libs (math, string, etc.)
  register_lua_functions(L);
  pico8_memory[DRAW_COLOR] = 6;

  // Read and print .p8 file content from SD card
  readP8File("/game.p8");
  //listFiles();
  // Example framebuffer fill: blocky color gradient
  //for (int y = 0; y < HEIGHT; y++) {
  //  for (int x = 0; x < WIDTH; x++) {
  //    uint8_t color = (x / 8 + y / 8) % 16;
  //    setPixel(x, y, color);
  //  }
  //}
  //pset(10, 10, 9);
  //if (luaL_dostring(L, luaCode.c_str()) != 0) {
  //  Serial.println(lua_tostring(L, -1));
  //  lua_pop(L, 1);
  //}
  //File gameFile = SD.open("/game.p8");
  //loadGfxSection(gameFile);
  //gameFile.close();
  //drawFramebuffer();
}

void loop() {
  // Call _update() in Lua
  run_frame(L);

  // Now push your framebuffer to the screen
  //M5.Lcd.println("frame");
  drawFramebuffer();

  //delay(16);  // ~60 FPS
}

