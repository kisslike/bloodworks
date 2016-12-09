#include "Gun.h"
#include "Bloodworks.h"
#include "cGlobals.h"
#include "BulletController.h"
#include "Player.h"
#include "Bullet.h"
#include "cRenderable.h"
#include "json.h"	

using json = nlohmann::json;

void Gun::init(Bloodworks *bloodworks, const char *gunData)
{
	this->bloodworks = bloodworks;

	std::string jsonFile;
	textFileRead(gunData, jsonFile);
	json j = json::parse(jsonFile.c_str());

	name = j["name"].get<std::string>();
	artFolder = j["artFolder"].get<std::string>();
	fixFolderPath(artFolder);

	if (j.count("hideSpread"))
	{
		hideSpread = j["hideSpread"].get<bool>();
	}
	else
	{
		hideSpread = false;
	}

	if (j.count("bulletTexture"))
	{
		bulletTexturePath = artFolder + j["bulletTexture"].get<std::string>();
		bulletTexture = resources.getTexture(bulletTexturePath.c_str());
	}
	if (j.count("bulletMeshShift"))
	{
		bulletMeshShift.w = j["bulletMeshShift"].at(0).get<float>();
		bulletMeshShift.h = j["bulletMeshShift"].at(1).get<float>();
	}
	else
	{
		bulletMeshShift.setZero();
	}
	iconPath = artFolder + j["icon"].get<std::string>();

	bulletSize.w = j["bulletSize"].at(0).get<float>();
	bulletSize.h = j["bulletSize"].at(1).get<float>();
	bulletRadius = j["bulletRadius"].get<float>();
	bulletSpeed = j["bulletSpeed"].get<float>();

	damage = IntVec2(j["bulletDamage"].at(0).get<int>(), j["bulletDamage"].at(1).get<int>());

	std::string scriptFilePath = j["scriptFile"].get<std::string>();
	fixFilePath(scriptFilePath);
	scriptName = j["scriptName"].get<std::string>();
	scriptTable = lua[j["scriptName"].get<std::string>()] = lua.create_table();
	data = lua.create_table();
	id = bloodworks->getUniqueId();


	spreadAngle = 0.0f;
	crosshairDistance = 400.0f;

	lua.script_file(scriptFilePath);
	scriptTable["init"](this);

}

void Gun::stop()
{
}

void Gun::start()
{
	isTriggered = false;
}

void Gun::tick(float dt)
{
	scriptTable["onTick"](this);
}

float Gun::getMaxCrosshairDistance()
{
	return crosshairDistance;
}

float Gun::getSpreadAngle()
{
	return spreadAngle;
}

int Gun::getId()
{
	return id;
}

sol::table& Gun::getScriptTable()
{
	return scriptTable;
}

const std::string& Gun::getIconPath() const
{
	return iconPath;
}

bool Gun::spreadVisible() const
{
	return !hideSpread;
}

void Gun::setTriggered(bool triggered)
{
	this->isTriggered = triggered;
}

Gun::~Gun()
{
	bulletTexture = nullptr;
}

Bullet* Gun::addBullet()
{
	Vec2 dir = bloodworks->getPlayer()->getAimDir();
	Bullet *bullet = new Bullet(bloodworks, this);
	Player *player = bloodworks->getPlayer();
	bullet->pos = player->getPos() + player->getAimDir() * 22 - player->getAimDir().sideVec() * 4.0f;
	bullet->moveSpeed = bulletSpeed;
	bullet->moveAngle = player->getAimDir().toAngle() + randFloat(-spreadAngle, spreadAngle);
	bullet->radius = bulletRadius;
	bullet->damage = randInt(damage.x, damage.y);
	if (bulletTexture != nullptr)
	{
		cTexturedQuadRenderable *renderable = new cTexturedQuadRenderable(bloodworks, bulletTexturePath.c_str(), "resources/default");
		renderable->setWorldMatrix(Mat3::scaleMatrix(bulletSize).translateBy(bulletMeshShift));
		bullet->addRenderable(renderable);
	}
	bloodworks->getBulletController()->addBullet(bullet);

	bloodworks->onAddedGunBullet(this, bullet);
	return bullet;
}
