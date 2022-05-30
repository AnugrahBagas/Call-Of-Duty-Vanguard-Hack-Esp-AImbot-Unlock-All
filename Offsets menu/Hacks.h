#pragma once
char rapidfireModValOn[2] = { 0xEB, 0x2C };
char rapidfireModValOff[2] = { 0x74, 0x14 };
trace_t t;

void noRecoil(int _switch)
{
	DWORD dwOld;
	VirtualProtect((PVOID)pNoRecoil, 0x1, PAGE_EXECUTE_READWRITE, &dwOld);
	_switch == 1 ? *(BYTE*)pNoRecoil = 0xC3 : *(BYTE*)pNoRecoil = 0x83;
	VirtualProtect((PVOID)pNoRecoil, 0x1, dwOld, &dwOld);
}

void noSpread(int _switch)
{
	DWORD dwOld;
	VirtualProtect((PVOID)pPerkCheck, 1, PAGE_EXECUTE_READWRITE, &dwOld);
	if(_switch == 1){
		*(BYTE*)pPerkCheck = 0x00;
		pNoSpread = 0.01f;}
	else
		*(BYTE*)pPerkCheck = 0x74;
	VirtualProtect((PVOID)pPerkCheck, 1, dwOld, &dwOld);
}

void CleanFloor(int _switch)
{
	_switch == 1 ? PushCommandToConsole("ai_corpseCount 0") : PushCommandToConsole("ai_corpseCount 30");
}

void UnlHealth(int _switch)
{
	if(_switch == 1){
		client_t * pLocalClient = GetLocalClient();
		if(pLocalClient->iHealth < 1000){
			pLocalClient->iHealth = 1000;
			pLocalClient->iMaxHealth = 1000;
		}
	}
}

void UnlAmmo(int _switch)
{
	if(_switch == 1){
		pLocalAmmo->Primaryclip = 100;
		pLocalAmmo->SecondaryClip  = 100;
		pLocalAmmo->C4 = 100;
		pLocalAmmo->Claymore = 100;
		pLocalAmmo->Grenades = 100;
		pLocalAmmo->Flashes = 100;
	}
}

void Superjump(int _switch)
{
	switch (_switch) {
	case 0: pJumpHeight = 39.0f;	break;
	case 1: pJumpHeight = 800.0f;	break;
	case 2: pJumpHeight = 2000.0f;	break;
	case 3: pJumpHeight = 4000.0f;	break;
	case 4: pJumpHeight = 8000.0f;	break;
	case 5: pJumpHeight = 10000.0f;	break;
	}
}

void Speed(int _switch)
{
	switch (_switch) {
	case 0: pSpeed = 190;  break;
	case 1: pSpeed = 400;  break;
	case 2: pSpeed = 800;  break;
	case 3: pSpeed = 2000; break;
	case 4: pSpeed = 4000; break;
	case 5: pSpeed = 5000; break;
	}
}

void AddMoney(int _switch)
{
	if(_switch == 1){
		int val, address;
		address = FindPattern(0x1A00000, 0x1BFFFF, (PBYTE)"\x00\x00\x00\x00\x00\x00\x06\xD7\x3E\x00", "??????xxxx");
		val = *(int*)address;
		*(int*)address = val + 10000;
}}

void FOV(int _switch)
{
	switch (_switch) {
	case 1: pFov = 80.0f;
	case 2: pFov = 100.0f;
	default: pFov = 65.0f;
	}
}

void TimeScale(int _switch)
{
	switch (_switch) {
	case 0: pTimeWarp = 0.2f;  break;
	case 1: pTimeWarp = 0.5f;  break;
	case 3: pTimeWarp = 1.5f;  break;
	case 4: pTimeWarp = 2.0f;  break;
	case 5: pTimeWarp = 3.0f;  break;
	default: pTimeWarp = 1.0f; break;
	}
}

void InstaKill(int _switch)
{
	for(int i = 0; i < 1024; i++){
		client_t * pLocalClient = GetLocalClient();
		client_t * pClient = GetClientByIndex(i);
		entity_t * pEntity = GetEntityByIndex(i);
		if((_switch == 1) && 
			(pEntity->eType == 1) &&
			(pEntity->Valid == 13) &&
			(pLocalClient->TeamInfo != pClient->TeamInfo) && 
			(pClient->iMaxHealth != 20761) && 
			(pClient->iMaxHealth != 9999) && 
			(pClient->iMaxHealth != 100))
			pClient->iHealth = 1;
	}
}

void RapidFire(int _switch)
{
	DWORD dwOld;
	VirtualProtect((PVOID)pRapidFire, 2, PAGE_EXECUTE_READWRITE, &dwOld);
	if(_switch == 1) {
		memcpy((void*)pRapidFire, &rapidfireModValOn, 2);
		pRfPerk = 0.01f;
	}
	else
		memcpy((void*)pRapidFire, &rapidfireModValOff, 2);
	VirtualProtect((PVOID)pRapidFire, 2, dwOld, &dwOld);
}

