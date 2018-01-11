#include "LevelUpPopup.h"
#include "Bloodworks.h"
#include "cRenderable.h"
#include "cTexturedQuadRenderable.h"
#include "cTextRenderable.h"
#include "cFont.h"
#include "cTexture.h"
#include "cButton.h"
#include "Player.h"
#include "Perk.h"
#include "MissionController.h"
#include "Bloodworks.h"
#include "BloodworksControls.h"
#include <sstream>

LevelUpPopup::LevelUpPopup(Bloodworks *bloodworks)
{
	this->bloodworks = bloodworks;
	levelupGroup = new cRenderableContainer(bloodworks);
	levelupGroup->setAlignment(RenderableAlignment::center);

	cTexturedQuadRenderable *t = new cTexturedQuadRenderable(bloodworks, "resources/level_up_bg.png", "resources/default");
	t->setWorldMatrix(Mat3::scaleMatrix(t->getTexture()->getDimensions().toVec() * 0.55f));
	levelupGroup->addRenderable(t);

	levelupGroupTitle = new cTextRenderable(bloodworks, resources.getFont("resources/fontData.txt"), "", 24.0f);
	levelupGroupTitle->setWorldMatrix(Mat3::translationMatrix(Vec2(0.0f, 110.0f)));
	levelupGroupTitle->setTextAlignment(TextAlignment::center);
	levelupGroup->addRenderable(levelupGroupTitle);

	levelupGroupSubTitle = new cTextRenderable(bloodworks, resources.getFont("resources/fontData.txt"), "", 18.0f);
	levelupGroupSubTitle->setWorldMatrix(Mat3::translationMatrix(Vec2(0.0f, 83.0f)));
	levelupGroupSubTitle->setTextAlignment(TextAlignment::center);
	levelupGroup->addRenderable(levelupGroupSubTitle);

	currentPerkName = new cTextRenderable(bloodworks, resources.getFont("resources/fontData.txt"), "", 18.0f);
	currentPerkName->setWorldMatrix(Mat3::translationMatrix(Vec2(0.0f, -80.0f)));
	currentPerkName->setTextAlignment(TextAlignment::center);
	levelupGroup->addRenderable(currentPerkName);

	currentPerkExplanation = new cTextRenderable(bloodworks, resources.getFont("resources/fontData.txt"), "", 14.0f, Vec4::fromColor(0xFFAAAAAA));
	currentPerkExplanation->setWorldMatrix(Mat3::translationMatrix(Vec2(0.0f, -117.0f)));
	currentPerkExplanation->setTextAlignment(TextAlignment::center);
	currentPerkExplanation->setVerticalTextAlignment(VerticalTextAlignment::mid);
	currentPerkExplanation->setMaxLength(500.0f);
	levelupGroup->addRenderable(currentPerkExplanation);

	levelUpText = new cTextRenderable(bloodworks, resources.getFont("resources/fontData.txt"), "Tab to Level Up", 24.0f);
	levelUpText->setWorldMatrix(Mat3::translationMatrix(Vec2(145.0f, 40.0f)));
	levelUpText->setTextAlignment(TextAlignment::center);
	levelUpText->setVerticalTextAlignment(VerticalTextAlignment::mid);
	levelUpText->setAlignment(RenderableAlignment::bottomLeft);
	bloodworks->addRenderable(levelUpText, GUI + 151);
	levelUpText->setVisible(false);
	levelupGroup->setVisible(false);

	bloodworks->addRenderable(levelupGroup, GUI + 150);

	levelUpSound = resources.getSoundSample("resources/sounds/level_up.ogg");
	waitingLevels = 0;
	levelUpShowTime = -1.0f;
}

LevelUpPopup::~LevelUpPopup()
{
	SAFE_DELETE(levelupGroup);
	SAFE_DELETE(levelUpText);
	for (auto& t : levelupPerksRenderables)
	{
		SAFE_DELETE(t);
	}
	levelupPerksRenderables.clear();
	levelUpSound = nullptr;
}

bool LevelUpPopup::isVisible() const
{
	return levelupGroup->isVisible();
}

