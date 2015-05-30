#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class MenuScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void onEnter() override;
    virtual void onExit() override;
    // implement the "static create()" method manually
    CREATE_FUNC(MenuScene);
    CC_SYNTHESIZE(cocos2d::MenuItemSprite*, startBtn, StartBtn);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onExitClicked(Ref* sender);
private:
    void menuCallback(Ref* sender);
    void onStartClicked(Ref* sender);
    CC_SYNTHESIZE(cocos2d::Node*, alertNode, AlertNode);
};

#endif // __HELLOWORLD_SCENE_H__
