#include "ModuleManager.h"
#include "Module.h"

#include <algorithm>
#include <iterator>

ModuleManager::ModuleManager() {

}

ModuleManager::~ModuleManager() {
	while(moduleList.size() > 0) {
		Module* module = moduleList.begin()->second;
		delete module;
	}
}

void ModuleManager::FixName(std::string& name)
{
	std::transform(name.begin(), name.end(), name.begin(), tolower);
	std::replace(name.begin(), name.end(), ' ', '-');
}

void ModuleManager::Add(Module* module) {
	// Add to list of modules
	std::string name = module->GetName();
	FixName(name);
	moduleList[name] = module;
}

void ModuleManager::Remove(Module* module) {
	// Remove module from list
	std::string name = module->GetName();
	FixName(name);
	moduleList.erase(name);

	delete module;
}

void ModuleManager::LoadModules() {
	for (map<string, Module*>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
		(*it).second->Load();
	}
}

void ModuleManager::UnloadModules() {
	for (map<string, Module*>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
		(*it).second->Unload();
	}
}

bool ModuleManager::UserInput(wchar_t* module, wchar_t* msg, bool fromGame) {
	bool block = false;
	std::string name;
	std::wstring modname(module);
	name.assign(modname.begin(), modname.end());
	for (map<string, Module*>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
		if(it->first == name)
			__raise it->second->UserInput(msg, fromGame, &block);
	}
	return block;
}