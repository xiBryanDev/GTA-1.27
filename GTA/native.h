#pragma once
#include "stdafx.h"
unsigned int FindNativeTableAddress();
typedef void* Void;
typedef int Any;
typedef unsigned int uint;
typedef int Hash;
typedef int BOOL;
typedef int Entity;
typedef int Player;
typedef int FireId;
typedef int Ped;
typedef int Vehicle;
typedef int Cam;
typedef int CarGenerator;
typedef int Group;
typedef int Train;
typedef int Pickup;
typedef int Object;
typedef int Weapon;
typedef int Interior;
typedef int Blip;
typedef int Texture;
typedef int TextureDict;
typedef int CoverPoint;
typedef int Camera;
typedef int TaskSequence;
typedef int ColourIndex;
typedef int Sphere;
typedef int ScrHandle;

union ScriptArg { unsigned int ui; float f; int i; char* str; };

struct Native_s
{
	Native_s* LastNativesTable;
	unsigned int NativeFunctions[7];
	unsigned int NativeCount;
	unsigned int NativeHashes[7];
};

struct NativeArg_s
{
	int* ReturnValue;
	unsigned int ArgCount;
	int* ArgValues;
	unsigned int ReadCount;
};

extern Native_s** g_Natives;
static NativeArg_s* NativeArg = (NativeArg_s*)0x10060000; // Open space addr

static void ResetArgs()
{
	NativeArg->ArgCount = 0;
	NativeArg->ReadCount = 0;
	NativeArg->ArgValues = (int*)0x10050000;
	NativeArg->ReturnValue = NativeArg->ArgValues;
}

template<typename T>
static void PushArg(T Value)
{
	*(T*)&NativeArg->ArgValues[NativeArg->ArgCount] = Value;
	NativeArg->ArgCount++;
}

static void PushArgsRecursively() {
}

template<typename T, typename... Args>
static void PushArgsRecursively(T first, Args... args) {
	PushArg(first);
	PushArgsRecursively(args...);
}

template<typename R>
static R GetReturn()
{
	R* Result = (R*)&NativeArg->ReturnValue[0];
	return *Result;
}

static void CallHash(unsigned int hash, NativeArg_s* pArg)
{
	Native_s* Natives = g_Natives[hash & 0xFF];
	while (Natives != 0)
	{
		for (unsigned int i = 0; i < Natives->NativeCount; i++)
		{
			if (Natives->NativeHashes[i] == hash)
			{
				((void(*)(NativeArg_s*))Natives->NativeFunctions[i])(pArg);
				return;
			}
		}
		Natives = Natives->LastNativesTable;
	}
}

template<typename T, typename... Args>
static T invoke(unsigned int NativeHash, Args... args)
{
	ResetArgs();
	PushArgsRecursively(args...);
	CallHash(NativeHash, NativeArg);
	return GetReturn<T>();
}

namespace SCRIPT
{
	static const char* GET_THIS_SCRIPT_NAME() { return invoke<const char*>(0xA40FD5D9); } // 0xA40FD5D9
	static void TRIGGER_SCRIPT_EVENT(int always1, ScriptArg* argsStruct, int argsStructCount, int playerbitset) { invoke<Void>(0x54763B35, always1, argsStruct, argsStructCount, playerbitset); } // 0x54763B35

}

namespace UI
{
	static void _0x51E7A037(char* p0) { invoke<Void>(0x51E7A037, p0); } // 0x51E7A037
	static float _END_TEXT_COMMAND_GET_WIDTH(Any p0) { return invoke<float>(0xD12A643A, p0); } // 0xD12A643A

	static void SET_TEXT_FONT(Any p0) { invoke<Void>(0x80BC530D, p0); } // 0x80BC530D
	static void _ADD_TEXT_COMPONENT_STRING(const char* text) { invoke<Void>(0x27A244D8, text); } // 0x27A244D8
	static void SET_TEXT_SCALE(float p0, float p1) { invoke<Void>(0xB6E15B23, p0, p1); } // 0xB6E15B23
	static void SET_TEXT_COLOUR(int R, int G, int B, int A) { invoke<Void>(0xE54DD2C8, R, G, B, A); } // 0xE54DD2C8
	static void SET_TEXT_WRAP(float p0, float p1) { invoke<Void>(0x6F60AB54, p0, p1); } // 0x6F60AB54
	static void SET_TEXT_CENTRE(Any p0) { invoke<Void>(0xE26D39A1, p0); } // 0xE26D39A1
	static void SET_TEXT_RIGHT_JUSTIFY(bool right) { invoke<Void>(0x45B60520, right); } // 0x45B60520
	static void SET_TEXT_DROPSHADOW(Any p0, Any p1, Any p2, Any p3, Any p4) { invoke<Void>(0xE6587517, p0, p1, p2, p3, p4); } // 0xE6587517
	static void SET_TEXT_OUTLINE() { invoke<Void>(0xC753412F); } // 0xC753412F
	static void _SET_TEXT_ENTRY(const char* text) { invoke<Void>(0x3E35563E, text); } // 0x3E35563E
	static void _DRAW_TEXT(float x, float y) { invoke<Void>(0x6F8350CE, x, y); } // 0x6F8350CE
	static void _0x574EE85C(char* p0) { invoke<Void>(0x574EE85C, p0); } // 0x574EE85C
	static int _SET_NOTIFICATION_MESSAGE(char* picName1, char* picName2, BOOL flash, int iconType, char* sender, char* subject) { return invoke<Any>(0xE7E3C98B, picName1, picName2, flash, iconType, sender, subject); } // 0xE7E3C98B
	static int _DRAW_NOTIFICATION(BOOL blink, BOOL p1) { return invoke<int>(0x08F7AF78, blink, p1); } // 0x08F7AF78
	static void _0xF42C43C7(char* p0) { invoke<Void>(0xF42C43C7, p0); } // 0xF42C43C7
	static void _0x38F82261(int Time, int unk) { invoke<Void>(0x38F82261, Time, unk); } // 0x38F82261
	static void HIDE_HUD_COMPONENT_THIS_FRAME(Any p0) { invoke<Void>(0xDB2D0762, p0); } // 0xDB2D0762
	static void HIDE_HELP_TEXT_THIS_FRAME() { invoke<Void>(0xF3807BED); } // 0xF3807BED
	static void SET_TEXT_EDGE(int p0, int p1, int p2, int p3, int p4) { invoke<Void>(0x3F1A5DAB, p0, p1, p2, p3, p4); } // 0x3F1A5DAB
	static BOOL IS_PAUSE_MENU_ACTIVE() { return invoke<BOOL>(0xD3600591); } // 0xD3600591
}


namespace GRAPHICS
{

	static void REQUEST_STREAMED_TEXTURE_DICT(const char* Type, BOOL Toggle) { invoke<Void>(0x4C9B035F, Type, Toggle); } // 0x4C9B035F
	static BOOL HAS_STREAMED_TEXTURE_DICT_LOADED(const char* Type) { return invoke<BOOL>(0x3F436EEF, Type); } // 0x3F436EEF
	static void DRAW_SPRITE(const char* Type, const char* IconType, float X, float Y, float Z, float width, float height, int RED, int GREEN, int BLUE, int Alpha) { invoke<Void>(0x1FEC16B0/*drawAddresses.texture*/, Type, IconType, X, Y, Z, width, height, RED, GREEN, BLUE, Alpha); } // 0x1FEC16B0

}