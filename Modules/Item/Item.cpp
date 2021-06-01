#include "Item.h"
#include "../../D2Ptrs.h"
#include "../../BH.h"
#include "../../D2Stubs.h"

map<std::string, Toggle> Item::Toggles;
UnitAny* Item::viewingUnit;
Patch* itemNamePatch = new Patch(Call, D2CLIENT, 0x96736, (int)ItemName_Interception, 6);//updated 1.13d

Patch* viewInvPatch1 = new Patch(Call, D2CLIENT, 0x845D0, (int)ViewInventoryPatch1_ASM, 5);//updated 1.13d
Patch* viewInvPatch2 = new Patch(Call, D2CLIENT, 0x873DB, (int)ViewInventoryPatch2_ASM, 5);//updated 1.13d
Patch* viewInvPatch3 = new Patch(Call, D2CLIENT, 0x8E241, (int)ViewInventoryPatch3_ASM, 6);//updated 1.13d

using namespace Drawing;

void Item::OnLoad() {
	Toggles["Show Ethereal"] = BH::config->ReadToggle("Show Ethereal", "None", true);
	Toggles["Show Sockets"] = BH::config->ReadToggle("Show Sockets", "None", true);
	Toggles["Show iLvl"] = BH::config->ReadToggle("Show ILvl", "None", true);
	Toggles["Show Rune Numbers"] = BH::config->ReadToggle("Show Rune Numbers", "None", true);
	
	showPlayer = BH::config->ReadKey("Show Player", "VK_0");

	viewInvPatch1->Install();
	viewInvPatch2->Install();
	viewInvPatch3->Install();

	if (Toggles["Show Ethereal"].state || Toggles["Show Sockets"].state || Toggles["Show iLvl"].state)
		itemNamePatch->Install();

	settingsTab = new UITab("Item", BH::settingsUI);

	new Checkhook(settingsTab, 4, 15, &Toggles["Show Ethereal"].state, "Show Ethereal");
	new Keyhook(settingsTab, 130, 17, &Toggles["Show Ethereal"].toggle, "");

	new Checkhook(settingsTab, 4, 30, &Toggles["Show Sockets"].state, "Show Sockets");
	new Keyhook(settingsTab, 130, 32, &Toggles["Show Sockets"].toggle, "");

	new Checkhook(settingsTab, 4, 45, &Toggles["Show iLvl"].state, "Show iLvl");
	new Keyhook(settingsTab, 130, 47, &Toggles["Show iLvl"].toggle, "");

	new Checkhook(settingsTab, 4, 60, &Toggles["Show Rune Numbers"].state, "Show Rune #");
	new Keyhook(settingsTab, 130, 62, &Toggles["Show Rune Numbers"].toggle, "");

	new Keyhook(settingsTab, 4, 77, &showPlayer, "Show Players Gear");
}

void Item::OnUnload() {
	itemNamePatch->Remove();

	viewInvPatch1->Remove();
	viewInvPatch2->Remove();
	viewInvPatch3->Remove();
}

void Item::OnLoop() {
	if (!p_D2CLIENT_UIMode[0x01])
		viewingUnit = NULL;

	if (viewingUnit  && viewingUnit->dwUnitId) {
		if (!viewingUnit->pInventory){
			viewingUnit = NULL;
			D2CLIENT_SetUIVar(0x01, 1, 0);			
		} else if (!D2CLIENT_FindServerSideUnit(viewingUnit->dwUnitId, viewingUnit->dwType)) {
			viewingUnit = NULL;
			D2CLIENT_SetUIVar(0x01, 1, 0);
		}
	}
}

void Item::OnKey(bool up, BYTE key, LPARAM lParam, bool* block) {
	if (key == showPlayer) {
		*block = true;
		if (up)
			return;
		UnitAny* selectedUnit = D2CLIENT_GetSelectedUnit();
		if (selectedUnit && selectedUnit->dwMode != 0 && selectedUnit->dwMode != 17 && selectedUnit->dwType == 0) {
			viewingUnit = selectedUnit;
			if (!p_D2CLIENT_UIMode[0x01])
				D2CLIENT_SetUIVar(0x01, 0, 0);
			return;
		}
	}
	for (map<string,Toggle>::iterator it = Toggles.begin(); it != Toggles.end(); it++) {
		if (key == (*it).second.toggle) {
			*block = true;
			if (up) {
				(*it).second.state = !(*it).second.state;
			}
			return;
		}
	}
}

void Item::OnLeftClick(bool up, int x, int y, bool* block) {
	if (up)
		return;
	if (p_D2CLIENT_UIMode[0x01] && viewingUnit != NULL && x >= 400)
		*block = true;
}

void __fastcall Item::ItemNamePatch(wchar_t *name, UnitAny *item)
{
	char* szName = UnicodeToAnsi(name);
	string itemName = szName;

	if (Toggles["Show Sockets"].state) {
		int sockets = D2COMMON_GetUnitStat(item, 194, 0);
		if (sockets > 0)
			itemName += "(" + to_string(sockets) + ")";
	}

	if (Toggles["Show Ethereal"].state && item->pItemData->dwFlags & 0x400000)
		itemName += "(Eth)";

	if (Toggles["Show iLvl"].state)
		itemName += "(L" + to_string(item->pItemData->dwItemLevel) + ")";

	if (Toggles["Show Rune Numbers"].state && D2COMMON_GetItemText(item->dwTxtFileNo)->nType == 74)
		itemName = "[" + to_string(item->dwTxtFileNo - 609) + "]" + itemName;

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, itemName.c_str(), itemName.length(), name, itemName.length());
	delete[] szName;
}

UnitAny* Item::GetViewUnit ()
{
	UnitAny* view = (viewingUnit) ? viewingUnit : D2CLIENT_GetPlayerUnit();
	if (view->dwUnitId == D2CLIENT_GetPlayerUnit()->dwUnitId)
		return D2CLIENT_GetPlayerUnit();

	Drawing::Texthook::Draw(560, 300, Drawing::Center, 0, Drawing::White, "%s", viewingUnit->pPlayerData->szName);
	return viewingUnit;
}


void __declspec(naked) ItemName_Interception()
{
	__asm {
		mov ecx, edi
		mov edx, ebx
		call Item::ItemNamePatch
		mov al, [ebp+0x12a]
		ret
	}
}

void __declspec(naked) ViewInventoryPatch1_ASM()
{
	__asm {
		//push eax;
		call Item::GetViewUnit;
		mov esi, eax;
		//pop eax;
		ret;
	}
}
void __declspec(naked) ViewInventoryPatch2_ASM()
{
	__asm {
		//push eax;
		call Item::GetViewUnit;
		mov ebx, eax;
		//pop eax;
		ret;
	}
}
void __declspec(naked) ViewInventoryPatch3_ASM()
{
	__asm
	{
		push eax;
		call Item::GetViewUnit;
		mov edi, eax;
		pop eax;
		mov esi, [edi + 60h];
		lea ecx, [eax + 17h];
		ret;
	}
}