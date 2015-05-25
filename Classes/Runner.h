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

USING_NS_CC;
//奔跑者状态
enum RUNSTATE{
    kRUN = 1,//奔跑
    kJUMP =2,//跳跃
    kDJUMP = 3,//连跳
    kFALLEN = 4,//下落
    kHURT = 5,//受伤
    kDEAD = 6,//死亡
};

class RunnerSprite :public Node{
public:
    CREATE_FUNC(RunnerSprite);
    virtual void onEnter();
    virtual void onExit();
};

#endif /* defined(__RunCpp__Runner__) */
