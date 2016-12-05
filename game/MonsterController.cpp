#include "MonsterController.h"
#include "Bloodworks.h"
#include "Monster.h"
#include "sol.h"


MonsterController::MonsterController(Bloodworks *bloodworks)
{
	this->bloodworks = bloodworks;

	grid.init(bloodworks->getMapMin() - 50.0f, bloodworks->getMapSize() + 100.0f, Vec2(50.0f));

	lua["mission"] = lua.create_table();

	monsterTemplates["monster"] = new MonsterTemplate("resources/monster/data.json");
}

void MonsterController::tick()
{
	for (int i = 0; i < monsters.size(); i++)
	{
		if (monsters[i]->isDead)
		{
			grid.removeFromGrid(monsters[i]);
			monstersMap.erase(monsters[i]->getId());
			SAFE_DELETE(monsters[i]);
			monsters[i] = monsters[monsters.size() - 1];
			monsters.resize(monsters.size() - 1);
			i--;
		}
	}

	for (auto& monster : monsters)
	{
		monster->tick();
		if (bloodworks->isCoorOutside(monster->position) == false)
		{
			grid.relocate(monster);
		}
	}

	if (input.isKeyDown(key_f1))
	{
		grid.drawDebug();
	}
}

MonsterController::~MonsterController()
{
	for (auto& monster : monsters)
	{
		grid.removeFromGrid(monster);
		SAFE_DELETE(monster);
	}

	for (auto& monsterTemplate : monsterTemplates)
	{
		auto& m = monsterTemplate.second;
		SAFE_DELETE(m);
	}
	monsterTemplates.clear();
}

const std::vector<Monster*>& MonsterController::getMonsterAt(const Vec2& pos)  const
{
	return grid.getNodeAtPos(pos);
}

Monster* MonsterController::getClosestMonster(const Vec2& pos)
{
	return getClosestMonsterWithIgnoreData(pos, "");
}


Monster* MonsterController::getClosestMonsterInRange(const Vec2& pos, float range)
{
	return getClosestMonsterInRangeWithIgnoreData(pos, range, "");
}

Monster* MonsterController::getClosestMonsterInRangeWithIgnoreData(const Vec2& pos, float range, const std::string& ignoreData)
{
	float closest = range * range;
	Monster *closestMonster = nullptr;

	if (range > 200)
	{
		Vec2 min = pos - range;
		Vec2 max = pos + range;

		IntVec2 minIndex = grid.getNodeIndex(min);
		IntVec2 maxIndex = grid.getNodeIndex(max);

		if (minIndex.x < 0)
		{
			minIndex.x = 0;
		}
		if (minIndex.y < 0)
		{
			minIndex.y = 0;
		}

		if (maxIndex.x > grid.getNodeCount().x - 1)
		{
			maxIndex.x = grid.getNodeCount().x - 1;
		}
		if (maxIndex.y < grid.getNodeCount().y - 1)
		{
			maxIndex.y = grid.getNodeCount().y - 1;
		}

		for (int i = minIndex.x; i <= maxIndex.x; i++)
		{
			for (int j = minIndex.y; j <= maxIndex.y; j++)
			{
				for (auto& monster : grid.getNodeAtIndex(i, j))
				{
					float d;
					if (monster->isDead == false
						&& (i == minIndex.x || i == monster->gridStart.x) && (j == minIndex.y || j == monster->gridStart.y)
						&& (d = monster->getPosition().distanceSquared(pos)) < closest
						&& (ignoreData.length() == 0 || ((bool)monster->data[ignoreData]) == false))
					{
						closest = d;
						closestMonster = monster;
					}
				}
			}
		}
	}
	else
	{
		for (auto& monster : monsters)
		{
			float d;
			if (monster->isDead == false && closest > (d = monster->getPosition().distanceSquared(pos)) && (ignoreData.length() == 0 || ((bool)monster->data[ignoreData]) == false))
			{
				closest = d;
				closestMonster = monster;
			}
		}
	}

	return closestMonster;
}

std::vector<Monster*> MonsterController::getAllMonstersInRange(const Vec2& pos, float range)
{
	float maxDistanceSquared = range * range;

	std::vector<Monster*> foundMonsters;

	if (range > 200)
	{
		Vec2 min = pos - range;
		Vec2 max = pos + range;

		IntVec2 minIndex = grid.getNodeIndex(min);
		IntVec2 maxIndex = grid.getNodeIndex(max);

		if (minIndex.x < 0)
		{
			minIndex.x = 0;
		}
		if (minIndex.y < 0)
		{
			minIndex.y = 0;
		}

		if (maxIndex.x > grid.getNodeCount().x - 1)
		{
			maxIndex.x = grid.getNodeCount().x - 1;
		}
		if (maxIndex.y < grid.getNodeCount().y - 1)
		{
			maxIndex.y = grid.getNodeCount().y - 1;
		}

		for (int i = minIndex.x; i <= maxIndex.x; i++)
		{
			for (int j = minIndex.y; j <= maxIndex.y; j++)
			{
				IntVec2 index(i, j);
				for (auto& monster : grid.getNodeAtIndex(i, j))
				{
					if (monster->isDead == false 
						&& (i == minIndex.x || i == monster->gridStart.x) && (j == minIndex.y || j == monster->gridStart.y)
						&& monster->getPosition().distanceSquared(pos) < maxDistanceSquared)
					{
						foundMonsters.push_back(monster);
					}
				}
			}
		}

	}
	else
	{
		for (auto& monster : monsters)
		{
			if (monster->getPosition().distanceSquared(pos) < maxDistanceSquared)
			{
				foundMonsters.push_back(monster);
			}
		}
	}

	return foundMonsters;
}

void MonsterController::damageMonstersInRangeWithIgnoreData(const Vec2& pos, float range, int minRange, int maxRange, bool mark, const std::string& ignoreData)
{
	std::vector<Monster*> monsters = getAllMonstersInRange(pos, range);
	for (auto& monster : monsters)
	{
		if (ignoreData.length() == 0|| ((bool)monster->data[ignoreData]) == false)
		{
			monster->doDamage(randInt(minRange, maxRange), (monster->position - pos).normalized());
			if (mark)
			{
				monster->data[ignoreData] = true;
			}
		}
	}
}

Monster* MonsterController::addMonster(const std::string& monsterTemplateName)
{
	MonsterTemplate *monsterTemplate = monsterTemplates[monsterTemplateName];

	Monster *newMonster = new Monster(bloodworks);
	monstersMap[newMonster->id] = newMonster;
	monsters.push_back(newMonster);
	newMonster->init(monsterTemplate);
	grid.insertToGrid(newMonster);
	return newMonster;
}

int MonsterController::getMonsterCount() const
{
	return (int)monsters.size();
}

Monster* MonsterController::getMonster(int id) const
{
	auto& element = monstersMap.find(id);
	return element == monstersMap.end() ? nullptr : element->second;
}

void MonsterController::damageMonstersInRange(const Vec2& pos, float range, int minRange, int maxRange)
{
	damageMonstersInRangeWithIgnoreData(pos, range, minRange, maxRange, false, "");
}

Monster* MonsterController::getClosestMonsterWithIgnoreData(const Vec2& pos, const std::string& ignoreData)
{
	float closest = FLT_MAX;
	Monster *closestMonster = nullptr;
	for (auto& monster : monsters)
	{
		float d;
		if (monster->isDead == false && closest > (d = monster->getPosition().distanceSquared(pos)) && (ignoreData.length() == 0 || ((bool)monster->data[ignoreData]) == false))
		{
			closest = d;
			closestMonster = monster;
		}
	}

	return closestMonster;
}
