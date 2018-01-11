#include "DropController.h"
#include "Bloodworks.h"
#include "cRenderable.h"
#include "cRenderableContainer.h"
#include "cTextRenderable.h"
#include "cTexturedQuadRenderable.h"
#include "cFont.h"
#include "cTexture.h"
#include "Player.h"
#include "Bonus.h"
#include "Monster.h"
#include "Gun.h"
#include "BloodworksControls.h"
#include "MonsterController.h"
#include "MissionController.h"

DropController::~DropController()
{
	reset();
}

int DropController::spawnGun(const Vec2& position, int forceIndex, float spawnTime)
{
	Drop drop;
	drop.bonus = nullptr;
	drop.time = timer.getTime();
	drop.canFadeout = false;
	drop.spawnTime = spawnTime;
	drop.id = bloodworks->getUniqueId();
	auto& guns = bloodworks->getGuns();

	if (forceIndex == -1)
	{
		int t = 10;
		do
		{
			drop.gun = guns[randInt(guns.size())];
		} while (drop.gun == bloodworks->getPlayer()->getGun() && t-- > 0);
	}
	else
	{
		drop.gun = guns[forceIndex];
	}

	drop.pos = position;

	drop.renderable = new cTexturedQuadRenderable(bloodworks, drop.gun->getIconPath(), "resources/default");
	Vec2 textureSize = drop.renderable->getTexture()->getDimensions().toVec();
	if (textureSize.w > 32.0f)
	{
		textureSize *= 32.0f / textureSize.w;
	}
	if (textureSize.h > 17.0f)
	{
		textureSize *= 17.0f / textureSize.h;
	}
	drop.renderable->setWorldMatrix(Mat3::scaleMatrix(textureSize).rotateBy(-pi * 0.15f).translateBy(position));
	bloodworks->addRenderable(drop.renderable, PLAYER - 1);

	drop.text = new cTextRenderable(bloodworks, resources.getFont("resources/fontSmallData.txt"), drop.gun->getName(), 11);
	drop.text->setTextAlignment(TextAlignment::center);
	drop.text->setWorldMatrix(Mat3::translationMatrix(position + Vec2(0.0f, 15.0f)));
	bloodworks->addRenderable(drop.text, GUI - 10);

	drops.push_back(drop);

	return drop.id;
}

int DropController::spawnBonus(const Vec2& position, int forceIndex, float spawnTime)
{
	Drop drop;
	drop.canFadeout = false;
	drop.time = timer.getTime();
	drop.spawnTime = spawnTime;
	drop.id = bloodworks->getUniqueId();
	auto& bonuses = bloodworks->getBonuses();
	if (forceIndex >= 0)
	{
		drop.bonus = bonuses[forceIndex];
	}
	else
	{
		drop.bonus = bonuses[randInt(bonuses.size())];
	}
	drop.gun = nullptr;
	drop.pos = position;


	drop.renderable = new cTexturedQuadRenderable(bloodworks, drop.bonus->getIconPath(), "resources/default");
	Vec2 textureSize = drop.renderable->getTexture()->getDimensions().toVec() * 0.12f;
	drop.renderable->setWorldMatrix(Mat3::scaleMatrix(textureSize).translateBy(position));
	bloodworks->addRenderable(drop.renderable, PLAYER - 1);

	drop.text = new cTextRenderable(bloodworks, resources.getFont("resources/fontSmallData.txt"), drop.bonus->getName(), 11);
	drop.text->setTextAlignment(TextAlignment::center);
	drop.text->setWorldMatrix(Mat3::translationMatrix(position + Vec2(0.0f, 15.0f)));
	bloodworks->addRenderable(drop.text, GUI - 10);

	drops.push_back(drop);
	return drop.id;
}

