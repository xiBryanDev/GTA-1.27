#pragma once
#include "stdafx.h"
#include "memory.h"



extern uint8_t hook_stub_section[0x10000] __attribute__((section(".text")));
extern int stubCount_;

enum class PPCRegister : uint8_t {
    R0 = 0,
    R11 = 11,
    R12 = 12,
};

class DetourPPC {
public:
    template<class T>
    DetourPPC(uint32_t target, T(*hook), bool jumpOnly = false, bool linked = false, PPCRegister reg = PPCRegister::R11)
        : targetAddr_(target), hookAddr_(*reinterpret_cast<uint32_t*>(hook)), reg_(static_cast<uint8_t>(reg)), stubAddr_(0)
    {
        // Salva instruções originais
        Memory::Read(targetAddr_, originalCode_, sizeof(originalCode_));

        if (!jumpOnly) {
            stubAddr_ = allocateStub();
            Memory::Write(stubAddr_, originalCode_, sizeof(originalCode_));

            uint32_t continueAddr = targetAddr_ + sizeof(originalCode_);
            uint32_t stubJump[4];
            makeJump(stubJump, continueAddr, reg_, false);
            Memory::Write(stubAddr_ + sizeof(originalCode_), stubJump, sizeof(stubJump));

            functionOpd_[0] = stubAddr_;
            uint32_t* tocEntry = *reinterpret_cast<uint32_t**>(0x1001C);
            functionOpd_[1] = tocEntry[1];
        }

        uint32_t hookJump[4];
        makeJump(hookJump, hookAddr_, reg_, linked);
        Memory::Write(targetAddr_, hookJump, sizeof(hookJump));
    }

    ~DetourPPC();

    template<typename Ret, typename... Args>
    Ret CallOriginal(Args... args) {
        if (!functionOpd_[0]) return Ret();
        auto fn = reinterpret_cast<Ret(*)(Args...)>(functionOpd_);
        return fn(args...);
    }



private:
    uint32_t targetAddr_;
    uint32_t hookAddr_;
    uint8_t reg_;
    uint32_t stubAddr_;
    uint32_t originalCode_[4]{};
    uint32_t functionOpd_[2];

    void makeJump(uint32_t out[4], uint32_t dest, uint8_t reg, bool linked);
    uint32_t allocateStub();
};
