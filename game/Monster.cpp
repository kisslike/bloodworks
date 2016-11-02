#include "Monster.h"
#include "Bloodworks.h"
#include "cAnimatedRenderable.h"

Monster::Monster(Bloodworks *bloodworks)
{
	this->bloodworks = bloodworks;

	renderable = new cAnimatedTexturedQuadRenderable(bloodworks, "resources/default");
	renderable->addAnimation("stand", true)
		.addFrame("resources/monster/0.png", 0.5f);

	renderable->addAnimation("walk", true)
		.addFrame("resources/monster/0.png", 0.1f)
		.addFrame("resources/monster/1.png", 0.1f)
		.addFrame("resources/monster/2.png", 0.1f)
		.addFrame("resources/monster/3.png", 0.1f)
		.addFrame("resources/monster/0.png", 0.1f)
		.addFrame("resources/monster/4.png", 0.1f)
		.addFrame("resources/monster/5.png", 0.1f)
		.addFrame("resources/monster/6.png", 0.1f);
	

	renderable->addAnimation("attack")
		.addFrame("resources/monster/7.png", 0.1f)
		.addFrame("resources/monster/8.png", 0.1f)
		.addFrame("resources/monster/9.png", 0.1f)
		.addFrame("resources/monster/10.png", 0.1f)
		.addFrame("resources/monster/11.png", 0.1f);

	renderable->setSize(Vec2(24.0f, 34.0f) * 0.6f);
	renderable->playAnimation("walk");
	bloodworks->addRenderable(renderable);
}

void Monster::tick(float dt)
{
	if (input.isKeyPressed(key_1))
	{
		renderable->playAnimation("stand");
	}
	if (input.isKeyPressed(key_2))
	{
		renderable->playAnimation("walk");
	}
	if (input.isKeyPressed(key_3))
	{
		renderable->playAnimation("attack", 0.0f, 1);
	}

}