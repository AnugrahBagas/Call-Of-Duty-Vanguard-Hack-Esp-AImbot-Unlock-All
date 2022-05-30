#include "SystemIncludes.h"
#include "Classes.h"
#pragma once
#pragma warning(disable:4244)
#pragma warning(disable:4996)
#pragma warning(disable:4172)
#pragma warning(disable:4715)
#pragma warning(disable:4309)
#pragma warning(disable:4101)
typedef bool(*World2Screen_)(int unknown, w2s_t * w2scalc, vec3_t vPoint, float Screen[2]);
World2Screen_ w2s = (World2Screen_)S_WORLDTOSCREEN;

typedef void(__cdecl *CG_Trace_t)(trace_t *, vec3_t, vec3_t, vec3_t, int, int);
CG_Trace_t CG_Trace = (CG_Trace_t)S_TRACE;

typedef w2s_t *(*Getw2s_t)();
Getw2s_t GetMatrix = (Getw2s_t)S_GETSCREENMATRIX;

typedef HRESULT(WINAPI* oReset)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef HRESULT(WINAPI* oEndScene)(LPDIRECT3DDEVICE9 pDevice);

vec3_t nullvec = { 0, 0, 0 };

void Trace(trace_t *t, vec3_t start, vec3_t end, int skip, int mask)
{
	CG_Trace(t, start, end, nullvec, skip, mask);
}

char * GetWeaponName(DWORD dwNum)
{
	return (char*)(*(DWORD*)((dwNum & 0xFF) * 4 + OFFS_WeaponName));
}

DWORD GetHPHeight(client_t * pClient, int h)
{
	return (h * (pClient->iHealth / pClient->iMaxHealth));
}

bool Visible(vec3_t point)
{
	trace_t t;
	Trace(&t, RefDef->vOrigin, point, CG->ClientNum, 0x803003);

	return (t.fraction >= 0.97f);
}

void PushCommandToConsole(char * szCommand)
{
	DWORD dwCall = S_PCTC;
	__asm
	{
		push szCommand;
		push 0;
		call dwCall;
		add esp, 0x8;
	}
}

DWORD AimAssistCall1(0x421b20);
DWORD AimAssistCall2(0x4ba870);
__declspec(naked) void AimAssist()
{
	__asm {
		movzx   ecx, word ptr[esi + 0x150]
			push    eax
			push    ecx
			call    AimAssistCall1
			add     esp, 8
			test    eax, eax
			jnz     loc_1
			retn
			loc_1 :
		mov     edx, [esp + 0x4]
			push    edx
			push    edi
			push    eax
			push    esi
			call    AimAssistCall2
			add     esp, 0x10
			test    eax, eax
			jnz     loc_2
			mov		eax, 0
			loc_2:
		mov     eax, 1
			retn
	}
}

void GetTagPos(entity_t * pEnt, WORD wBone, float * vOut)
{
	__asm {
		push vOut
			movzx edi, wBone
			mov esi, pEnt
			call AimAssist
			add esp, 0x4
	}
}

WORD RegisterTag(char * szBone)
{
	DWORD dwCall = S_REGTAG;
	__asm {
		push 1
			push szBone
			call[dwCall]
			add esp, 0x8
	}
}

char * Ch(int n)
{
	char a[32];
	sprintf(a, "%i", n);
	return a;
}

char * fCharize(float number)
{
	char c[20];
	sprintf(c, "%f", number);
	return c;
}

void Crosshair1(int mid, DWORD Color, LPDIRECT3DDEVICE9 pDevice)
{
	//DrawDot(0, mid-16, Color, 
}

void DrawLine(int x1, int y1, int x2, int y2, D3DCOLOR Color, LPD3DXLINE pLine, LPDIRECT3DDEVICE9 pDevice)
{
	D3DXCreateLine(pDevice, &pLine);
	D3DXVECTOR2 line[] = { D3DXVECTOR2(x1, y1), D3DXVECTOR2(x2, y2) };
	pLine->Begin();
	pLine->Draw(line, 2, Color);
	pLine->End();
	pLine->Release();
}

void PrintText(char pString[], int x, int y, D3DCOLOR col, ID3DXFont *font, bool Center = 0, int w = 500, int h = 30)
{
	RECT FontRect = { x, y, x + w, y + h };
	if (!Center)
		font->DrawText(NULL, pString, -1, &FontRect, DT_LEFT | DT_WORDBREAK, col);
	if (Center)
		font->DrawText(NULL, pString, -1, &FontRect, DT_CENTER, col);
}

