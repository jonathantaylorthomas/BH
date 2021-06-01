#include "Config.h"
#include "BH.h"
#include <sstream>

/* Parse()
	Parse the configuration file and stores the results in a key->value pair.
	Can be called multiple times so you can reload the configuration.
*/
bool Config::Parse() {
	
	//If this is a config with a pre-set of values, can't parse!
	if (configName.length() == 0)
		return false;

	//Open the configuration file
	fstream file(BH::path + configName);
	if (!file.is_open())
		return false;

	//Lock Critical Section then wipe contents incase we are reloading.
	contents.erase(contents.begin(), contents.end());

	//Begin to loop the configuration file one line at a time.
	string line;
	char szLine[2048];
	while(!file.eof()) {
		//Get the current line from the file.
		file.getline(szLine, 2048);
		line = szLine;

		//Remove any comments from the config
		if (line.find_first_of("//") != string::npos)
			line = line.erase(line.find_first_of("//"));

		//Insure we have something in the line now.
		if (line.length() == 0)
			continue;

		//Grab the Key and Value
		string key = Trim(line.substr(0, line.find_first_of(":")));
		string value = Trim(line.substr(line.find_first_of(":") + 1));

		//Store them!
		contents.insert(pair<string, string>(key,value));
	}
	return true;
}

bool Config::Write() {
	//If this is a config with a pre-set of values, can't parse!
	if (configName.length() == 0)
		return false;

	//Open the configuration file
	fstream file(BH::path + configName);
	if (!file.is_open())
		return false;

	//Read in the configuration value
	vector<string> configLines;
	char line[2048];
	while(!file.eof()) {
		file.getline(line, 2048);
		configLines.push_back(line);
	}

	for (vector<string>::iterator it = configLines.begin(); it < configLines.end(); it++) {
		//Remove any comments from the config
		if ((*it).find_first_of("//") != string::npos)
			(*it) = (*it).erase((*it).find_first_of("//"));

		//Insure we have something in the line now.
		if ((*it).length() == 0)
			continue;
	}

}

/* ReadBoolean(std::string key, bool value)
 *	Reads in a boolean from the key-pair.
 */
bool Config::ReadBoolean(std::string key, bool value) {
	//Check if configuration value exists
	if (contents[key].length() == 0)
		return value;
	//Convert string to boolean
	const char* szValue = contents[key].c_str();
	if ((_stricmp(szValue, "1") == 0) || (_stricmp(szValue, "y") == 0) || (_stricmp(szValue, "yes") == 0) || (_stricmp(szValue, "true") == 0))
		return true;
	return false;
}

/* ReadInt(std::string key, int value)
 *	Reads in a decimal or hex(which is converted to decimal) from the key-pair.
 */
int Config::ReadInt(std::string key, int value) {
	//Check if configuration value exists
	if (contents[key].length() == 0)
		return value;
	//Use a string stream to convert values
	stringstream convert;
	int ret;
	//Hack todo hex conversion
	if (!contents[key].find("0x"))
		from_string<int>(ret,contents[key],std::hex);
	else
		from_string<int>(ret,contents[key],std::dec);
	return ret;
}

/* ReadToggle(std::string key, std::string toggle, bool state)
 *	Reads in a toggle from the key->pair
 *	Config Example:
 *		Key: True, VK_A
 */
Toggle Config::ReadToggle(std::string key, std::string toggle, bool state) {
	//Check if configuration value exists.
	Toggle ret;
	ret.toggle = GetKeyCode(toggle.c_str()).value;
	ret.state = state;
	if (contents[key].length() == 0)
		return ret;

	//Pull the keycode from after the comma.
	ret.toggle = GetKeyCode(Trim(contents[key].substr(contents[key].find_first_of(",") + 1)).c_str()).value;
	
	//Pull the state from before the comma
	string boolean = Trim(contents[key].substr(0, contents[key].find_first_of(",")));
	const char* newState = boolean.c_str();
	
	//Convert state to boolean
	if ((_stricmp(newState, "1") == 0) || (_stricmp(newState, "y") == 0) || (_stricmp(newState, "yes") == 0) || (_stricmp(newState, "true") == 0))
		ret.state = true;
	else
		ret.state = false;

	return ret;
}

/* ReadKey(std::string key, std::string toggle)
 *	Reads in a key from the key->pair.
 */
unsigned int Config::ReadKey(std::string key, std::string toggle) {
	//Check if configuration value exists.
	if (contents[key].length() == 0)
		return GetKeyCode(toggle.c_str()).value;

	//Grab the proper key code and make s ure it's valid
	KeyCode ret = GetKeyCode(contents[key].c_str());
	if (ret.value == 0)
		return GetKeyCode(toggle.c_str()).value;

	return ret.value;
}

/* ReadArray(std::string key)
 *	Reads in a index-based array from the array
 */
vector<string> Config::ReadArray(std::string key) {
	vector<string> ret;
	int currentIndex = 0;
	while(true) {
		stringstream index;
		index << currentIndex;
		string currentKey = key + "[" + index.str() + "]";
		if (contents[currentKey].length() == 0)
			return ret;
		ret.push_back(contents[currentKey]);
		currentIndex++;
	}
	//Never should occur
	return ret;
}

/* ReadGroup(std::string key)
 *	Reads in a map from the key->pair
 *	Config Example:
 *		Value[Test]: 0
 *		Value[Pickles]: 1
 */
map<string, string> Config::ReadAssoc(std::string key) {
	map<string, string> ret;

	for (map<string, string>::iterator it = contents.begin(); it != contents.end(); it++) {
		if (!(*it).first.find(key + "[")) {
			pair<string,string> assoc;
			//Pull the value from between the []'s
			assoc.first = (*it).first.substr((*it).first.find("["), (*it).first.length() - (*it).first.find("[") - 1);
			//Simply store the value that was after the :
			assoc.second = (*it).second;
			ret.insert(assoc);
		}
	}

	return ret;
}