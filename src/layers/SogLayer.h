#include <Geode/Geode.hpp>

class SogLayer : public cocos2d::CCLayer {
protected:
    cocos2d::CCSprite* m_background;
public:
    static SogLayer* create();
    static cocos2d::CCScene* scene();
    bool init();
    void onClose(CCObject*);
    void keyBackClicked();
};