#pragma once
#include "../Module.h"
#include "../../Config.h"
#include "../../Drawing.h"

struct UnitAny;

class Item : public Module {
	private:
		static map<std::string, Toggle> Toggles;
		unsigned int showPlayer;
		static UnitAny* viewingUnit;
		Drawing::UITab* settingsTab;
	public:

		Item() : Module("Item") {};

		void OnLoad();
		void OnUnload();

		void OnLoop();
		void OnKey(bool up, BYTE key, LPARAM lParam, bool* block);
		void OnLeftClick(bool up, int x, int y, bool* block);

		static void __fastcall ItemNamePatch(wchar_t *name, UnitAny *item);
		static UnitAny* GetViewUnit();
};

void ItemName_Interception();
void ViewInventoryPatch1_ASM();
void ViewInventoryPatch2_ASM();
void ViewInventoryPatch3_ASM();