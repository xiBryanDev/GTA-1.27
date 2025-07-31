#include "native.h"

Native_s** g_Natives;

unsigned int FindNativeTableAddress()
{
	unsigned int  l_uiNativeTableAddress = 0;
	unsigned int l_uiStartAddress = 0x390000;
	unsigned int l_uiAddress = 0;
	for (unsigned int i = 0; i < 0x10000; i++)
	{
		if (*(volatile unsigned int*)(l_uiStartAddress + i) == 0x3C6072BD)
			if (*(volatile unsigned int*)(l_uiStartAddress + i + 8) == 0x6063E002)
			{
				l_uiAddress = *(volatile unsigned int*)(l_uiStartAddress + i + 0x10);
				l_uiAddress &= 0xFFFFFF;
				l_uiAddress += (l_uiStartAddress + i + 0x10) - 1;
				//printf("address: 0x%08x\n", l_uiAddress);
				break;
			}
	}
	l_uiNativeTableAddress = (*(volatile unsigned int*)(l_uiAddress + 0x24) << 16) + (*(volatile unsigned int*)(l_uiAddress + 0x2C) & 0xFFFF);
	l_uiNativeTableAddress -= 0x10000;
	return l_uiNativeTableAddress;
}