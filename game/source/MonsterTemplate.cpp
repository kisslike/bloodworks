#include "MonsterTemplate.h"
#include "cGlobals.h"
#include "json.h"
#include "bloodworks.h"

MonsterTemplate::MonsterTemplate(Bloodworks* bloodworks, nlohmann::json& j, const DirentHelper::File& file)
{
	name = j["name"].get<std::string>();
	size = Vec2(j["size"].at(0).get<float>(), j["size"].at(1).get<float>());
	basePath = file.folder;
	if (j.count("textureShift"))
	{
		textureShift = Vec2(j["textureShift"].at(0).get<float>(), j["textureShift"].at(1).get<float>());
	}
	else
	{
		textureShift.setZero();
	}
	hitPoint = j["hitPoint"].get<int>();
	experience = j["experience"].get<int>();
	score = j["score"].get<int>();
	bulletRadius = j["bulletRadius"].get<float>();
	collisionRadius = j["collisionRadius"].get<float>();

	if (j.count("hasBlood"))
	{
		hasBlood = j["hasBlood"].get<bool>();
	}
	else
	{
		hasBlood = true;
	}

	auto& animations = j["animations"];

	for (nlohmann::json::iterator it = animations.begin(); it != animations.end(); ++it)
	{
		cAnimatedTexturedQuadRenderable::AnimationData data = getAnimationData(it, file);
		animationData.push_back(data);
	}
	scriptName = j["scriptName"].get<std::string>();
	scriptTable = lua[scriptName] = lua.create_table();
	scriptPath = file.folder + j["scriptFile"].get<std::string>();
	bloodworks->loadLuaFile(scriptPath);

	if (j.count("bitSpeed"))
	{
		bitSpeed = j["bitSpeed"].get<float>();
	}
	else
	{
		bitSpeed = 1.0f;
	}
	auto& parts = j["bodyParts"];

	for (nlohmann::json::iterator it = parts.begin(); it != parts.end(); ++it)
	{
		auto& val = it.value();
		BodyPartData data;
		if (val.is_array())
		{
			data.texture = resources.getTexture(file.folder + val[0].get<std::string>());
			data.shift = Vec2(val[1].get<float>(), val[2].get<float>());
		}
		else
		{
			data.texture = resources.getTexture(file.folder + it.value().get<std::string>());
			data.shift.setZero();
		}
		bodyParts.push_back(data);
	}

	auto& partBits = j["bodyPartBits"];

	for (nlohmann::json::iterator it = partBits.begin(); it != partBits.end(); ++it)
	{
		bodyPartBits.push_back(resources.getTexture(file.folder + it.value().get<std::string>()));
	}

	if (j.count("hitSounds"))
	{
		auto& hitSoundsJson = j["hitSounds"];

		for (nlohmann::json::iterator it = hitSoundsJson.begin(); it != hitSoundsJson.end(); ++it)
		{
			hitSounds.push_back(resources.getSoundSample(file.folder + it.value().get<std::string>()));
		}
	}

	if (j.count("killSounds"))
	{
		auto& killSoundsJson = j["killSounds"];

		for (nlohmann::json::iterator it = killSoundsJson.begin(); it != killSoundsJson.end(); ++it)
		{
			killSounds.push_back(resources.getSoundSample(file.folder + it.value().get<std::string>()));
		}
	}


	scriptArgs = lua.create_table();
	if (j.count("scriptArgs"))
	{
		auto& scriptArgsJson = j["scriptArgs"];
		for (nlohmann::json::iterator it = scriptArgsJson.begin(); it != scriptArgsJson.end(); ++it)
		{
			switch (it.value().type())
			{
			case nlohmann::json::value_t::boolean:
				scriptArgs[it.key()] = it.value().get<bool>();
				break;
			case nlohmann::json::value_t::number_float:
				scriptArgs[it.key()] = it.value().get<float>();
				break;
			case nlohmann::json::value_t::number_integer:
				scriptArgs[it.key()] = it.value().get<int>();
				break;
			case nlohmann::json::value_t::number_unsigned:
				scriptArgs[it.key()] = it.value().get<unsigned>();
				break;
			case nlohmann::json::value_t::string:
				scriptArgs[it.key()] = it.value().get<std::string>();
				break;
			default:
				break;
			}
		}
	}
}

MonsterTemplate::~MonsterTemplate()
{
	animationData.clear();
	for (auto& bodyPart : bodyParts)
	{
		bodyPart.texture = nullptr;
	}
	bodyParts.clear();
	for (auto& bodyPart : bodyPartBits)
	{
		bodyPart = nullptr;
	}
	bodyPartBits.clear();
	for (auto& hitSound : hitSounds)
	{
		hitSound = nullptr;
	}
	hitSounds.clear();
}

const std::string& MonsterTemplate::getScriptName() const
{
	return scriptName;
}

