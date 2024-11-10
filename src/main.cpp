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

		sogMenu->setPosition(ccp((winSize.width / 2) + 141, director->getScreenTop() - 23));

		this->addChild(sogMenu);

		return true;
	}
		void onSoggy(CCObject*) {
		CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInT::create(0.5f, SogLayer::scene(false)));
	}
};

// struct LevelInfoLayerHook : Modify<LevelInfoLayerHook, LevelInfoLayer> {
// 	bool init(GJGameLevel* level, bool challenge) {
// 		if(!LevelInfoLayer::init(level, challenge)) {
// 			return false;
// 		}

// 		auto sogSpr = CCSprite::createWithSpriteFrameName("GJ_soggyRope_001.png"_spr);
// 		auto sogBtn = CCMenuItemSpriteExtra::create(sogSpr, this, menu_selector(LevelInfoLayerHook::onSoggy));
// 		auto sogMenu = CCMenu::create();

// 		sogBtn->m_animationType = MenuAnimationType::Move;
// 		sogBtn->m_startPosition = sogSpr->getPosition();
// 		sogBtn->m_duration = 0.2f;
// 		sogBtn->m_unselectedDuration = 0.2f;
// 		sogBtn->m_offset = ccp(0, -8.f);
// 		sogMenu->addChild(sogBtn);

// 		addChild(sogMenu, 100);

// 		return true;
// 	}
// 	void onSoggy(CCObject*) {
// 		CCDirector::sharedDirector()->pushScene(CCTransitionMoveInT::create(0.5f, SogLayer::scene(true)));
// 	}
// };