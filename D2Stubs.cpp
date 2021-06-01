#include <Windows.h>
#include "D2Ptrs.h"

DWORD __declspec(naked) __fastcall D2CLIENT_GetUnitName_STUB(DWORD UnitAny)
{
	__asm
	{
		mov eax, ecx
		jmp D2CLIENT_GetUnitName_I
	}
}

DWORD __declspec(naked) __fastcall D2CLIENT_InitAutomapLayer(DWORD nLayerNo)
{
	__asm 
	{
		push edi;
		mov edi, ecx;
		call D2CLIENT_InitAutomapLayer_I;
		pop edi;
		ret;
	}
}

DWORD __declspec(naked) __fastcall D2CLIENT_GetLevelName_STUB(DWORD levelId)
{
	__asm
	{
		jmp D2CLIENT_GetLevelName_I
	}
}

//__declspec(naked) CellFile* __fastcall D2CLIENT_LoadUiImage(CHAR* szPath)
//{
//	__asm
//	{
//		MOV EAX, ECX
//		PUSH 0
//		CALL D2CLIENT_LoadUiImage_I
//		RETN
//	}
//}

DWORD __declspec(naked) __fastcall D2CLIENT_ClickParty_ASM(RosterUnit* RosterUnit, DWORD Mode)
{
	__asm
	{
		mov eax, ecx
		jmp D2CLIENT_ClickParty_I
	}
}