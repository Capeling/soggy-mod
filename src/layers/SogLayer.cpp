#include "SogLayer.hpp"

using namespace geode::prelude;

SogLayer* SogLayer::create() {
    auto ret = new SogLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
};

CCScene* SogLayer::scene() {
    auto layer = SogLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

bool SogLayer::init() {
    if(!CCLayer::init())
        return false;
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();
    
    m_background = CCSprite::createWithSpriteFrameName("GJ_sog_001.png"_spr);
    m_background->setAnchorPoint({ 0.f, 0.f });
    addChild(m_background, -2);

    m_background->setScaleX((winSize.width + 10.f) / m_background->getTextureRect().size.width);
    m_background->setScaleY((winSize.height + 10.f) / m_background->getTextureRect().size.height);
    m_background->setPosition(ccp(-5, -5));

    CCSprite* backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    CCMenuItemSpriteExtra* backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(SogLayer::onClose));

    CCMenu* backMenu = CCMenu::create();
    backMenu->addChild(backBtn);
    addChild(backMenu, 1);

    backMenu->setPosition(ccp(director->getScreenLeft() + 25.f, director->getScreenTop() - 22.f));
    setKeyboardEnabled(true);
    setKeypadEnabled(true);
    GameManager::sharedState()->fadeInMusic("SogLoop.mp3"_spr);
    //FMODAudioEngine::sharedEngine()->playMusic("SogLoop.mp3"_spr, true, 0.0f, 0);
    return true;
}

void SogLayer::keyBackClicked() {
    SogLayer::onClose(nullptr);
}

void SogLayer::onClose(CCObject*) {

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    auto scene = director->getRunningScene();
    auto garage = GJGarageLayer::scene();

    CCDirector::get()->replaceScene(garage);

    this->retain();
    this->removeFromParentAndCleanup(false);
    garage->addChild(this, 1000);

    auto moveTo = CCMoveTo::create(0.3f, ccp(0, winSize.height));
    auto easeIn = CCEaseIn::create(moveTo, 2.0f);
    auto callFunc = CCCallFunc::create(this, callfunc_selector(SogLayer::removeFromParent));

    auto ccSeq = CCSequence::create(easeIn, callFunc, 0);
    this->runAction(ccSeq);
    GameManager::sharedState()->fadeInMenuMusic();

}