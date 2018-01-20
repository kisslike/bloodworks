#pragma once

#include <vector>
#include <unordered_map>
#include "cGrid.h"
#include "sol.h"

class Bloodworks;
class Bullet;

class BulletController
{
	Bloodworks *bloodworks;
	cVector<Bullet*> bullets;
	std::unordered_map<int, Bullet*> bulletMap;
	cGrid<Bullet> grid;
public:
	BulletController(Bloodworks *bloodworks);
	~BulletController();
	void addBullet(Bullet* bullet);
	void tick();
	Bullet *addCustomBullet(const sol::table& params);
	void reset();
	void relocateBullet(Bullet* bullet);
	void drawDebug();
	void updateGrid();
};