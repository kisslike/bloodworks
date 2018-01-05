#pragma once

#include <string>

#include "Sol.h"
#include "cMat.h"
#include "cVec.h"
#include "cSound.h"
#include "DirentHelper.h"
#include "BuffFloat.h"
#include "json.h"
#include "cPersistent.h"

class Bloodworks;
class cRenderable;
class Bullet;
class GameObject;
class Monster;
class GameObjectTemplate;

class MissionMod
{
	friend class BloodworksLuaWorld;

	sol::table scriptTable;

	sol::function onTickFunc;
	sol::function initFunc;

	cPersistent persistent;

	sol::table data;
public:
	MissionMod(nlohmann::json& j, const DirentHelper::File& file);
	~MissionMod();

	void init();
	void onTick();
};

class MissionController
{
	Bloodworks *bloodworks;
	sol::table scriptTable;
	
	std::unordered_map<int, GameObject*> gameObjects;

	struct MissionData
	{
		std::string name;
		std::string icon;
		std::string description;
		std::string scriptName;
		std::string scriptFile;
		std::string basePath;

		cPersistent persistent;
	};

	cVector<MissionData*> missions;
	float missionLoadTime;
	int loadedMission;

	cSoundSampleWithParams missionLoop;
	cSoundHandle missionLoopHandle;
	float soundSpeed;
	float musicVolume;

	BuffFloat gameSpeedMultiplier; // this can be stored in a mission class

	cVector< MissionMod* > missionMods;
public:
	MissionController(Bloodworks *bloodworks);
	~MissionController();
	void tick();
	GameObject* addGameObject(const std::string& script, GameObjectTemplate *gameObjectTemplate, const sol::table& params);
	GameObject* addGameObjectUsingTemplate(const std::string& templateName, const sol::table& params);
	void removeGameObject(int id);
	void addMission(nlohmann::json& j, const DirentHelper::File& file);
	void addMissionMod(nlohmann::json& j, const DirentHelper::File& file);

	void loadMission(const std::string& name);
	void reset();
	void onPlayerDied();
	bool isLoaded() const;

	sol::table getMissionData();
	void onMonsterDied(Monster* monster);
	void repositionGUI();
	void updateMusicVolume();
	void onDebugTick();
	float getGameSpeedMultiplier()
	{
		return gameSpeedMultiplier.getBuffedValue();
	}
	BuffFloat& getGameSpeedMultiplierBuff()
	{
		return gameSpeedMultiplier;
	}
	bool canExit();

	int getMissionCount() const
	{
		return missions.size();
	}

	float getMissionStartTime() const
	{
		return missionLoadTime;
	}

	const std::string& GetMissionName(int index) const
	{
		return missions[index]->name;
	}
	const std::string& GetMissionDecription(int index) const
	{
		return missions[index]->description;
	}
	const std::string& GetMissionIcon(int index) const
	{
		return missions[index]->icon;
	}
	const std::string& GetMissionScriptName(int index) const
	{
		return missions[index]->scriptName;
	}

	std::string getCurrentMissionScript();
};