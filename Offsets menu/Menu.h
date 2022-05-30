#include "Functions.h"
#pragma once

typedef struct {
	int index;
	int items;
	char * title;
	int *toggle;
	DWORD HCOLOR;
} HEADER;

typedef struct {
	int index;
	char * title;
	int *toggle;
	int togglemaxval;
	int hacktype;
	HEADER *parentHeader;
	DWORD HCOLOR;
} ITEM;

class Menu {
public:
	LPD3DXFONT pFont;
	LPD3DXFONT pHeaderFont;
	void BuildMenu(char * menuname, int x, int y, int h, int w, DWORD TITLECOL, DWORD BACKCOLOR, DWORD BORDERCOLOR, LPDIRECT3DDEVICE9 pDevice);

private:
	//D3D
	LPDIRECT3DDEVICE9 pDevice;
	ID3DXLine *pLine;

	//Menu settings
	int itemcount;
	int headercount;
	int x, y, w, h;
	DWORD COLOR;

	//Menu content
	ITEM MENUITEM[99];
	HEADER MENUHEADER[99];
	char hackrval[256];

	void CreateItem(char * title, int *toggle, HEADER *parentHeader, int hacktype);
	void CreateHeader(char * title, int *toggle, int items);
	void Navigation();
	void PrintMenu(LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont, LPD3DXFONT pHeaderFont);
	void PrintToggles(LPD3DXFONT pFont, LPD3DXFONT pHeaderFont, int i);
	void Hacks(LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont);

	//Navigation
	int selector = 1;
	int show = 1;

	//Hack toggles
	int iHealth = 0, iAmmo = 0, iRapidFire = 0,
		iSuperJump = 0, iInstaADS = 0, iRecoil = 0,
		iSpread = 0, iJump = 0, iSpeed = 0,
		iMoney = 0, iFloor = 2, iFOV = 0,
		iTime = 0, iInstaKill = 0, iChaos = 0,
		iESP = 0, iELines = 0, iEHeads = 0,
		iEHealth = 0, iEWeapons = 0, iAimbot = 0,
		HPlayer = 1, HSystem = 0, HESP = 0;

};