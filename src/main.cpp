#include <Geode/Geode.hpp>
#include <Geode/ui/Layout.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include "layers/SogLayer.hpp"

#include <Geode/modify/SecretLayer5.hpp>
#include <Geode/modify/RewardUnlockLayer.hpp>

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

struct TheWraithHook : Modify<TheWraithHook, SecretLayer5> {
	struct Fields {
        gd::string text = "";
    };

    void onSubmit(cocos2d::CCObject* sender) {
        this->m_fields->text = this->m_textInput->getString();
        std::transform(
            this->m_fields->text.begin(), 
            this->m_fields->text.end(), 
            this->m_fields->text.begin(), 
            [](unsigned char c){ return std::tolower(c); }
        );

        SecretLayer5::onSubmit(sender);
    }

    void showSuccessAnimationWrapper() {
        this->showSuccessAnimation();
    }

    void fadeInLoadingCircleWrapper() {
        this->m_circleSprite->fadeInCircle(false, 0.5, 0.0);
    }

    void showSoggyCat() {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        CCSprite* sprite = CCSprite::createWithSpriteFrameName("GJ_soggy_001.png"_spr);
        sprite->setPosition(ccp(winSize.width / 2, (winSize.height / 2) + 69));
        sprite->setScale(0.2f);

        CCNode* rewardPage = as<CCNode*>(cocos2d::CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(1));
        CCLayer* layer = as<CCLayer*>(rewardPage->getChildren()->objectAtIndex(0));
        layer->addChild(sprite);
    }

    void showCollectRewardWrapper() {
        GJRewardItem* item = GJRewardItem::create();
        RewardUnlockLayer* layer = RewardUnlockLayer::create(2, nullptr);

        // Put layer in scene
        cocos2d::CCDirector::sharedDirector()->getRunningScene()->addChild(layer);

        layer->showCollectReward(item);

        // Show soggycat after 1 second
        cocos2d::CCDelayTime* delay = cocos2d::CCDelayTime::create(2);
        cocos2d::CCCallFunc* call = cocos2d::CCCallFunc::create(this, callfunc_selector(TheWraithHook::showSoggyCat));

        cocos2d::CCSequence* sequence = cocos2d::CCSequence::create(
            delay,
            call,
            nullptr
        );

        cocos2d::CCDirector::sharedDirector()->getRunningScene()->runAction(sequence);
    }

    void onlineRewardStatusFailed() {
        if (this->m_fields->text == "soggy") {
            cocos2d::CCDelayTime* delayCompleteAnimation = cocos2d::CCDelayTime::create(4);
            cocos2d::CCCallFunc* callCompleteAnimation = cocos2d::CCCallFunc::create(this, callfunc_selector(TheWraithHook::showSuccessAnimationWrapper));
            cocos2d::CCDelayTime* delayOpenChest = cocos2d::CCDelayTime::create(1);
            cocos2d::CCCallFunc* callFadeInLoadingCircle = cocos2d::CCCallFunc::create(this, callfunc_selector(TheWraithHook::fadeInLoadingCircleWrapper));
            cocos2d::CCCallFunc* callOpenChest = cocos2d::CCCallFunc::create(this, callfunc_selector(TheWraithHook::showCollectRewardWrapper));

            cocos2d::CCSequence* sequence = cocos2d::CCSequence::create(
                delayCompleteAnimation,
                callCompleteAnimation,
                delayOpenChest,
                callFadeInLoadingCircle,
                callOpenChest,
                nullptr
            );

            cocos2d::CCDirector::sharedDirector()->getRunningScene()->runAction(sequence);
        } else {
            SecretLayer5::onlineRewardStatusFailed();
        }
    }
};