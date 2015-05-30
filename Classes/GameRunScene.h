//
//  GameRunScene.h
//  RunCpp
//
//  Created by hong bo on 15/5/20.
//
//

#ifndef __RunCpp__GameRunScene__
#define __RunCpp__GameRunScene__

#include <stdio.h>
#include <string>
#include <vector>
#include "cocos2d.h"
#include "Runner.h"

class GameRunScene : public cocos2d::Layer{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameRunScene);
    cocos2d::Vec2 tiledCoorForPosition(const cocos2d::Vec2& position);
    cocos2d::Vec2 positionForTiledCoor(const cocos2d::Vec2& tiledCoor);
    CC_SYNTHESIZE(RunnerSprite*, runner, Runner);
    CC_SYNTHESIZE(cocos2d::Sprite*, farBg, FarBg);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void update_BgLayer(float dt);
private:
    
    cocos2d::experimental::TMXTiledMap* _tiledMap;
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event * event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event * event);
 
    
   
};

#endif /* defined(__RunCpp__GameRunScene__) */
