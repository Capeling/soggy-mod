#ifndef __SOGLAYER_HPP__
#define __SOGLAYER_HPP__

#include <Geode/DefaultInclude.hpp>

namespace soggy_mod {

class SogLayer : public cocos2d::CCLayer {
    public:
    static SogLayer* create(bool fromRope);
    static cocos2d::CCScene* scene(bool fromRope);

protected:

    bool init(bool fromRope);

    void onClose(CCObject*);
    void keyBackClicked();

    cocos2d::CCSprite* m_background = nullptr;
    bool m_fromRope = false;
};

}

#endif /* __SOGLAYER_HPP__ */