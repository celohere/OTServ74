#include "otpch.h"

#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

#include "configmanager.h"
#include "luascript.h"
#include "scriptmanager.h"

#include "Actions.h"
#include "creatureevent.h"
#include "globalevent.h"
#include "movement.h"
#include "spells.h"
#include "talkaction.h"
#include "weapons.h"

Actions* g_actions = nullptr;
TalkActions* g_talkactions = nullptr;
Spells* g_spells = nullptr;
MoveEvents* g_moveEvents = nullptr;
Weapons* g_weapons = nullptr;
CreatureEvents* g_creatureEvents = nullptr;
GlobalEvents* g_globalEvents = nullptr;

extern ConfigManager g_config;


ScriptingManager* ScriptingManager::_instance = nullptr;

ScriptingManager::ScriptingManager()
{
	//
}

ScriptingManager::~ScriptingManager()
{
	//
}

ScriptingManager* ScriptingManager::getInstance()
{
	if (_instance == nullptr) {
		_instance = new ScriptingManager();
	}
	return _instance;
}


bool ScriptingManager::loadScriptSystems()
{
	std::cout << ":: Loading Script Systems" << std::endl;

	std::string datadir = g_config.getString(ConfigManager::DATA_DIRECTORY);

	// load weapons data
	std::cout << ":: Loading Weapons ...";
	g_weapons = new Weapons();
	if (!g_weapons->loadFromXml(datadir)) {
		LOG_ERROR("Unable to load Weapons!");
		return false;
	}

	g_weapons->loadDefaults();
	std::cout << "[done]" << std::endl;

	// load spells data
	std::cout << ":: Loading Spells ...";
	g_spells = new Spells();
	if (!g_spells->loadFromXml(datadir)) {
		LOG_ERROR("Unable to load Spells!");
		return false;
	}
	std::cout << "[done]" << std::endl;

	/*
	std::cout << ":: Loading Fields ...";
	Items::loadFieldsFromXml(datadir);
	std::cout << "[done]" << std::endl;
	*/

	// load actions data
	std::cout << ":: Loading Actions ...";
	g_actions = new Actions();
	if (!g_actions->loadFromXml(datadir)) {
		LOG_ERROR("Unable to load Actions!");
		return false;
	}
	std::cout << "[done]" << std::endl;

	// load talkactions data
	std::cout << ":: Loading Talkactions ...";
	g_talkactions = new TalkActions();
	if (!g_talkactions->loadFromXml(datadir)) {
		LOG_ERROR("Unable to load Talkactions!");
		return false;
	}
	std::cout << "[done]" << std::endl;

	// load moveEvents
	std::cout << ":: Loading MoveEvents ...";
	g_moveEvents = new MoveEvents();
	if (!g_moveEvents->loadFromXml(datadir)) {
		LOG_ERROR("Unable to load MoveEvents!");
		return false;
	}
	std::cout << "[done]" << std::endl;

	// load creature events
	std::cout << ":: Loading CreatureEvents ...";
	g_creatureEvents = new CreatureEvents();
	if (!g_creatureEvents->loadFromXml(datadir)) {
		LOG_ERROR("Unable to load CreatureEvents!");
		return false;
	}
	std::cout << "[done]" << std::endl;

	// load global events
	std::cout << ":: Loading GlobalEvents ...";
	g_globalEvents = new GlobalEvents();
	if (!g_globalEvents->loadFromXml(datadir)) {
		LOG_ERROR("Unable to load GlobalEvents!");
		return false;
	}
	std::cout << "[done]" << std::endl;

	return true;
}
