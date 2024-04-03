#include <Geode/modify/GJGarageLayer.hpp>
#include "layers/SogLayer.h"

using namespace geode::prelude;

class SoggyStuff : public CCMenu {
protected:
	CCSprite* m_sogSpr = nullptr;
	CCMenuItemSpriteExtra* m_sogBtn = nullptr;
public:
	static SoggyStuff* create() {
		auto ret = new SoggyStuff();
		if(ret && ret->init()) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return ret;
	}
	bool init() {
		if(!CCMenu::init())
			return false;

		auto director = CCDirector::sharedDirector();
    	auto winSize = director->getWinSize();

		m_sogSpr = CCSprite::createWithSpriteFrameName("GJ_soggyBtn_001.png"_spr);
		m_sogBtn = CCMenuItemSpriteExtra::create(m_sogSpr, this, menu_selector(SoggyStuff::onSog));

		m_sogBtn->m_animationType = MenuAnimationType::Move;
		m_sogBtn->m_startPosition = m_sogSpr->getPosition();
		m_sogBtn->m_destPosition = ccp(0, -15.f);
		m_sogBtn->setPositionY(-2);
		this->addChild(m_sogBtn);

		scheduleUpdate();

		return true;
	}
	virtual void update(float dt) {
		m_sogBtn->setVisible(true);
		m_sogSpr->setVisible(true);
		setVisible(true);
	}
	void onSog(CCObject*) {
		CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInT::create(0.5f, SogLayer::scene()));
	}
};

class $modify(GJGarageLayerExt, GJGarageLayer) {
	bool init() {
		if(!GJGarageLayer::init())
			return false;
		auto director = CCDirector::sharedDirector();
    	auto winSize = director->getWinSize();

		auto soggyStuff = SoggyStuff::create();

		soggyStuff->setPosition(ccp((winSize.width / 2) + 141, director->getScreenTop() - 23));

		this->addChild(soggyStuff);

		return true;
	}
};
