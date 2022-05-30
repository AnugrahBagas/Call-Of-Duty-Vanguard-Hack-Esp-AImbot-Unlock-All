#include "Addresses.h"
#pragma once

#define PI 3.14159265
#define stand	0x0
#define crouch	0x4
#define lay		0x8

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];

typedef struct
{
	UINT NumVerticies;
	UINT PrimCount;
	UINT StartIndex;
} model_t;

class ScreenRes_t
{
public:
	__int32 iX;
	__int32 iY;
};//[Size=0x0008]

class PlayerActor_t
{
public:
	__int32 iServerTime; //0x0000 
	char _0x0004[24];
	vec3_t vOrigin; //0x001C 
	vec3_t vVelocity; //0x0028 
	char _0x0034[36];
	__int32 iGravity; //0x0058 
	__int32 iSpeed; //0x005C 
	char _0x0060[172];
	float vViewangles[2]; //0x010C 
	char _0x0114[4];
	__int32 iPlayerHeight; //0x0118 
	float flPlayerHeight; //0x011C 
	char _0x0120[64];
	__int32 iHealth; //0x0160 
	char _0x0164[4];
	__int32 iMaxHealth; //0x0168 
	char _0x016C[200];
	__int32 iSecWeaponID3; //0x0234 
	__int32 iOffhandID3; //0x0238 
	char _0x023C[4];
	__int32 iPriWeaponID3; //0x0240 
	char _0x0244[228];
	__int32 iTacticalEquipID; //0x0328 
	__int32 iTacticalEquipAmmo; //0x032C 
	__int32 iCurrentWeapon; //0x0330 
	char _0x0334[4];
	float flZoomFraction; //0x0338 
	float flCrosshairSpreadAdd; //0x033C 
	char _0x0340[20];
	__int32 iSecWeaponID2; //0x0354 
	__int32 iSecAmmoClip; //0x0358 
	__int32 iOffhandID2; //0x035C 
	char _0x0360[12];
	__int32 iPriWeaponID2; //0x036C 
	__int32 iPriAmmoClip; //0x0370 
	char _0x0374[88];
	__int32 iSecWeaponID1; //0x03CC 
	__int32 iSecAmmo; //0x03D0 
	__int32 iSecAkimboAmmo; //0x03D4 
	__int32 iOffhandID1; //0x03D8 
	__int32 iOffhandAmmo; //0x03DC 
	char _0x03E0[16];
	__int32 iPriWeaponID1; //0x03F0 
	__int32 iPriAmmo; //0x03F4 
	__int32 iPriAkimboAmmo; //0x03F8 
	char _0x03FC[4];
	__int32 iActionslot1; //0x0400 
	char _0x0404[332];

};//Size=0x0550

class entity_t
{
public:
	char unknown0[2]; //0x0000
	__int16 InfoValid; //0x0002  
	char unknown4[12]; //0x0004
	vec3_t vOrigin;
	char unknown28[4]; //0x001C
	float Angle; //0x0020  
	char unknown36[60]; //0x0024
	__int32 eFlag; //0x0060  
	__int32 eType; //0x0064  
	char unknown104[8]; //0x0068
	vec3_t vOrigin_old;
	char unknown124[12]; //0x007C
	__int32 eType2; //0x0088  
	char unknown140[8]; //0x008C
	float AngleX; //0x0094  
	float AngleY; //0x0098  
	char unknown156[60]; //0x009C
	__int32 eFlag2; //0x00D8  
	__int32 eType3; //0x00DC  
	char unknown224[8]; //0x00E0
	vec3_t vOrigin_old2;
	char unknown244[24]; //0x00F4
	float Angle2_x; //0x010C  
	float Angle2_y; //0x0110  
	char unknown276[52]; //0x0114
	__int32 Weapon; //0x0148  
	char unknown332[4]; //0x014C
	__int32 ClientNum; //0x0150  
	char unknown340[40]; //0x0154
	__int32 Valid2; //0x017C  
	char unknown384[1]; //0x0180
	__int8 Valid; //0x0181  
	char unknown386[18]; //0x0182
};//Size=0x0194

class TeamInfo_ptr
{
public:
	char _0x0000[4];
	__int32 iTeam; //0x0004 
	char _0x0008[120];
};//Size=0x0080

class client_t
{
public:
	__int16 iCurrentValid; //0x0000 
	char _0x0002[10];
	__int32 iPose; //0x000C 
	char _0x0010[12];
	vec3_t vOrigin; //0x001C 
	char _0x0028[24];
	vec3_t vAngles; //0x0040 
	char _0x004C[48];
	__int16 iCurrentWeapon; //0x007C 
	__int16 iClientNum; //0x007E 
	char _0x0080[84];
	vec3_t vViewOrigin; //0x00D4 
	char _0x00E0[12];
	vec3_t vOldOrigin; //0x00EC 
	char _0x00F8[20];
	PlayerActor_t* PlayerActor; //0x010C 
	char _0x0110[4];
	TeamInfo_ptr* TeamInfo; //0x0114 
	char _0x0118[44];
	__int32 iCommandCount; //0x0144 
	char _0x0148[8];
	__int32 iHealth; //0x0150 
	__int32 iMaxHealth; //0x0154 
	char _0x0158[284];

};//Size=0x0274

