#define _DEFINE_PTRS
#include "BH.h"
#include <Shlwapi.h>
#include "D2Ptrs.h"
#include "D2Intercepts.h"
#include "D2Handlers.h"
#include "Modules.h"

string BH::path;
HINSTANCE BH::instance;
ModuleManager* BH::moduleManager;
Config* BH::config;
Drawing::UI* BH::settingsUI;
bool BH::cGuardLoaded;
WNDPROC BH::OldWNDPROC;

Patch* patches[] = {
	new Patch(Call, D2CLIENT, 0x4F28B, (int)GameLoop_Interception, 6),

	new Patch(Jump, D2CLIENT, 0x572D8, (int)GameDraw_Interception, 6),
	new Patch(Jump, D2CLIENT, 0x5ADB3, (int)GameAutomapDraw_Interception, 5),

	//new Patch(Call, BNCLIENT, 0xCEBC, (int)ChatPacketRecv_Interception, 12),//updated
	//new Patch(Call, D2MCPCLIENT, 0x6297, (int)RealmPacketRecv_Interception, 5),//updated

		new Patch(Call, D2CLIENT, 0x5F802, (int)GamePacketRecv_Interception, 5),
		new Patch(Call, D2CLIENT, 0x7C89D, (int)GameInput_Interception, 5),
	//new Patch(Call, D2MULTI, 0x11D63, (int)ChannelInput_Interception, 5),//updated
	//new Patch(Call, D2MULTI, 0x14A9A, (int)ChannelWhisper_Interception, 5),
	//new Patch(Jump, D2MULTI, 0x14BE0, (int)ChannelChat_Interception, 6),
	//new Patch(Jump, D2MULTI, 0x14850, (int)ChannelEmote_Interception, 6),

};

//Patch* BH::oogDraw = new Patch(Call, D2WIN, 0xEC61, (int)OOGDraw_Interception, 5);

unsigned int index = 0;
bool BH::Startup(HINSTANCE instance, VOID* reserved) {

	BH::instance = instance;
	if (reserved != NULL) {
		cGuardModule* pModule = (cGuardModule*)reserved;
		if(!pModule)
			return FALSE;
		path.assign(pModule->szPath);
		cGuardLoaded = true;
	} else {
		char szPath[MAX_PATH];
		GetModuleFileName(BH::instance, szPath, MAX_PATH);
		PathRemoveFileSpec(szPath);
		path.assign(szPath);
		path += "\\";
		cGuardLoaded = false;
	}

	moduleManager = new ModuleManager();
	config = new Config("BH.cfg");
	config->Parse();

	for (int n = 0; n < (sizeof(patches) / sizeof(Patch*)); n++) {
		patches[n]->Install();
	}

	/*if (!D2CLIENT_GetPlayerUnit())
		oogDraw->Install();*/

	if(D2GFX_GetHwnd()) {
		BH::OldWNDPROC = (WNDPROC)GetWindowLong(D2GFX_GetHwnd(),GWL_WNDPROC);
		SetWindowLong(D2GFX_GetHwnd(),GWL_WNDPROC,(LONG)GameWindowEvent);
	}

	settingsUI = new Drawing::UI("Settings", 350, 200);

	new Maphack();
	new ScreenInfo();
	////new Gamefilter();
	new Bnet();
	new Item();
	////new SpamFilter();
	new AutoTele();
	new Party();

	CreateThread(0,0,GameThread, 0,0,0);

	moduleManager->LoadModules();

	return true;
}

bool BH::Shutdown() {

	moduleManager->UnloadModules();

	delete moduleManager;
	delete settingsUI;

	SetWindowLong(D2GFX_GetHwnd(),GWL_WNDPROC,(LONG)BH::OldWNDPROC);
	for (int n = 0; n < (sizeof(patches) / sizeof(Patch*)); n++) {
		delete patches[n];
	}

	/*oogDraw->Remove();*/

	delete config;

	return true;
}