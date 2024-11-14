#ifndef SOGLAYER_HPP
#define SOGLAYER_HPP

#include <Geode/Geode.hpp>

class SogLayer : public cocos2d::CCLayer {
protected:
    cocos2d::CCSprite* m_background = nullptr;
    bool m_fromRope = false;
public:
    static SogLayer* create(bool fromRope);
    static cocos2d::CCScene* scene(bool fromRope);
    bool init(bool fromRope);
    void onClose(CCObject*);
    void onHonk(CCObject*);
    void keyBackClicked();
};

#endif