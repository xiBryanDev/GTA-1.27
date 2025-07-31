#include "detour.h"

uint8_t hook_stub_section[0x10000] __attribute__((section(".text")));
int stubCount_;


DetourPPC::~DetourPPC() {
    // Restaura instruções originais ao destruir
    Memory::Write(targetAddr_, originalCode_, sizeof(originalCode_));
}

void DetourPPC::makeJump(uint32_t out[4], uint32_t dest, uint8_t reg, bool linked) {
    // lis reg, dest@h
    out[0] = 0x3C000000 | (reg << 21) | ((dest >> 16) & 0xFFFF);
    // ori reg, reg, dest@l
    out[1] = 0x60000000 | (reg << 21) | (reg << 16) | (dest & 0xFFFF);
    // mtctr reg
    out[2] = 0x7C0903A6 | (reg << 21);
    // bctr ou bctrl
    out[3] = linked ? 0x4E800421 : 0x4E800420;
}

uint32_t DetourPPC::allocateStub() {
    uint32_t addr = reinterpret_cast<uint32_t>(&hook_stub_section[stubCount_ * 0xA0]);
    stubCount_++;
    return addr;
}

