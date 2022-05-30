#include "Menu.h"
#include "ColorDefinitions.h"
#include "Hacks.h"
#pragma once

void Menu::CreateItem(char * title, int *toggle, HEADER *parentHeader, int hacktype)
{
	itemcount++;
	MENUITEM[itemcount].index = itemcount;
	MENUITEM[itemcount].title = title;
	MENUITEM[itemcount].toggle = toggle;
	MENUITEM[itemcount].hacktype = hacktype;
	MENUITEM[itemcount].parentHeader = parentHeader;
}

void Menu::CreateHeader(char * title, int *toggle, int items)
{

	CreateItem(title, toggle, 0, 99);

	headercount++;
	MENUHEADER[headercount].index = itemcount;
	MENUHEADER[headercount].title = title;
	MENUHEADER[headercount].toggle = toggle;
	MENUHEADER[headercount].items = items;

}

void Menu::BuildMenu(char * menuname, int x, int y, int h, int w, DWORD TITLECOL, DWORD BACKCOLOR, DWORD BORDERCOLOR, LPDIRECT3DDEVICE9 pDevice)
{
	if (GetAsyncKeyState(VK_INSERT) & 1)show = (!show);
	if (show) {

		CreateHeader("Player", &HPlayer, 8);
		if (HPlayer == 1)
		{
			CreateItem("Unl. Health", &iHealth, &MENUHEADER[1], 0);
			CreateItem("Unl. Ammo", &iAmmo, &MENUHEADER[1], 0);
			CreateItem("Rapid fire", &iRapidFire, &MENUHEADER[1], 0);
			CreateItem("Instant ADS", &iInstaADS, &MENUHEADER[1], 0);
			CreateItem("No Recoil", &iRecoil, &MENUHEADER[1], 0);
			CreateItem("No Spread", &iSpread, &MENUHEADER[1], 0);
			CreateItem("Super Jump", &iJump, &MENUHEADER[1], 1);
			CreateItem("Super Speed", &iSpeed, &MENUHEADER[1], 1);
			CreateItem("Add Money", &iMoney, &MENUHEADER[1], 0);
		}

		CreateHeader("System", &HSystem, 4);
		if (HPlayer == 1)
		{
			CreateItem("Clean Floor", &iFloor, &MENUHEADER[2], 0);
			CreateItem("FoV", &iFOV, &MENUHEADER[2], 2);
			CreateItem("Time Warp", &iTime, &MENUHEADER[2], 3);
			CreateItem("One shot kill", &iInstaKill, &MENUHEADER[2], 0);
			//TODO: Shoot Though Walls
			CreateItem("Chaos Mode", &iChaos, &MENUHEADER[2], 0);
		}

		CreateHeader("ESP", &HESP, 4);
		if (HPlayer == 1)
		{
			CreateItem("ESP", &iESP, &MENUHEADER[3], 0);
			CreateItem("Lines", &iELines, &MENUHEADER[3], 0);
			CreateItem("Mark Head", &iEHeads, &MENUHEADER[3], 0);
			CreateItem("Show Health", &iEHealth, &MENUHEADER[3], 0);
			CreateItem("Show Weapon", &iEWeapons, &MENUHEADER[3], 0);
		}

		// TODO: Implement aimbot+triggerbot
		//CreateHeader("Aimbot", &header4, 2);
		//if (header4 == 1)
		//{
		//	CreateItem("Aimbot", &iAimbot, &MENUHEADER[4], 0);
		//	CreateItem("Triggerbot", &iTriggerbot, &MENUHEADER[4], 0);
		//}

		char d[32], e[32], f[32], g[32], h[32];

		//ViewAngles debug
		sprintf(d, "%f", ViewAngles->AngleX);
		sprintf(e, "%f", ViewAngles->AngleY);
		PrintText(d, 50, 300, WHITE, pFont);
		PrintText(e, 50, 315, WHITE, pFont);

		//ViewAxis debug
		sprintf(f, "%f", RefDef->vViewAxis[0]);
		sprintf(g, "%f", RefDef->vViewAxis[1]);
		sprintf(h, "%f", RefDef->vViewAxis[2]);
		PrintText(f, 50, 350, BLUE, pFont);
		PrintText(g, 50, 365, BLUE, pFont);
		PrintText(h, 50, 380, BLUE, pFont);

		PrintMenu(pDevice, pFont, pHeaderFont);

		if (selector < 1)
			selector = 1;

		if (selector > itemcount)
			selector = 1;
	}
	//Draw
	DrawBox(0, y, 185, (itemcount + 1) * 15, BLACK, GOLD, pDevice);
	PrintText(menuname, 55, y, GOLD, pHeaderFont);
	
	//Update
	Navigation();
	Hacks(pDevice, pFont);
	itemcount = 0;
	headercount = 0;
}

