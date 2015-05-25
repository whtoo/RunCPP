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
#include "cocos2d.h"
enum KTILEDMAPFLAG {
    kTagTileMap = 10
};

typedef enum : int {
    kActionRun = 1,
    kActionAnim = 2,
    kActionJump = 3,
} GameActionEnum;

typedef enum : int{
    kStepInit = 1,
    kStepOnGround = 2,
    kStepOnAir = 3,
    kStepStop= 4
} GameStepEnum;

typedef struct ForceWorld{
    float speed = 98.0f;
    float gravity = -49.0f;
} ForceWorld;

class GameRunScene : public cocos2d::Layer{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameRunScene);
    cocos2d::Vec2 tiledCoorForPosition(const cocos2d::Vec2& position);
    cocos2d::Vec2 positionForTiledCoor(const cocos2d::Vec2& tiledCoor);
    void jumpOnce();
    void updateLogic(float dt);
private:
    cocos2d::Sprite* _runMan;
    cocos2d::experimental::TMXTiledMap* _tiledMap;
    cocos2d::PhysicsWorld* _mWorld;
    struct ForceWorld worldForce;
    GameActionEnum actionState = kActionRun;
    void startRun();
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event * event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event * event);
    void setWithPhysicsWorld(cocos2d::PhysicsWorld* world){
        this->_mWorld = world;
    }
    GameStepEnum runStep = kStepInit;
    
   
};

#endif /* defined(__RunCpp__GameRunScene__) */
