#include "SogLayer.hpp"

using namespace geode::prelude;

SogLayer* SogLayer::create(bool fromRope) {
    auto ret = new SogLayer();
    if (ret && ret->init(fromRope)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
};

CCScene* SogLayer::scene(bool fromRope) {
    auto layer = SogLayer::create(fromRope);
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

bool SogLayer::init(bool fromRope) {
    if(!CCLayer::init())
        return false;

    m_fromRope = fromRope;

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    std::string backgroundImage = "GJ_sog_001.png"_spr;
    std::string music = "SogLoop.mp3"_spr;

    auto rand = std::rand() % 100 + 1;
    if(rand == 46) {
        backgroundImage = "GJ_evilSog_001.png"_spr;
        music = "EvilSog.mp3"_spr;
        this->runAction(CCSequence::create(CCDelayTime::create(8.5f), CCCallFuncO::create(this, callfuncO_selector(SogLayer::onClose), nullptr), 0));
    }
    
    m_background = CCSprite::createWithSpriteFrameName(backgroundImage.c_str());
    m_background->setAnchorPoint({ 0.f, 0.f });
    addChild(m_background, -2);

    m_background->setScaleX((winSize.width + 10.f) / m_background->getTextureRect().size.width);
    m_background->setScaleY((winSize.height + 10.f) / m_background->getTextureRect().size.height);
    m_background->setPosition(ccp(-5, -5));
    m_background->setID("background");

    if(rand != 46) {
        CCSprite* backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        CCMenuItemSpriteExtra* backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(SogLayer::onClose));
        backBtn->setID("back-button");

        CCMenu* buttonMenu = CCMenu::create();
        buttonMenu->addChild(backBtn);
        addChild(buttonMenu, 1);
        buttonMenu->setID("button-menu");

        backBtn->setPosition(ccp(-winSize.width / 2 + 23.f, winSize.height / 2 - 25.f));

        auto honkSpr = CCSprite::createWithSpriteFrameName("block005b_05_001.png");
        honkSpr->setScale(3.f);
        honkSpr->setOpacity(0);

        auto honkBtn = CCMenuItemSpriteExtra::create(honkSpr, this, menu_selector(SogLayer::onHonk));
        honkBtn->setPosition(ccp(-50.f, 17.f));

        buttonMenu->addChild(honkBtn);

        setKeyboardEnabled(true);
        setKeypadEnabled(true);
    }

    GameManager::sharedState()->fadeInMusic(music);
    //FMODAudioEngine::sharedEngine()->playMusic("SogLoop.mp3"_spr, true, 0.0f, 0);
    return true;
}

void SogLayer::keyBackClicked() {
    SogLayer::onClose(nullptr);
}

void SogLayer::onHonk(CCObject*) {
    FMODAudioEngine::get()->playEffect("honk.wav"_spr);
}

void SogLayer::onClose(CCObject*) {
    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    this->retain();
    this->removeFromParentAndCleanup(false);

    if(!m_fromRope) {
        auto garage = GJGarageLayer::scene();

        director->replaceScene(garage);
        garage->addChild(this, 1000);
        this->release();
    } else {
        director->popScene();
        director->m_pNextScene->addChild(this, 1000);
    }


    auto moveTo = CCMoveTo::create(0.3f, ccp(0, winSize.height));
    auto easeIn = CCEaseIn::create(moveTo, 2.0f);
    auto callFunc = CCCallFunc::create(this, callfunc_selector(SogLayer::removeFromParent));

    auto ccSeq = CCSequence::create(easeIn, callFunc, 0);
    this->runAction(ccSeq);
    GameManager::sharedState()->fadeInMenuMusic();
    
    setKeyboardEnabled(false);
    setKeypadEnabled(false);
}