void ChaosMode(int _switch)
{
	if(_switch == 1){
		pEnableChaos = 1;
		pActivateChaos = 1;
	}
	else if(_switch == 0)
		pActivateChaos = 0;
	
	PushCommandToConsole("map_restart");
}

void InstantADS(int _switch)
{
	DWORD dwOld;
	VirtualProtect((PVOID)pPerkCheckADS, 0x1, PAGE_EXECUTE_READWRITE, &dwOld);
	if(_switch == 1)
	{
		pInstantADS = 5.f;
		*(BYTE*)pPerkCheckADS = 0x0;
	}
	else
	{
		pInstantADS = 1.5f;
		*(BYTE*)pPerkCheckADS = 0x10;
	}
	VirtualProtect((PVOID)pPerkCheckADS, 0x1, dwOld, &dwOld);
}

void ESP(int _switchESP, int MarkHead, int Weapons, int HP, int Lines, LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont, LPD3DXLINE pLine)
{if(IsInGame()){
	vec3_t vTarget, vOut;
	float fScreen[2], fScreen2[2], fScreen3[2], flOut[2];
	w2s_t * Matrix = GetMatrix();
	float x, y;
	DWORD width(2), COLOR;
	char a[32], b[32], c[32], d[32], e[32], f[32], g[32], h[32], i[32], j[32], k[32], l[32], m[32];

	for(int i = 0; i < 2048; i++)
	{
		client_t * pClient = GetClientByIndex(i);
		entity_t * pEntity = GetEntityByIndex(i);
		client_t * pLocalClient = GetLocalClient();
		entity_t * pLocalEntity = GetLocalEntity();
		
		if(_switchESP)
		{
			if((pEntity->eType == 1) && (pClient->iHealth > 0) && (pEntity->Valid == 13) && (pClient->vOrigin != pLocalClient->vOrigin))
			{
				WORD wHead = RegisterTag("j_head");
				GetTagPos(pEntity, wHead, vTarget);

				if((w2s(0, Matrix, vTarget, fScreen)) && (w2s(0, Matrix, pEntity->vOrigin, fScreen2))){
						if((fScreen2[0] < 1.f) || (fScreen2[1] < 1.f)) continue;

						if((pClient->iMaxHealth != 100) && (pClient->iMaxHealth != 20761))//if not dog / pavelow
						{ 
							//Draw settings
							if(pClient->TeamInfo->iTeam != pLocalClient->TeamInfo->iTeam){
								if(Visible(vTarget)) COLOR = RED; //If visible show brighter box
								else COLOR = MAROON;}
							else 
								COLOR = GREEN ;

							y = fScreen2[1] - fScreen[1];
						}

						if(y < 10.f) y = 10.f;
						x = y / 2.5f;

						//Boxes
						DrawBorder(fScreen[0]-(x/2), fScreen[1]-10.f,x,y+12.f, width, COLOR, pDevice);

						if(MarkHead){ //Cross head
							FillRGB(fScreen[0]-(x/2), fScreen[1], x, 1, COLOR, pDevice); //X line
							FillRGB(fScreen[0], fScreen[1], 1, y, COLOR, pDevice); //Y line
						}

						DWORD x1 = floor((ScreenRes->iX/2)/1.25);
						DWORD y1 = floor(ScreenRes->iY/1.25);

						if(Lines) //Draw Lines
							DrawLine(x1, y1, fScreen[0], fScreen2[1], COLOR, pLine, pDevice);

						if(HP) //Show Health
							DrawBorder(fScreen[0]-(x/2)-(width), fScreen[1]-10.f, 3, GetHPHeight(pClient, (y+10.f)), width, GREEN, pDevice);

						if(Weapons) //Show Weapon
							PrintText(GetWeaponName(pClient->iCurrentWeapon), fScreen[0]-(x/2)-(width), fScreen[1]-25.f, COLOR, pFont);
					}
				}
			}
		}
/*

		if(ShowItems)
		{
			float flOut[2];
			char* Item(nullptr);
			bool allow(false);

			if(pEntity->eType != 1)
				continue;

			if(pEntity->Valid == 5)
			{
				if(pEntity->eFlag == 1280)
					PrintText("Chopper", flOut[0]-10.f, flOut[1], ORANGE, pFont);
				else if(pEntity->eFlag == 48)
					PrintText("Gas", flOut[0]-10.f, flOut[1], ORANGE, pFont);
			}

			if(pEntity->Valid == 3)
			{
				if(pEntity->eType == 1)// || (pEntity->eType == 5))
				{
					if(pEntity->Weapon == 72)
					{
						Item = "Flash";
						PrintText("Flash", flOut[0]-10.f, flOut[1], ORANGE, pFont);
						allow = 1;
					}
					else if(pEntity->Weapon == 3)
					{
						Item = "Frag";
						PrintText("Frag", flOut[0]-10.f, flOut[1], ORANGE, pFont);
						allow = 1;
					}
				}
			}
		}*/
	}
}

void Aimbot(int Aimbot, int Triggerbot, vec3_t Target)
{
	if(IsInGame()){
		if(Aimbot){
			//TODO: Add aimbot

		}
	}
}

void Crosshair(int _switch)
{
	if(_switch != 0){

	}
}