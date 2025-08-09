#include <layers/SogLayer.hpp>
#include <Events.hpp>

using namespace geode::prelude;

namespace soggy_mod {

SogLayer* SogLayer::create(bool fromRope) {
    auto ret = new SogLayer();
    if (ret->init(fromRope)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
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

    auto honkBtnPos = ccp(-50.f, -17.f);

    std::string backgroundImage = "SM_sog_001.png"_spr;
    std::string music = "SogLoop.mp3"_spr;

    auto rand = std::rand() % 100 + 1;
    if(rand == 46) {
        backgroundImage = "SM_evilSog_001.png"_spr;
        music = "EvilSog.mp3"_spr;
        this->runAction(CCSequence::create(CCDelayTime::create(8.5f), CCCallFuncO::create(this, callfuncO_selector(SogLayer::onClose), nullptr), 0));
    } else if (rand > 90) {
        backgroundImage = "SM_sog_02_001.png"_spr;
        music = "SogLoop02.mp3"_spr;
        honkBtnPos = ccp(34, 36);
    }

    m_background = CCSprite::create(backgroundImage.c_str());
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

        auto honkBtn = CCMenuItemExt::createSpriteExtra(honkSpr, [this](auto selector) {
            FMODAudioEngine::get()->playEffect("honk.wav"_spr, 1.0 + (m_honkCount * 0.01f), 0.0, 1.f);
            m_honkCount++;
            if (m_honkCount > 25)
                m_honkCount = 0;
        });
        honkBtn->setPosition(honkBtnPos);

        buttonMenu->addChild(honkBtn);

        setKeyboardEnabled(true);
        setKeypadEnabled(true);
    }

    GameManager::sharedState()->fadeInMusic(music);
    OnSogLayer(this, rand).post();
    return true;
}

void SogLayer::keyBackClicked() {
    SogLayer::onClose(nullptr);
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
    } else {
        director->popScene();
        director->m_pNextScene->addChild(this, 1000);
    }
    this->release();


    auto moveTo = CCMoveTo::create(0.3f, ccp(0, winSize.height));
    auto easeIn = CCEaseIn::create(moveTo, 2.0f);
    auto callFunc = CCCallFunc::create(this, callfunc_selector(SogLayer::removeFromParent));

    auto ccSeq = CCSequence::create(easeIn, callFunc, 0);
    this->runAction(ccSeq);
    GameManager::sharedState()->fadeInMenuMusic();

    setKeyboardEnabled(false);
    setKeypadEnabled(false);
}

}