void LevelUpPopup::show(bool setAlpha)
{
	levelUpSound->play();
	lastMouseMoveTimer = 0.01f;
	input.showMouse();
	if (setAlpha)
	{
		input.setMousePosition(bloodworks->getScreenDimensions().w / 2, bloodworks->getScreenDimensions().h / 2 + 70);
	}
	bloodworks->doPause();
	levelupGroup->setVisible(true);
	std::stringstream ss;

	ss << "You are level " << bloodworks->getPlayer()->getLevel() << "!";
	levelupGroupTitle->setText(ss.str());

	std::stringstream ss2;

	if (waitingLevels >= 2)
	{
		ss2 << "You have " << waitingLevels << " perks to select.";
	}
	levelupGroupSubTitle->setText(ss2.str());

	currentPerkName->setText("");
	currentPerkExplanation->setText("");
	hoverLevelupPerkIndex = -1;

	levelupGroup->setColor(Vec4(1.0f, 1.0f, 1.0f, setAlpha ? 0.0f : 1.0f));
	levelUpText->setVisible(false);
	if (levelupPerksRenderables.size())
	{
		for (auto& t : levelupPerksRenderables)
		{
			t->setVisible(true);
		}
		return;
	}

	cVector<Perk*> availablePerks;
	
	auto& missionScript = lua[bloodworks->getMissionController()->getCurrentMissionScript()];
	if (missionScript["getPerksForLevelUp"])
	{
		sol::table table = missionScript["getPerksForLevelUp"]();

		for (auto t : table)
		{
			std::string script = t.second.as<std::string>();
			
			for (auto p : bloodworks->getPerks())
			{
				if (p->getScriptName() == script)
				{
					availablePerks.push_back(p);
					break;
				}
			}
		}
	}
	else
	{
		availablePerks = bloodworks->getAvailablePerks();
	}

	auto& missionData = bloodworks->getMissionController()->getMissionData();
	int selectCount = missionData["perkPerLevel"] ? missionData["perkPerLevel"].get<int>() : 3;

	selectCount = min(selectCount, availablePerks.size());
	levelupPerks.clear();
	while (selectCount-- > 0)
	{
		int r = randInt(availablePerks.size());
		levelupPerks.push_back(availablePerks[r]);
		availablePerks.swapToTailRemove(r);
	}
	assert(levelupPerksRenderables.size() == 0);

	float perkDistance = levelupPerks.size() < 5 ? 140.0f : 110.0f;
	for (int i = 0; i < levelupPerks.size(); i++)
	{
		cButton *t = new cButton(bloodworks);
		Vec2 pos = Vec2(-i * perkDistance + (levelupPerks.size() - 1) * perkDistance * 0.5f, 20.0f);
		t->setDefaultMatrix(pos, 1.0f, 0.0f);
		t->setHoverMatrix(pos, 1.25f, 0.0f);
		t->setAlignment(RenderableAlignment::center);
		t->setColor(Vec4(1.0f, 1.0f, 1.0f, 0.0f));
		t->setHitArea(Vec2(-50.0f), Vec2(50.0f));
		t->setHoverSpeed(10.0f);
		t->setSounds(resources.getSoundSample("resources/sounds/click.ogg"), resources.getSoundSample("resources/sounds/hover.ogg"));

		std::string iconPath = levelupPerks[i]->getIconPath();
		bool addText = false;
		if (iconPath == "")
		{
			iconPath = "resources/perks/default_icon.png";
			addText = true;
		}
		cTexturedQuadRenderable *quad = new cTexturedQuadRenderable(bloodworks, iconPath, "resources/default");
		quad->setWorldMatrix(Mat3::scaleMatrix(40.0f));
		t->addRenderable(quad);

		if (addText)
		{
			cTextRenderable *text = new cTextRenderable(bloodworks, resources.getFont("resources/fontData.txt"), levelupPerks[i]->getName(), 12.0f);
			text->setWorldMatrix(Mat3::translationMatrix(0.0f, 8.0f));
			text->setTextAlignment(TextAlignment::center);
			text->setVerticalTextAlignment(VerticalTextAlignment::mid);
			text->setMaxLength(70.0f);
			text->setMaxLineCount(6);
			text->setLinePadding(0.0f);
			t->addRenderable(text);
		}

		bloodworks->addRenderable(t, GUI + 151);
		levelupPerksRenderables.push_back(t);
	}
}