void FillRGB(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* pDevice)
{
	if (w < 0)w = 1;
	if (h < 0)h = 1;
	if (x < 0)x = 1;
	if (y < 0)y = 1;

	D3DRECT rec = { x, y, x + w, y + h };
	pDevice->Clear(1, &rec, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawBorder(int x, int y, int w, int h, int px, D3DCOLOR BorderColor, IDirect3DDevice9* pDevice)
{
	FillRGB(x, (y + h - px), w, px, BorderColor, pDevice);
	FillRGB(x, y, px, h, BorderColor, pDevice);
	FillRGB(x, y, w, px, BorderColor, pDevice);
	FillRGB((x + w - px), y, px, h, BorderColor, pDevice);
}

void DrawBox(int x, int y, int w, int h, D3DCOLOR BoxColor, D3DCOLOR BorderColor, IDirect3DDevice9* pDevice)
{
	FillRGB(x, y, w, h, BoxColor, pDevice);
	DrawBorder(x, y, w, h, 1, BorderColor, pDevice);
}

void DrawDot(int x, int y, DWORD color, LPDIRECT3DDEVICE9 pDevice)
{
	D3DRECT rec = { x, y, x + 1, y + 1 };
	pDevice->Clear(1, &rec, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawPoint(int x, int y, int w, int h, DWORD color, IDirect3DDevice9* pDevice)
{
	FillRGB((int)x, (int)y, (int)w, (int)h, color, pDevice);
}

void DrawCircle(int X, int Y, int radius, int numSides, DWORD Color, ID3DXLine * pLine)
{
	D3DXVECTOR2 Line[128];
	float Step = PI * 2.0 / numSides;
	int Count = 0;
	for (float a = 0; a < PI*2.0; a += Step)
	{
		float X1 = radius * cos(a) + X;
		float Y1 = radius * sin(a) + Y;
		float X2 = radius * cos(a + Step) + X;
		float Y2 = radius * sin(a + Step) + Y;
		Line[Count].x = X1;
		Line[Count].y = Y1;
		Line[Count + 1].x = X2;
		Line[Count + 1].y = Y2;
		Count += 2;
	}
	pLine->Begin();
	pLine->Draw(Line, Count, Color);
	pLine->End();
}

bool isMouseinRegion(int x1, int y1, int x2, int y2)
{
	POINT cPos;
	GetCursorPos(&cPos);
	if (cPos.x > x1 && cPos.x < x2 && cPos.y > y1 && cPos.y < y2) {
		return true;
	}
	else {
		return false;
	}
}

bool bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return 0;
	return (*szMask) == NULL;
}

DWORD FindPattern(DWORD dwAddress, DWORD dwLen, BYTE *bMask, char * szMask)
{
	for (DWORD i = 0; i < dwLen; i++)
		if (bCompare((BYTE*)(dwAddress + i), bMask, szMask))
			return (DWORD)(dwAddress + i);
	return 0;
}

void *DetourFunction(BYTE *src, const BYTE *dst, const int len)
{
	BYTE *jmp = (BYTE*)malloc(len + 5);
	DWORD dwBack;

	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &dwBack);
	memcpy(jmp, src, len);
	jmp += len;
	jmp[0] = 0xE9;
	*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;
	for (int i = 5; i < len; i++)  src[i] = 0x90;
	VirtualProtect(src, len, dwBack, &dwBack);
	return (jmp - len);
}

void JMP(BYTE *pAddress, DWORD dwJumpTo, DWORD dwLen)
{
	DWORD dwOldProtect, dwBkup, dwRelAddr;

	VirtualProtect(pAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	dwRelAddr = (DWORD)(dwJumpTo - (DWORD)pAddress) - 5;
	*pAddress = 0xE9;

	*((DWORD *)(pAddress + 0x1)) = dwRelAddr;
	for (DWORD x = 0x5; x < dwLen; x++) *(pAddress + x) = 0x90;
	VirtualProtect(pAddress, dwLen, dwOldProtect, &dwBkup);

	return;
}

DWORD Distance(vec3_t entity1, vec3_t entity2)
{
	float A, B;
	A = fabs(entity1[0] - entity2[0]);
	B = fabs(entity1[1] - entity2[1]);
	float C = sqrt(A*A + B*B);
	return floor(C / 10);
}