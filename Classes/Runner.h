//
//  Runner.h
//  RunCpp
//
//  Created by hong bo on 15/5/25.
//
//

#ifndef __RunCpp__Runner__
#define __RunCpp__Runner__

#include <stdio.h>
#include "cocos2d.h"

//奔跑者状态
enum RUNSTATE : int{
    kROLERUN = 1,//奔跑
    kROLEJUMP =2,//跳跃
    kROLEJUMP2 = 21,//连跳
    kROLEJUMPDOWN = 3,//下落
    kROLEHURT = 4,//受伤
    kROLEDEAD = 5,//死亡
    kROLESTANDBY = 6,
    kROLEINVALID = 10
};

typedef enum : short{
    kTOP = 1,
    kRTOP = 5,
    kBOTTON = 2,
    kRBOTTON = 6,
    kLEFT = 3,
    kRIGHT =4,
    
    kNONE = 100
} CollisionFace;


class RunnerSprite :public cocos2d::Node{
public:
    RunnerSprite();
    ~RunnerSprite();
    static RunnerSprite* createWithTMX(cocos2d::experimental::TMXTiledMap* map);
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    void extralInit();
    CC_SYNTHESIZE(std::vector<cocos2d::Rect> , barriers, Barriers);
    CC_SYNTHESIZE(std::vector<cocos2d::Rect> , goldens, Goldens);
    CC_SYNTHESIZE(RUNSTATE, runState, RunState);
    CC_SYNTHESIZE(cocos2d::Sprite*, mRunner, MRunner);
    CC_SYNTHESIZE(cocos2d::experimental::TMXTiledMap*, mMap, MMap);
    CC_PROPERTY(cocos2d::Vec2, rolePos, RolePos);
    CC_SYNTHESIZE(float, xSpeed, XSpeed);
    CC_SYNTHESIZE(float, ySpeed, YSpeed);
    CC_SYNTHESIZE(float, constXSpeed, ConstXSpeed);
    CC_SYNTHESIZE(float, constYSpeed, ConstYSpeed);
    CC_SYNTHESIZE(float, gravity, Gravity);
    CC_SYNTHESIZE(cocos2d::Vec2, subPos, SubPos);
    void runner_update(float dt);
    void runner_logic();
    bool isDoubleJumped();
    void setDoubledJumped(bool flag);
    void runLogic();
    void jumpUpLogic();
    void jumpDownLogic();
    void camera_update(float dt);
    bool isCollisionWithBarriers(CollisionFace face);
    bool isCollisionWithTop(cocos2d::Rect box);
    bool isCollisionWithBottom(cocos2d::Rect box);
    bool isCollisionWithLeft(cocos2d::Rect box);
    bool isCollisionWithRight(cocos2d::Rect box);
    bool isCollisionWithRTop(cocos2d::Rect box);
    bool isCollisionWithRBottom(cocos2d::Rect box);
    void fixCollision(CollisionFace face,cocos2d::Rect box);
    void jumpTwiceUpLogic();
    void result_logic(float dt);
    CC_SYNTHESIZE(cocos2d::Sprite*, farBg, FarBg);

private:
    
   
    /*
       y ^
         |
         |
         ----->x
     */
    void setMapViewByRunner();
    bool doubleJumpFlag = false;
    cocos2d::Vec2 _paceIntern;//速度
    cocos2d::Vec2 _forceIntern;//力
    float _massIntern;//质量
    
};

#endif /* defined(__RunCpp__Runner__) */