void DropController::tick()
{
	LastEntrySet S("DropController::tick");
	if (bloodworks->isFirstTick())
	{
		out << "DropController::tick\n";
	}
	if (bloodworks->isMissionLoaded() == false)
	{
		return;
	}
	bool dontSpawnBonus = bloodworks->getMissionController()->getMissionData()["dontSpawnBonus"];

	if (timer.getDt() > 0.0f && dontSpawnBonus == false )
	{
		float timeSinceLastDrop = timer.getTime() - lastRandomDropSpawn;
		float extraDropChance = (timeSinceLastDrop - 10.0f) / 12.0f;
		float r = randFloat();
		if (r < extraDropChance)
		{
			const Vec2& pos = bloodworks->getMonsterController()->getRandomPos(lua.create_table()); // todo this shouldnt be in monstercontroller
			lastRandomDropSpawn = timer.getTime();
			if (!bloodworks->IsGUIHidden())
			{
				spawnDrop(pos);
			}
		}
	}
	Vec2 playerPos = bloodworks->getPlayer()->getPosition();
	Vec2 crosshairPos = playerPos + bloodworks->getPlayer()->getCrosshairPos();
	for (int i = 0; i < drops.size(); i++)
	{
		auto& drop = drops[i];
		bool remove = false;

		if (timer.getTime() - drop.time < 0.3f)
		{
			float s = (timer.getTime() - drop.time) / 0.34f;
			drop.renderable->setColor(Vec4(1.0f, 1.0f, 1.0f, s));
		}
		else
		{
			drop.renderable->setColor(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		drop.text->setVisible(drop.pos.distanceSquared(crosshairPos) < 30.0f * 30.0f && bloodworks->IsGUIHidden() == false);

		if (drop.pos.distanceSquared(playerPos) < 30.0f * 30.0f)
		{
			if (drop.gun)
			{
				if (drop.gun->isUltimate())
				{
					bloodworks->getPlayer()->setSecondaryGun(drop.gun);
				}
				else
				{
					bloodworks->getPlayer()->setGun(drop.gun);
				}
				bloodworks->onPlayerPickedGun(drop.gun);
			}
			else
			{
				drop.bonus->spawnAt(drop.pos);
				bloodworks->onPlayerPickedBonus(drop.bonus, drop.pos);
			}
			remove = true;
		}

		if (!dontSpawnBonus)
		{
			if (drop.time + drop.spawnTime < timer.getTime())
			{
				remove = true;
			}
			else if (drop.time + 35.0f < timer.getTime())
			{
				bool isFadeout = ((int)(timer.getTime() * 3)) % 2 != 0;
				if (isFadeout == false)
				{
					drop.canFadeout = true;
				}
				drop.renderable->setColor(Vec4(1.0f, 1.0f, 1.0, (isFadeout && drop.canFadeout) ? 0.2f : 1.0f));
			}
		}

		if (remove)
		{
			SAFE_DELETE(drop.renderable);
			SAFE_DELETE(drop.text);
			drops.swapToTailRemove(i);
			i--;
		}
	}
}

int DropController::spawnDrop(const Vec2& position, float spawnTime)
{
	cVector<float> bonusChances;
	bonusChances.resize(bloodworks->getBonuses().size());
	cVector<float> gunChances;
	gunChances.resize(bloodworks->getGuns().size());
	float totalChance = 0.0f;
	for (int i = 0; i < bloodworks->getBonuses().size(); i++)
	{
		bonusChances[i] = bloodworks->getBonuses()[i]->getSpawnChance();
		totalChance += bonusChances[i];
	}
	float bonusChance = totalChance;
	for (int i = 0; i < bloodworks->getGuns().size(); i++)
	{
		gunChances[i] = bloodworks->getGuns()[i]->getSpawnChance();
		totalChance += gunChances[i];
	}
	float r = randFloat() * totalChance;

	if (r < bonusChance)
	{
		for (int i = 0; i < bloodworks->getBonuses().size(); i++)
		{
			r -= bonusChances[i];
			if (r < 0.00001f)
			{
				return spawnBonus(position, i, spawnTime);
			}
		}
		return spawnBonus(position, randInt(bloodworks->getBonuses().size()), spawnTime);
	}
	else
	{
		r -= bonusChance;

		for (int i = 0; i < bloodworks->getGuns().size(); i++)
		{
			r -= gunChances[i];
			if (r < 0.00001f)
			{
				return spawnGun(position, i, spawnTime);
			}
		}
		return spawnGun(position, randInt(bloodworks->getGuns().size()), spawnTime);
	}
}

void DropController::reset()
{
	for (auto& drop : drops)
	{
		SAFE_DELETE(drop.renderable);
		SAFE_DELETE(drop.text);
	}
	drops.clear();
	lastRandomDropSpawn = lastDropSpawn = timer.getTime();
}

void DropController::onMonsterDied(Monster* monster, float dropChance)
{
	if (bloodworks->getMissionController()->getMissionData()["dontSpawnBonus"])
	{
		return;
	}

	Vec2 screenMin = bloodworks->getCameraPos() - bloodworks->getScreenDimensions().toVec() * 0.5f * bloodworks->getCameraZoom() + 50;
	Vec2 screenMax = bloodworks->getCameraPos() + bloodworks->getScreenDimensions().toVec() * 0.5f * bloodworks->getCameraZoom() - 50;

	const Vec2& pos = monster->getPosition();

	//if (pos.x > screenMin.x && pos.y > screenMin.y && pos.x < screenMax.x && pos.y < screenMax.y)
	{
		float timeSinceLastDrop = timer.getTime() - lastDropSpawn;

		if (timeSinceLastDrop < 5.0f)
		{
			return;
		}

		float extraDropChance = (timeSinceLastDrop - 10.0f) / 20.0f;
		float r = randFloat();
		if (r < dropChance + extraDropChance)
		{
			lastDropSpawn = timer.getTime();			
			if (!bloodworks->IsGUIHidden())
			{
				spawnDrop(pos);
			}
		}
	}
}

float DropController::getLastSpawnTime() const
{
	return max(lastRandomDropSpawn, lastDropSpawn);
}

void DropController::clearButHighlighted()
{
	Vec2 playerPos = bloodworks->getPlayer()->getPosition();
	Vec2 crosshairPos = playerPos + bloodworks->getPlayer()->getCrosshairPos();
	for (auto& drop : drops)
	{
		if (drop.pos.distanceSquared(crosshairPos) > 30.0f * 30.0f)
		{
			drop.time -= 100.0f;
		}
	}
}

void DropController::removeDrop(int id)
{
	for (auto& drop : drops)
	{
		if (drop.id == id)
		{
			drop.spawnTime = 0.0f;
		}
	}
}

