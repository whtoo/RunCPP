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

bool GameRunScene::init(){
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto map = cocos2d::experimental::TMXTiledMap::create("t1.tmx");
    _tiledMap = map;
    
    auto runner = RunnerSprite::createWithTMX(_tiledMap);
    setRunner(runner);
    addChild(_tiledMap);
    addChild(runner);
    
    return true;
}

void GameRunScene::onEnter(){
    Layer::onEnter();
    
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
    
}

void GameRunScene::onExit(){
    this->getEventDispatcher()->removeEventListenersForTarget(this);
    Layer::onExit();
}




bool GameRunScene::onTouchBegan(Touch *touch, Event * event){
    if(runner->getRunState() == kROLERUN || runner->getRunState() == kROLESTANDBY){
        runner->setRunState(kROLEJUMP);
    }
    if(runner->getRunState() == kROLEJUMP || runner->isDoubleJumped() == false){
        runner->setRunState(kROLEJUMP2);
        runner->setDoubledJumped(true);
    }
   
    return true;
}
void GameRunScene::onTouchMoved(Touch *touch, Event * event){
    

}
void GameRunScene::onTouchEnded(Touch *touch, Event * event){
    
}