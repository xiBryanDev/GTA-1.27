#pragma once
#include "stdafx.h"
extern bool useHenSyscalls;
class Memory
{
public:
    static uint32_t Read(uint64_t address, void* data, size_t size);
    static uint32_t Write(uint64_t address, void* data, size_t size);

    static bool Blr(uint64_t address);
    static bool Nop(uint64_t address, size_t size = 4);
    static bool LiReg(uint64_t address, uint8_t reg, uint32_t value);

    // Função template para facilitar escrita de valores de qualquer tipo
    template<typename T>
    static bool Set(uint64_t address, T value)
    {
        return Write(address, &value, sizeof(T)) == SUCCEEDED;
    }

    // Função template para facilitar leitura de valores de qualquer tipo
    template<typename T>
    static bool Get(uint64_t address, T& outValue)
    {
        return Read(address, &outValue, sizeof(T)) == SUCCEEDED;
    }

private:
    static uint32_t SysHenReadProcessMemory(uint64_t address, void* data, size_t size);
    static uint32_t SysHenWriteProcessMemory(uint64_t address, void* data, size_t size);
    static uint32_t SysDbgReadProcessMemory(uint64_t address, void* data, size_t size);
    static uint32_t SysDbgWriteProcessMemory(uint64_t address, void* data, size_t size);
};