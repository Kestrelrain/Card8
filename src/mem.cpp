#include "mem.h"
uint16_t read_u16(uint16_t addr) {
    return pico8_memory[addr] | (pico8_memory[addr + 1] << 8);
}

void write_u16(uint16_t addr, uint16_t val) {
    pico8_memory[addr] = val & 0xFF;
    pico8_memory[addr + 1] = (val >> 8) & 0xFF;
}

uint32_t read_u32(uint16_t addr) {
    return (uint32_t)pico8_memory[addr]
         | ((uint32_t)pico8_memory[addr + 1] << 8)
         | ((uint32_t)pico8_memory[addr + 2] << 16)
         | ((uint32_t)pico8_memory[addr + 3] << 24);
}

void write_u32(uint16_t addr, uint32_t val) {
    pico8_memory[addr + 0] = val & 0xFF;
    pico8_memory[addr + 1] = (val >> 8) & 0xFF;
    pico8_memory[addr + 2] = (val >> 16) & 0xFF;
    pico8_memory[addr + 3] = (val >> 24) & 0xFF;
}
