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
    kRUN = 1,//奔跑
    kJUMP =2,//跳跃
    kJUMP2 = 21,//连跳
    kJUMPDOWN = 3,//下落
    kHURT = 4,//受伤
    kDEAD = 5,//死亡
    kStandBy = 6,
};


class RunnerSprite :public cocos2d::Node{
public:
    RunnerSprite();
    ~RunnerSprite();
    static RunnerSprite* createWithTMX(cocos2d::TMXTiledMap* map);
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    void extralInit();
    CC_SYNTHESIZE(RUNSTATE, runState, RunState);
    CC_SYNTHESIZE(cocos2d::Sprite*, mRunner, MRunner);
    CC_SYNTHESIZE(cocos2d::TMXTiledMap*, mMap, MMap);
    CC_PROPERTY(cocos2d::Vec2, rolePos, RolePos);
    void runner_update(float dt);
    void runner_logic();
    bool isDoubleJumped();
    void setDoubledJumped(bool flag);
private:
    /*
       y ^
         |
         |
         ----->x
     */
    bool doubleJumpFlag = false;
    cocos2d::Vec2 _paceIntern;//速度
    cocos2d::Vec2 _forceIntern;//力
    float _massIntern;//质量
    
};

#endif /* defined(__RunCpp__Runner__) */
