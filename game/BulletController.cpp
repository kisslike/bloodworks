#include "BulletController.h"
#include "Bullet.h"

#include "cGlobals.h"
#include "lua.h"
#include "Gun.h"
#include "Bloodworks.h"

void BulletController::init(Bloodworks *bloodworks)
{
	this->bloodworks = bloodworks;

	grid.init(bloodworks->getMapMin() - 100.0f, bloodworks->getMapSize() + 200.0f, Vec2(50.0f));
}

void BulletController::clear()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		SAFE_DELETE(bullets[i]);
	}
	bulletMap.clear();

}

BulletController::~BulletController()
{
	clear();
}

void BulletController::addBullet(Bullet* bullet)
{
	bullets.push_back(bullet);
	bulletMap[bullet->getId()] = bullet;
	grid.insertToGrid(bullet);
}

void BulletController::tick()
{
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->isDead == false)
		{
			bullets[i]->tick();
		}

		if (bullets[i]->isDead)
		{
			if (bullets[i]->hasParticles() == false)
			{
				grid.removeFromGrid(bullets[i]);
				int id = bullets[i]->getId();
				SAFE_DELETE(bullets[i]);
				bulletMap.erase(id);
				bullets[i] = bullets[bullets.size() - 1];
				bullets.resize(bullets.size() - 1);
				i--;
			}
		}
		else
		{
			grid.relocate(bullets[i]);
		}
	}

	if (input.isKeyDown(key_f2))
	{
		grid.drawDebug();
	}
}