void LevelUpPopup::tick()
{
	LastEntrySet S("LevelUpPopup::tick");
	if (bloodworks->isFirstTick())
	{
		out << "LevelUpPopup::tick\n";
	}
	if (levelUpShowTime > 0.0f)
	{
		levelUpShowTime -= timer.getNonSlowedDt() * 3.9f;
		if (levelUpShowTime < 0.0f)
		{
			levelUpShowTime = 0.0f;
		}
		float t = 0.6f - levelUpShowTime;
		t = min(t, 0.0f);
		levelUpText->setTextSize(levelUpShowTime * 5.0f + t * 3.0f + 24.0f);
	}
	
	if (!isVisible())
	{
		return;
	}
	float alpha = levelupGroup->getColor().a;
	if (alpha < 1.0f)
	{
		alpha += timer.getNonSlowedDt() * 4.0f;
		alpha = min(1.0f, alpha);
		Vec4 color = Vec4(1.0f, 1.0f, 1.0f, alpha);
		levelupGroup->setColor(color);
		for (int i = 0; i < levelupPerks.size(); i++)
		{
			levelupPerksRenderables[i]->setColor(color);
		}
	}

	if (input.getDeltaMousePos().lengthSquared() > 0.0f)
	{
		lastMouseMoveTimer = 1.0f;
		for (int i = 0; i < levelupPerks.size(); i++)
		{
			levelupPerksRenderables[i]->setEnforcedHovering(cButton::no_enforce);
		}
	}

	lastMouseMoveTimer -= timer.getNonSlowedDt();

	if (lastMouseMoveTimer < 0.0f && hoverLevelupPerkIndex == -1)
	{
		joyPadFree = false;
		for (int i = 0; i < levelupPerks.size(); i++)
		{
			levelupPerksRenderables[i]->setEnforcedHovering(1 == levelupPerks.size() / 2 ? cButton::enforce_hovering : cButton::enforce_not_hovering);
		}
	}

	if (joyPadFree)
	{
		int indexToSet = hoverLevelupPerkIndex;
		if (input.getJoystickAxisPos().x > 0.5f || mapper.isKeyPressed(GameKey::Right))
		{
			indexToSet--;
			if (indexToSet < 0)
			{
				indexToSet += levelupPerks.size();
			}
			joyPadFree = false;
		}
		else if (input.getJoystickAxisPos().x < -0.5f || mapper.isKeyPressed(GameKey::Left))
		{
			indexToSet++;
			if (indexToSet >= levelupPerks.size())
			{
				indexToSet -= levelupPerks.size();
			}
			joyPadFree = false;
		}

		if (joyPadFree == false)
		{
			for (int i = 0; i < levelupPerks.size(); i++)
			{
				levelupPerksRenderables[i]->setEnforcedHovering(i == indexToSet ? cButton::enforce_hovering : cButton::enforce_not_hovering);
			}
		}
	}
	else
	{
		if (fabs(input.getJoystickAxisPos().x) < 0.4f && mapper.isKeyUp(GameKey::Right) && mapper.isKeyUp(GameKey::Left))
		{
			joyPadFree = true;
		}
	}
	
	//debugRenderer.addText("O", mouseScreenPos.x, mouseScreenPos.y, 0.0f, Vec4(1.0f), 12.0f, TextAlignment::center, RenderableAlignment::center);
	for (int i = 0; i < levelupPerks.size(); i++)
	{
		levelupPerksRenderables[i]->check(input.getMousePos());
		if (hoverLevelupPerkIndex != i)
		{
			if (levelupPerksRenderables[i]->isHovering())
			{
				hoverLevelupPerkIndex = i;
				std::stringstream name;
				name << levelupPerks[i]->getName();
				if (levelupPerks[i]->isLevelVisible())
				{
					name << " (Level : " << (levelupPerks[i]->getLevel() + 1) << ")";
				}
				currentPerkName->setText(name.str());
				currentPerkExplanation->setText(levelupPerks[i]->getDescription());
				hoverLevelupPerkIndex = i;
			}
		}
		else if (hoverLevelupPerkIndex == i)
		{
			if (!levelupPerksRenderables[i]->isHovering())
			{
				hoverLevelupPerkIndex = -1;
				currentPerkName->setText("");
				currentPerkExplanation->setText("");
			}
		}

		if (levelupPerksRenderables[i]->isClicked() || mapper.isKeyPressed(GameKey::Select))
		{
			levelupPerks[i]->takeLevel();
			bloodworks->onPerkUsed(levelupPerks[i]);
			bloodworks->doUnpause();
			levelupGroup->setVisible(false);
			clearPerks();
			waitingLevels--;
			if (waitingLevels > 0)
			{
				show(false);
			}
			else
			{
				input.hideMouse();
			}
			return;
		}
	}


	if (mapper.isKeyPressed(GameKey::Back) || (input.isKeyPressed(mouse_button_left) && AARect(Vec2(-380, -200), Vec2(380, 200)).isOutside(game->getRelativeMousePos(input.getMousePos(), RenderableAlignment::center))))
	{
		mapper.clearKeyPress(GameKey::Back);
		input.clearKeyPress(mouse_button_left);

		input.hideMouse();
		bloodworks->doUnpause();
		levelupGroup->setVisible(false);
		for (auto& t : levelupPerksRenderables)
		{
			t->setVisible(false);
		}
		showLevelUpText();
	}
}

void LevelUpPopup::reset()
{
	waitingLevels = 0;
	levelupGroup->setVisible(false);
	levelUpText->setVisible(false);
	clearPerks();
}

void LevelUpPopup::clearPerks()
{
	levelupPerks.clear();
	for (auto& t : levelupPerksRenderables)
	{
		SAFE_DELETE(t);
	}
	levelupPerksRenderables.clear();
}

void LevelUpPopup::showLevelUpText()
{
	if (bloodworks->IsGUIHidden() == false)
	{
		levelUpSound->play();
	}
	levelUpText->setVisible(true);
	levelUpShowTime = 1.0f;
}