void Menu::Navigation()
{
	//KEYPRESS UP
	if (GetAsyncKeyState(VK_UP) & 1) {
		if (selector > 1) {
			if ((MENUITEM[selector].toggle == 0) && (MENUITEM[selector].hacktype = 99))
				selector -= MENUITEM[selector].parentHeader->items;
			else
				selector--;
		}
		Sleep(100);
	}

	//KEYPRESS DOWN
	if (GetAsyncKeyState(VK_DOWN) & 1) {
		if (selector < itemcount) {
			if ((MENUITEM[selector].toggle == 0) && (MENUITEM[selector].hacktype = 99))
				selector += MENUITEM[selector].parentHeader->items;
			else
				selector++;
		}
		Sleep(100);
	}

	//KEYPRESS RIGHT
	if (GetAsyncKeyState(VK_RIGHT)<0) {
		if (*MENUITEM[selector].toggle < MENUITEM[selector].togglemaxval) {
			*MENUITEM[selector].toggle += 1;
			Sleep(100);
		}
	}

	//KEYPRESS LEFT
	if (GetAsyncKeyState(VK_LEFT)<0) {
		if (*MENUITEM[selector].toggle > 0) {
			*MENUITEM[selector].toggle -= 1;
			Sleep(100);
		}
	}
}

void Menu::PrintToggles(LPD3DXFONT pFont, LPD3DXFONT pHeaderFont, int i)
{
	//ITEMTYPE 0; DEFAULT
	if (MENUITEM[i].hacktype == 0) {
		MENUITEM[i].togglemaxval = 1;
		switch (*MENUITEM[i].toggle)
		{
		case 0: PrintText("Off", 150, MENUITEM[i].index * 15, WHITE, pFont); break;
		case 1: PrintText("On", 150, MENUITEM[i].index * 15, GOLDEN, pFont); break;
		}
	}

	//ITEMTYPE 1; JUMP & SPEED
	else if (MENUITEM[i].hacktype == 1) {
		MENUITEM[i].togglemaxval = 5;
		switch (*MENUITEM[i].toggle)
		{
		case 0: PrintText("Off", 150, MENUITEM[i].index * 15, WHITE, pFont); break;
		case 1: PrintText("x2", 150, MENUITEM[i].index * 15, GOLDEN, pFont); break;
		case 2: PrintText("x5", 150, MENUITEM[i].index * 15, GOLDEN, pFont); break;
		case 3: PrintText("x10", 150, MENUITEM[i].index * 15, GOLDEN, pFont); break;
		case 4: PrintText("x20", 150, MENUITEM[i].index * 15, GOLDEN, pFont); break;
		case 5: PrintText("MAX", 150, MENUITEM[i].index * 15, GOLDEN, pFont); break;
		}
	}

	//ITEMTYPE 2; FIELD OF VIEW
	else if (MENUITEM[i].hacktype == 2) {
		MENUITEM[i].togglemaxval = 1;
		switch (*MENUITEM[i].toggle)
		{
		case 0: PrintText("65", 150, MENUITEM[i].index * 15, WHITE, pFont); break;
		case 1: PrintText("80", 150, MENUITEM[i].index * 15, GOLDEN, pFont); break;
		}
	}

	//ITEMTYPE 3; TIMESCALE
	else if (MENUITEM[i].hacktype == 3) {
		MENUITEM[i].togglemaxval = 5;
		switch (*MENUITEM[i].toggle)
		{
		case 0: PrintText("0.2", 150, MENUITEM[i].index * 15, GOLDEN, pFont); break;
		case 1: PrintText("0.5", 150, MENUITEM[i].index * 15, GOLDEN, pFont); break;
		case 2: PrintText("1.0", 150, MENUITEM[i].index * 15, WHITE, pFont); break;
		case 3: PrintText("1.5", 150, MENUITEM[i].index * 15, GOLDEN, pFont); break;
		case 4: PrintText("2.0", 150, MENUITEM[i].index * 15, GOLDEN, pFont); break;
		case 5: PrintText("3.0", 150, MENUITEM[i].index * 15, GOLDEN, pFont); break;
		}
	}

	//ITEMTYPE 4; CROSSHAIR
	else if (MENUITEM[i].hacktype == 4) {
		MENUITEM[i].togglemaxval = 4;
		switch (*MENUITEM[i].toggle)
		{
		case 0: PrintText("Off", 150, MENUITEM[i].index * 15, WHITE, pFont);	break;
		case 1: PrintText("1", 150, MENUITEM[i].index * 15, GOLDEN, pFont);	break;
		case 2: PrintText("2", 150, MENUITEM[i].index * 15, GOLDEN, pFont);	break;
		case 3: PrintText("3", 150, MENUITEM[i].index * 15, GOLDEN, pFont);	break;
		case 4: PrintText("4", 150, MENUITEM[i].index * 15, GOLDEN, pFont);	break;
		}
	}

	//ITEMTYPE 5; COLORS
	else if (MENUITEM[i].hacktype == 5) {
		MENUITEM[i].togglemaxval = 7;
		switch (*MENUITEM[i].toggle)
		{
		case 0: PrintText("Off", 150, MENUITEM[i].index * 15, WHITE, pFont);	break;
		case 1: PrintText("WHITE", 150, MENUITEM[i].index * 15, WHITE, pFont);	break;
		case 2: PrintText("RED", 150, MENUITEM[i].index * 15, RED, pFont);	break;
		case 3: PrintText("GREEN", 150, MENUITEM[i].index * 15, GREEN, pFont);	break;
		case 4: PrintText("BLUE", 150, MENUITEM[i].index * 15, BLUE, pFont);	break;
		case 5: PrintText("GOLDEN", 150, MENUITEM[i].index * 15, GOLDEN, pFont);	break;
		case 6: PrintText("ORANGE", 150, MENUITEM[i].index * 15, ORANGE, pFont);	break;
		case 7: PrintText("MAGENTA", 150, MENUITEM[i].index * 15, MAGENTA, pFont); break;
		}
	}


	//HACKTYPE 99; HEADER
	else if (MENUITEM[i].hacktype == 99) {
		MENUITEM[i].togglemaxval = 1;
		switch (*MENUITEM[i].toggle)
		{
		case 0: PrintText("[+]", 170, MENUITEM[i].index * 15, WHITE, pHeaderFont);	break;
		case 1: PrintText("[-]", 170, MENUITEM[i].index * 15, GOLDEN, pHeaderFont);	break;
		}
	}
}

