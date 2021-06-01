#pragma once
#include <Windows.h>

DWORD __fastcall D2CLIENT_GetUnitName_STUB(DWORD UnitAny);
DWORD __fastcall D2CLIENT_InitAutomapLayer(DWORD nLayerNo);
DWORD __fastcall D2CLIENT_GetLevelName_STUB(DWORD levelId);
//CellFile* __fastcall D2CLIENT_LoadUiImage(CHAR* szPath);
DWORD __fastcall D2CLIENT_ClickParty_ASM(RosterUnit* RosterUnit, DWORD Mode);