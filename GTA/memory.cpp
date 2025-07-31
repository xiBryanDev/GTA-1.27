#include "memory.h"
bool useHenSyscalls;
uint32_t Memory::SysHenReadProcessMemory(uint64_t address, void* data, size_t size)
{
    system_call_6(8, 0x7777, 0x31, static_cast<uint64_t>(sys_process_getpid()), address, reinterpret_cast<uint64_t>(data), size);
    return_to_user_prog(uint32_t);
}

uint32_t Memory::SysHenWriteProcessMemory(uint64_t address, void* data, size_t size)
{
    system_call_6(8, 0x7777, 0x32, static_cast<uint64_t>(sys_process_getpid()), address, reinterpret_cast<uint64_t>(data), size);
    return_to_user_prog(uint32_t);
}

uint32_t Memory::SysDbgReadProcessMemory(uint64_t address, void* data, size_t size)
{
    system_call_4(904, static_cast<uint64_t>(sys_process_getpid()), address, size, reinterpret_cast<uint64_t>(data));
    return_to_user_prog(uint32_t);
}

uint32_t Memory::SysDbgWriteProcessMemory(uint64_t address, void* data, size_t size)
{
    system_call_4(905, static_cast<uint64_t>(sys_process_getpid()), address, size, reinterpret_cast<uint64_t>(data));
    return_to_user_prog(uint32_t);
}

uint32_t Memory::Read(uint64_t address, void* data, size_t size)
{
    if (!useHenSyscalls)
    {
        uint32_t read = SysDbgReadProcessMemory(address, data, size);
        if (read == SUCCEEDED)
        {
            return read;
        }
    }
    useHenSyscalls = true;
    return SysHenReadProcessMemory(address, data, size);
}

uint32_t Memory::Write(uint64_t address, void* data, size_t size)
{
    if (!useHenSyscalls)
    {
        uint32_t write = SysDbgWriteProcessMemory(address, data, size);
        if (write == SUCCEEDED)
        {
            return write;
        }
    }
    useHenSyscalls = true;
    return SysHenWriteProcessMemory(address, data, size);
}

bool Memory::Blr(uint64_t address)
{
    return Write(address, (void*)"\x4E\x80\x00\x20", 4) == SUCCEEDED;
}

bool Memory::Nop(uint64_t address, size_t size)
{
    for (size_t i = 0; i < size; i += 4)
    {
        if (Write(address + i, (void*)"\x60\x00\x00\x00", 4) != SUCCEEDED)
            return false;
    }
    return true;
}

bool Memory::LiReg(uint64_t address, uint8_t reg, uint32_t value)
{
    if (reg > 31) return false;
    uint32_t instr = (0x38000000 | ((reg & 0x1F) << 21) | (value & 0xFFFF));
    return Write(address, &instr, sizeof(instr)) == SUCCEEDED;
}
