#include <Geode/modify/GJGarageLayer.hpp>
#include "layers/SogLayer.hpp"

using namespace geode::prelude;

class $modify(GJGarageLayerExt, GJGarageLayer) {
	bool init() {
		if(!GJGarageLayer::init())
			return false;
		auto director = CCDirector::sharedDirector();
    	auto winSize = director->getWinSize();

		auto sogSpr = CCSprite::createWithSpriteFrameName("GJ_soggyBtn_001.png"_spr);
		auto sogBtn = CCMenuItemSpriteExtra::create(sogSpr, this, menu_selector(GJGarageLayerExt::onSog));
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
		void onSog(CCObject*) {
		CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInT::create(0.5f, SogLayer::scene()));
	}
};
