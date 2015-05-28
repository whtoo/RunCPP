//
//  GameRunScene.cpp
//  RunCpp
//
//  Created by hong bo on 15/5/20.
//
//

#include "GameRunScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;



Scene* GameRunScene::createScene(){
    auto scene = Scene::createWithPhysics();
    
    auto layer = GameRunScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

Vec2 GameRunScene::tiledCoorForPosition(const Vec2& position)   //转成格子坐标
{
    Size mapSize = _tiledMap->getMapSize();
    Size tileSize = _tiledMap->getTileSize();
    int x = (position.x ) / tileSize.width;
    int y = (mapSize.height * tileSize.height - position.y ) / tileSize.height;
    return Vec2(x,y);
}

Vec2 GameRunScene::positionForTiledCoor(const Vec2& tiledCoor)   //转成OpenGL坐标
{
    Size mapSize = _tiledMap->getMapSize();
    Size tileSize = _tiledMap->getTileSize();
    int x = tiledCoor.x * tileSize.width + tileSize.width / 2;
    int y = (mapSize.height * tileSize.height) - (tiledCoor.y * tileSize.height + tileSize.height / 2);
    return Vec2(x,y);
}

void GameRunScene::startRun(){
    this->_runMan->stopAllActions();
    worldForce.speed = Vec2(0, 0);
    worldForce.gravity = -9.8;
    if(runStep == kStepInit || runStep == kStepOnGround){
        Vector<SpriteFrame*> animFrames(15);
        auto cache = SpriteFrameCache::getInstance();
        char str[100] = {0};
        for(int i = 1; i < 17; i++)
        {
            sprintf(str, "image%01d.png", i);
            CCLOG("imgae %s",str);
            auto frame = cache->getSpriteFrameByName( str );
            
            animFrames.pushBack(frame);
        }
        
        auto animation = Animate::create( Animation::createWithSpriteFrames(animFrames,0.1));
        animation->setTag(kActionAnim);

        this->_runMan->runAction(RepeatForever::create(animation));
    }
    actionState = kActionRun;
    runStep = kStepOnGround;
    
}

bool GameRunScene::init(){
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto map = cocos2d::experimental::TMXTiledMap::create("ditu.tmx");
    this->_tiledMap = map;
    
    auto objGroup = map->getObjectGroup("Role");
    ValueMap obj = static_cast<ValueMap>(objGroup->getObject("player"));
    
    addChild(map, 0, kTagTileMap);
    float spX = obj["x"].asFloat();
    float spY = obj["y"].asFloat();
    CCLOG("%.2f %.2f",spX,spY);
    CCLOG("tiled 1 %02f %02f",_tiledMap->getMapSize().width,_tiledMap->getMapSize().height);
    CCLOG("tiled 2 %02f %02f",_tiledMap->getTileSize().width,_tiledMap->getTileSize().height);

    if(this->_runMan == nullptr){
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playerrun.plist", "playerrun.png");
        this->_runMan = cocos2d::Sprite::createWithSpriteFrameName("image1.png");
        this->_runMan->setPosition(Vec2(spX,spY));
        this->_runMan->setAnchorPoint(Vec2(0.5,0.0));
        bool flag = Rect(1,2,3,2).intersectsRect(Rect(1,2,3,2));
        if(flag){
            CCLOG("collision");
        }
        this->addChild(this->_runMan);
        
    }
    
    
    return true;
}

void GameRunScene::onEnter(){
    Layer::onEnter();
    
    this->startRun();
    
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2
    (GameRunScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2
    (GameRunScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2
    (GameRunScene::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2
    (GameRunScene::onTouchCancelled, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->scheduleUpdate();
    
}

void GameRunScene::onExit(){
    
    this->unscheduleUpdate();
    Layer::onExit();
}

bool GameRunScene::onContactBegin(PhysicsContact&  contact){
    CCLOG("onContactBegin");
    
    return true;
}



void GameRunScene::update(float dt){
    
    if(runStep == kStepOnAir){
        worldForce.speed.y += worldForce.gravity;
    }
    
    
    if(worldForce.speed.y < -20){
        worldForce.speed.y = 20;
    }
    
    float sx = dt * worldForce.speed.x + _runMan->getPositionX();
    float sy = dt * worldForce.speed.y + _runMan->getPositionY();
    this->_runMan->setPosition(Vec2(sx,sy));
    
}

void GameRunScene::jumpOnce(){
   
}


bool GameRunScene::onTouchBegan(Touch *touch, Event * event){
    if(actionState == kActionRun){
        worldForce.speed.y = 18;
        runStep = kStepOnAir;
    }
  
    
    return true;
}
void GameRunScene::onTouchMoved(Touch *touch, Event * event){
    

}
void GameRunScene::onTouchEnded(Touch *touch, Event * event){
    
}