class CG_t
{
public:
	__int32 ClientNum;//0x0000
	char _0x0004[64];
	__int32 iVelocity; //0x0044 
	vec3_t vOrigin; //0x0048 
	vec3_t vVelocity; //0x0054 
	char _0x0060[216];
	float vViewAngles[2]; //0x0138 
	char _0x0140[548];
	float flZoomFraction; //0x0364 
	char _0x0368[216];
};//Size=0x0440

class RefDef_t
{
public:
	__int32 iX; //0x0000 
	__int32 iY; //0x0004 
	__int32 iWidth; //0x0008 
	__int32 iHeight; //0x000C 
	float flFovX; //0x0010 
	float flFovY; //0x0014 
	vec3_t vOrigin; //0x0018 
	vec3_t vViewAxis[3]; //0x0024 
	char _0x0048[4];
	vec3_t vOldOrigin; //0x004C 
	char _0x0058[100];
	char sz_pad[19068]; //0x00BC 
	float RefdefViewAngles[2]; //0x4B38 
};//Size=0x4B40

class viewAngles_t
{
public:
	char unknown0[52]; //0x0000
	float AngleX; //0x0034  
	float AngleY; //0x0038  
	char unknown60[28]; //0x003C
	float ViewAngleX; //0x0058  
	float ViewAngleY; //0x005C  
};

class ScreenPlacement_t
{
public:
	float scaleVirtualToReal[2];
	float scaleVirtualToFull[2];
	float scaleRealToVirtual[2];
	float virtualScreenOffsetX;
	float virtualViewableMin[2];
	float virtualViewableMax[2];
	float realViewportSize[2];
	float realViewableMin[2];
	float realViewableMax[2];
	float subScreenLeft;
};

class Weapon_t
{
public:
	char *szModel; //0x0000 
	char _0x0004[4];
	char *szLocalizedName; //0x0008 
	char _0x000C[60];
	float flZoomedFOV; //0x0048 
	char _0x004C[8];
	__int32 iMaxClipRounds; //0x0054 
	__int32 iBulletImpactEffectType; //0x0058 
	__int32 iWeaponFireDelay; //0x005C 
	__int32 iWeaponBulletType; //0x0060 
	char _0x0064[4];
	vec3_t vWeaponWeight; //0x0068 
	char _0x0074[16];
	__int32 Icon; //0x0084 
};

class trace_t //usually decent change between games, this one is for cod4
{
public:
	float fraction;
	float normal[3];
	int surfaceFlags;
	int contents;
	const char *material;
	int hitType;
	unsigned __int16 hitId;
	unsigned __int16 modelIndex;
	unsigned __int16 partName;
	unsigned __int16 partGroup;
	bool allsolid;
	bool startsolid;
	bool walkable;
}; 

class ammo_t
{
public:
	__int32 Primarystk; //0x0000 
	char _0x0004[20];
	__int32 Secondarystk; //0x0018 
	char _0x001C[92];
	__int32 Primaryclip; //0x0078 
	char _0x007C[8];
	__int32 Grenades; //0x0084 
	char _0x0088[8];
	__int32 Flashes; //0x0090 
	char _0x0094[8];
	__int32 SecondaryClip; //0x009C 
	char _0x00A0[8];
	__int32 C4; //0x00A8 
	char _0x00AC[8];
	__int32 Claymore; //0x00B4 
	__int32 akimbo2; //0x00B8
	char _0x00BC[4];
};//Size=0x0440

class w2s_t
{
public:
	char unknown[0xDC];
};

entity_t *GetEntityByIndex(int iIndex)
{
	return (entity_t *)(OFFS_Entity + (iIndex * SIZE_Entity));
}

client_t *GetClientByIndex(int iIndex)
{
	return (client_t *)(OFFS_Client + (iIndex * SIZE_Client));
}

entity_t *GetLocalEntity()
{
	return (entity_t *)(OFFS_Entity);
}

client_t *GetLocalClient()
{
	return (client_t *)(OFFS_Client);
}

CG_t * CG = (CG_t*)OFFS_CG;
ammo_t		* pLocalAmmo	= (ammo_t*)OFFS_Ammo;
RefDef_t	* RefDef	= (RefDef_t*)OFFS_RefDef;
viewAngles_t* ViewAngles= (viewAngles_t*)OFFS_ViewAngles;
ScreenRes_t	* ScreenRes = (ScreenRes_t*)OFFS_ScreenRes;