#include "gfx.h"
#include <M5Cardputer.h>
#include "mem.h"

void pico8_rng_step() {
    // Swap 0x5f44 and 0x5f46 (2 bytes each)
    uint16_t a = read_u16(RNG_MEM);
    uint16_t b = read_u16(RNG_MEM + 2);
    write_u16(RNG_MEM, b);
    write_u16(RNG_MEM + 2, a);

    // Add 32-bit at 0x5f48 to 0x5f44
    uint32_t val_a = read_u32(RNG_MEM);
    uint32_t val_b = read_u32(RNG_MEM + 4);
    val_a += val_b;
    write_u32(RNG_MEM, val_a);

    // Then add new 0x5f44 to 0x5f48
    val_b += val_a;
    write_u32(RNG_MEM + 4, val_b);
}

float pico8_rnd(float x) {
    pico8_rng_step();
    uint32_t raw = read_u32(RNG_MEM);
    float r = (raw & 0x7FFFFFFF) / 2147483648.0f;  // [0,1)
    return r * x;
}

void pico8_srand(uint32_t seed) {
    // You can choose any seed mixer (same as above)
    write_u32(RNG_MEM, seed ^ 0xA5A5A5A5);
    write_u32(RNG_MEM + 4, seed);
}


float rnd() {
    return (float)random(0, 0x10000) / 0x10000;  // ~0.000015 resolution
}

float rnd(float n) {
    return rnd() * n;
}