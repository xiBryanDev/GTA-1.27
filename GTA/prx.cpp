#include "stdafx.h"
#include "detour.h"
#include "native.h"
#include "graphics.h"
#include "pad.h"
#include "menu.h"


#pragma diag_suppress 77

SYS_MODULE_INFO("GTA", 0, 1, 1);
SYS_MODULE_START(module_start);
SYS_MODULE_STOP(module_stop);


DetourPPC* IS_PLAYER_ONLINE_t;
bool IS_PLAYER_ONLINE()
{
	g_Menu->Render();
	return true;
}

extern "C"
{
	int module_start()
	{
		printf("GTA module start address: 0x%08X\n", (uint32_t)&__start__Ztext[0]);

		Memory::LiReg(0x10CF2C, 4, 1);//By Divino Glade
		Memory::LiReg(0x1389F60, 3, 1);//BypassPrologue
		Memory::LiReg(0x8797C, 3, 1);//BypassPrologue

		g_Natives = (Native_s**)FindNativeTableAddress();

		g_Menu = new Menu();

		IS_PLAYER_ONLINE_t = new DetourPPC(0x3E3A20, IS_PLAYER_ONLINE, true, false, PPCRegister::R12);


		ps3_gamepad::start();

		return 0;
	}

	int module_stop()
	{
		delete IS_PLAYER_ONLINE_t;
		ps3_gamepad::stop();

		return 0;
	}
}
