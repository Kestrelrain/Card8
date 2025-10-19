#include "mem.h"
uint16_t read_u16(uint16_t addr) {
    return memory[addr] | (memory[addr + 1] << 8);
}

void write_u16(uint16_t addr, uint16_t val) {
    memory[addr] = val & 0xFF;
    memory[addr + 1] = (val >> 8) & 0xFF;
}

uint32_t read_u32(uint16_t addr) {
    return (uint32_t)memory[addr]
         | ((uint32_t)memory[addr + 1] << 8)
         | ((uint32_t)memory[addr + 2] << 16)
         | ((uint32_t)memory[addr + 3] << 24);
}

void write_u32(uint16_t addr, uint32_t val) {
    memory[addr + 0] = val & 0xFF;
    memory[addr + 1] = (val >> 8) & 0xFF;
    memory[addr + 2] = (val >> 16) & 0xFF;
    memory[addr + 3] = (val >> 24) & 0xFF;
}
