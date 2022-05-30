#include <Windows.h>
#pragma once
#define WindowX					0x00AA41F4
#define WindowY					0x00AA41F8
#define S_PCTC					0x00457c90
#define S_GETTAGPOS				0x0055B7D0
#define S_REGTAG				0x005048B0
#define S_TRACE					0x004F6220
#define OFFS_CG					0x0096F098
#define OFFS_RefDef				0x0098F948
#define OFFS_ScreenRes			0x00AA41F4
#define OFFS_Entity				0x009AC010
#define OFFS_Client				0x01197AD8
#define OFFS_Ammo				0x013820A0
#define S_GETSCREENMATRIX		0x004649C0
#define S_WORLDTOSCREEN			0x0047F2A0
#define SIZE_Client				0x00000270
#define SIZE_Entity				0x00000194
#define OFFS_WeaponName			0x009A7D6C
#define S_MSGBOX				0x00425540
#define OFFS_ViewAngles			0x00B363B0
//#define RapidFire1              *(int*)0x137F848					  //[integer]
//#define RapidFire2              *(int*)0x982D08						  //[integer]
#define pJumpHeight             *(float*)(*(int*)0x1d35a90 + 0xC)       //[float]
#define pFov                  *(float*)(*(int*)0xa76130 + 0xC)        //[float]
#define pSpeed                 *(int*)(*(int*)0x1357a80 + 0xC)       //[integer]
//#define remoteAccess            *(float*)(*(int*)0x12cfb54 + 0xC)		//[float]
#define pTimeWarp				*(float*)(*(int*)0x1769F38 + 0xC)		//[float]
#define pNoRecoil				0x4a2420								 //[byte]
#define pNoSpread				*(float*)(*(int*)0x1d398a8 + 0xC)		//[float]
#define pPerkCheck				0x42a8e5								 //[byte]
#define pEnableChaos				*(int*)(*(int*)0x1f1570c + 0xC)		  //[integer]
#define pActivateChaos			*(int*)(*(int*)0x1e06e98 + 0xC)		  //[integer]
#define pRapidFire				0x6477F6							  //[integer]
#define pRfPerk				*(float*)(*(int*)0x1D398F4 + 0xC)		//[float]
#define pPerkCheckADS			0x509c29							  //[integer]
#define pInstantADS				*(float*)(*(int*)0x1D398AC + 0xC)		//[float]
bool IsInGame(){return *(int*)0x1197C28 == 0 ? false : true;}