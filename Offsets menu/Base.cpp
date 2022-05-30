#include "Menu.h"
#include "ColorDefinitions.h"
#pragma once
LPDIRECT3DDEVICE9 g_pDevice = NULL;
oReset pReset;
oEndScene pEndScene;
Menu dMenu;

void ReFont(LPDIRECT3DDEVICE9 pDevice)
{
	if (g_pDevice != pDevice)
	{
		g_pDevice = pDevice;
		try
		{
			if (dMenu.pFont != 0)
				dMenu.pFont->Release();
			if (dMenu.pHeaderFont != 0)
				dMenu.pHeaderFont->Release();
		}
		catch (...) {}
		dMenu.pFont = 0;
		dMenu.pHeaderFont = 0;
		D3DXCreateFontA(pDevice, 14, 0, FW_NORMAL, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &dMenu.pFont);
		D3DXCreateFontA(pDevice, 14, 0, FW_BOLD, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &dMenu.pHeaderFont);
	}
}

HRESULT WINAPI Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	dMenu.pFont->OnLostDevice();
	dMenu.pHeaderFont->OnLostDevice();
	HRESULT hRet = pReset(pDevice, pPresentationParameters);
	dMenu.pFont->OnResetDevice();
	dMenu.pHeaderFont->OnResetDevice();
	return hRet;
}

HRESULT WINAPI EndScene(LPDIRECT3DDEVICE9 pDevice)
{
	ReFont(pDevice);
	dMenu.BuildMenu("Eidolon' D3D", 10, 0, 190, 200, GOLD, RED, BLACK, pDevice);
	return pEndScene(pDevice);
}

int D3Dinit(void)
{
	//Find D3D9.Direct3DCreate9EX
	//And extract the virtual call table 
	//In order to hook on the endscene and reset functions
	DWORD           hD3D, adr, *vtbl;
	do {
		hD3D = (DWORD)GetModuleHandle("d3d9.dll");
		Sleep(10);
	} while (!hD3D);

	adr = FindPattern(hD3D, 15, (PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx");
	if (adr) {
		memcpy(&vtbl, (void *)(adr + 2), 4);
		pReset = (oReset)DetourFunction((PBYTE)vtbl[16], (PBYTE)Reset, 5);
		pEndScene = (oEndScene)DetourFunction((PBYTE)vtbl[42], (PBYTE)EndScene, 5);
	}
	return 0;
}

BOOL WINAPI DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hDll);
	if (dwReason == DLL_PROCESS_ATTACH)
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)D3Dinit, 0, 0, 0);
	return TRUE;
}