void Menu::PrintMenu(LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont, LPD3DXFONT pHeaderFont)
{
	//HIGHLIGHT SELECTED ITEM
	if (selector == MENUITEM[selector].index)
		DrawBorder(1, MENUITEM[selector].index * 15 + 1, 183, 13, 1, GOLDEN, pDevice);

	//PRINT ITEMS ON SCREEN
	for (int i = MENUHEADER[1].index; i <= itemcount; i++)
	{
		// HEADERS
		if ((MENUITEM[i].parentHeader == 0) && (MENUITEM[i].hacktype == 99))
			PrintText(MENUITEM[i].title, 0, MENUITEM[i].index * 15, GOLDEN, pHeaderFont);

		//ITEMS
		else {
			//HIGHLIGHT ACTIVE HACK
			if (*MENUITEM[i].toggle != 0) MENUITEM[i].HCOLOR = GOLDEN; else MENUITEM[i].HCOLOR = WHITE;

			//DRAW FUNCTION LINE
			PrintText(MENUITEM[i].title, 15, MENUITEM[i].index * 15, MENUITEM[i].HCOLOR, pFont);

		}
		//PRINT TOGGLE VALUES
		PrintToggles(pFont, pHeaderFont, i);
	}

	for (int i = MENUHEADER[1].index; i <= (MENUHEADER[1].items + 1); i++)
		MENUITEM[i].HCOLOR = WHITE;
}

void Menu::Hacks(LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont)
{
	if (IsInGame()) {
		//Header 1 - Player
		UnlHealth(iHealth);
		UnlAmmo(iAmmo);
		RapidFire(iRapidFire);
		Superjump(iJump);
		Speed(iSpeed);
		AddMoney(iMoney);
		noRecoil(iRecoil);
		noSpread(iSpread);
		InstantADS(iInstaADS);

		//Header 2 - System
		CleanFloor(iFloor);
		FOV(iFOV);
		TimeScale(iTime);
		InstaKill(iInstaKill);
		//Shoot Though Walls
		ChaosMode(iChaos);

		//Header 3 - D3D
		ESP(iESP, iELines, iEHeads, iEHealth, iEWeapons, pDevice, pFont, pLine);
	}
}
