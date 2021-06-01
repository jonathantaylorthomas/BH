#pragma once
#include "../Module.h"

struct Control;

class Bnet : public Module {
	private:
		bool showLastGame;
		static unsigned int failToJoin;
		static std::string lastName;

	public:

		Bnet() : Module("Bnet") {};

		void OnLoad();
		void OnUnload();

		void OnGameJoin(const string& name, const string& pass, int diff);
		void OnGameExit();

		static void __fastcall NextGamePatch(Control* box, bool(__stdcall *FunCallBack)(Control*,DWORD,DWORD));
};

void FailToJoin_Interception();