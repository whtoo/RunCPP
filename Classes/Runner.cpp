//
//  Runner.cpp
//  RunCpp
//
//  Created by hong bo on 15/5/25.
//
//

#include "Runner.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

RunnerSprite::RunnerSprite(){};
RunnerSprite::~RunnerSprite(){};

bool RunnerSprite::isDoubleJumped(){
    return doubleJumpFlag;
}

void RunnerSprite::setDoubledJumped(bool flag){
    this->doubleJumpFlag = flag;
}

void RunnerSprite::extralInit(){
    auto objGroup = this->mMap->getObjectGroup("Role");
    ValueMap obj = static_cast<ValueMap>(objGroup->getObject("player"));
    
//    addChild(map, 0, kTagTileMap);
//    float spX = obj["x"].asFloat();
//    float spY = obj["y"].asFloat();
//    CCLOG("%.2f %.2f",spX,spY);
//    CCLOG("tiled 1 %02f %02f",_tiledMap->getMapSize().width,_tiledMap->getMapSize().height);
//    CCLOG("tiled 2 %02f %02f",_tiledMap->getTileSize().width,_tiledMap->getTileSize().height);
//    
//    if(this->_runMan == nullptr){
//        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(";", "playerrun.png");
//        this->_runMan = cocos2d::Sprite::createWithSpriteFrameName("image1.png");
//        this->_runMan->setPosition(Vec2(spX,spY));
//        this->_runMan->setAnchorPoint(Vec2(0.5,0.0));
//        bool flag = Rect(1,2,3,2).intersectsRect(Rect(1,2,3,2));
//        if(flag){
//            CCLOG("collision");
//        }
//        this->addChild(this->_runMan);
//        
//    }
}
static RunnerSprite* createWithTMX(cocos2d::TMXTiledMap* map){
    auto runner = new RunnerSprite();
    
    if(runner && runner->init()){
        runner->setMMap(map);
        runner->extralInit();
        runner->autorelease();
        return runner;
    }
    
    CC_SAFE_DELETE(runner);
    return nullptr;
};

void RunnerSprite::runner_update(float dt){
    runner_logic();
}

void RunnerSprite::runner_logic(){

}
bool RunnerSprite::init(){
    
    return true;
};

void RunnerSprite::onEnter(){
    Node::onEnter();
    this->schedule(schedule_selector(RunnerSprite::runner_update), 0.016f);
};

void RunnerSprite::onExit(){
    this->unschedule(schedule_selector(RunnerSprite::runner_update));
    Node::onExit();
};

Vec2 RunnerSprite::getRolePos(){
    return mRunner->getPosition();
};

void RunnerSprite::setRolePos(Vec2 pos){
    mRunner->setPosition(pos);
}