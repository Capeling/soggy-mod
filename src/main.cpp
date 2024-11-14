#include <Geode/Geode.hpp>
#include <Geode/ui/Layout.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "layers/SogLayer.hpp"

using namespace geode::prelude;

struct GJGarageLayerHook : Modify<GJGarageLayerHook, GJGarageLayer> {
	bool init() {
		if(!GJGarageLayer::init())
			return false;
		auto director = CCDirector::sharedDirector();
    	auto winSize = director->getWinSize();

		auto sogSpr = CCSprite::createWithSpriteFrameName("GJ_soggyBtn_001.png"_spr);
		auto sogBtn = CCMenuItemSpriteExtra::create(sogSpr, this, menu_selector(GJGarageLayerHook::onSoggy));
		auto sogMenu = CCMenu::create();

		sogBtn->m_animationType = MenuAnimationType::Move;
		sogBtn->m_startPosition = sogSpr->getPosition();
		sogBtn->m_offset = ccp(0, -15.f);
		sogBtn->setPositionY(-2);
		sogMenu->addChild(sogBtn);

		sogBtn->setID("soggy-button"_spr);
		sogMenu->setID("soggy-menu"_spr);

		sogMenu->setPosition(ccp((winSize.width / 2) + 141, director->getScreenTop() - 23));

		this->addChild(sogMenu);

		return true;
	}
		void onSoggy(CCObject*) {
		CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInT::create(0.5f, SogLayer::scene(false)));
	}
};

struct LevelInfoLayerHook : Modify<LevelInfoLayerHook, LevelInfoLayer> {
	bool init(GJGameLevel* level, bool challenge) {
		if(!LevelInfoLayer::init(level, challenge)) {
			return false;
		}

		auto sogSpr = CCSprite::createWithSpriteFrameName("GJ_soggyRope_001.png"_spr);
		auto sogBtn = CCMenuItemSpriteExtra::create(sogSpr, this, menu_selector(LevelInfoLayerHook::onSoggy));
		
		auto garageMenu = static_cast<CCMenu*>(getChildByID("garage-menu"));
		if(!garageMenu) {
			log::error("LevelInfoLayer 'garage-menu' does not exist, returning.");
			return true;
		}

		garageMenu->setLayout(AxisLayout::create());
		garageMenu->setPositionX(garageMenu->getPositionX() - 25);

		sogBtn->m_animationType = MenuAnimationType::Move;
		sogBtn->m_startPosition = sogSpr->getPosition();
		sogBtn->m_duration = 0.2f;
		sogBtn->m_unselectedDuration = 0.2f;
		sogBtn->m_offset = ccp(0, -8.f);
		sogBtn->setID("soggy-button"_spr);

		garageMenu->addChild(sogBtn);
		garageMenu->updateLayout();

		return true;
	}
	void onSoggy(CCObject*) {
		CCDirector::sharedDirector()->pushScene(CCTransitionMoveInT::create(0.5f, SogLayer::scene(true)));
	}
};