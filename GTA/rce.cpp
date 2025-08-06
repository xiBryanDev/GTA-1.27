//#include "rce.h"
//#include "native.h"
//	
//unsigned int TunablePointer = 0x1E70374;//idk man it might work
//
//
//int Read_Global(int a_uiGlobalID)
//{
//	int Ptr = *(volatile int*)((TunablePointer - 0x04) + (((a_uiGlobalID / 0x40000) & 0x3F) * 4));
//	if (Ptr != 0)
//		return *(volatile int*)(Ptr + ((a_uiGlobalID % 0x40000) * 4));
//	return 0;
//}
//
////bool assureHostOfFreemode(int player = PLAYER::PLAYER_ID()) {
////	if (I_AM_ONLINE) {
////		if (getHostOfFreemodeScript() != player) {
////			if (freemodeScriptCGameScriptHandlerNetwork->cGameScriptHandlerNetComponentSyncOne != 0) {
////				setScriptHost(freemodeScriptCGameScriptHandlerNetwork, player);
////			}
////		}
////		else {
////			//I am host of script
////			return true;
////		}
////	}
////	return false;
////}
//
//bool doRCEV5NegativeFix = true;
//void executeRCEV5(int player, int global, int value) {
//
//	if (Read_Global(1666041) != 0x7FFFFFFF) {
//		printf("Not ready!");
//		return;
//	}
//	else {
//		assureHostOfFreemode(player);//this is necessary, function now works, just slowly
//		if (getHostOfFreemodeScript() != player) {
//			//printf("Player %i chosen, not ready %s\n", player, PLAYER::GET_PLAYER_NAME(player));
//			print2("Not ready yet! Please wait a bit and retry");
//			//return;
//		}
//		else {
//
//			//ScriptArg rargs[3] = {195,player,0};//-1 is default value I think
//			unsigned int playerBit = (1 << player);
//			//SCRIPT::TRIGGER_SCRIPT_EVENT(1,rargs,3,playerBit);
//
//			int index = global - (1666041 + 1);
//
//			if (index < 0) {
//				//print2("Fail!");
//				//printf("Cannot set a negative value %i\n",index);
//				//0x7FFFFFFE = Global_1666040
//				//1666040 - (1666041+1) = -2 which makes sense
//				int tmpindex = index;
//				index = 0x7FFFFFFF + (index + 1);
//				//1666040 should return 0x7FFFFFFE
//				//printf("Global: %i, Ni: 0x%X, Oi: %i\n",global,index,tmpindex);
//			}
//
//			bool isTopBitSet = ((int)(value & (1 << 31)) >> 31);
//			if (doRCEV5NegativeFix) {
//				value = value | (1 << 31);//make sure it is negative, otherwise you will get freemode script crash
//			}
//			ScriptArg args[5] = { 317,0,index,value/*imm3*/,-1/*player id, set to -1 to do nothing on last*/ };
//
//
//			SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, 5, playerBit);
//
//			if (doRCEV5NegativeFix) {
//				if (!isTopBitSet) {
//					//clear the top bit with the v6 method
//					RCEV6(player, global, 31);//This does appear to work on tests, setting 0x70000000 will result in F0 if this function is not called
//				}
//			}
//
//			//now call script event 195 to reset the value we just set wrong on accident via the previous trigger
//			//func_6131 would fix the value that was set
//			//SCRIPT::TRIGGER_SCRIPT_EVENT(1,rargs,3,playerBit);
//		}
//	}
//	encryptFunctionEnd();
//}
