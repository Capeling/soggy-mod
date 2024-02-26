#include <Geode/modify/GJGarageLayer.hpp>
#include "layers/SogLayer.h"

using namespace geode::prelude;

class $modify(GJGarageLayerExt, GJGarageLayer) {
	bool init() {
		if(!GJGarageLayer::init())
			return false;
		auto director = CCDirector::sharedDirector();
    	auto winSize = director->getWinSize();
		auto topMenu = this->getChildByID("top-left-menu");

		auto sogSpr = CCSprite::createWithSpriteFrameName("GJ_soggyBtn_001.png"_spr);
		auto sogBtn = CCMenuItemSpriteExtra::create(sogSpr, this, menu_selector(GJGarageLayerExt::onSog));
		
		auto sogMenu = CCMenu::create();
		sogMenu->setPosition(ccp((winSize.width / 2) + 141, topMenu->getPositionY()));
		sogMenu->addChild(sogBtn);

		sogBtn->m_animationType = MenuAnimationType::Move;
		sogBtn->m_startPosition = sogSpr->getPosition();
		sogBtn->m_destPosition = ccp(0, -15.f);
		this->addChild(sogMenu);
		return true;
	}
	void onSog(CCObject*) {
		CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInT::create(0.5f, SogLayer::scene()));
	}
};