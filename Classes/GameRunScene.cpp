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
    
    //auto physicalWorld = scene->getPhysicsWorld();
    //layer->setWithPhysicsWorld(physicalWorld);

    layer->worldForce.speed = 96.0;
    layer->worldForce.gravity = -49.0f;
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
    CCLOG("speed %02f gravity %02f",this->worldForce.speed,this->worldForce.gravity);
    auto moveBy = MoveBy::create(0.2f, Vec2(this->worldForce.speed * 0.2,0));
    moveBy->setTag(kActionRun);
       //auto rotate = RotateBy::create(0.2, 90);
    this->_runMan->runAction(RepeatForever::create(moveBy));
    
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
    //auto layer1 = map->getLayer("layer1");
    
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
        this->_runMan->setAnchorPoint(Vec2(0.5,0));
//        auto size = this->_runMan->getContentSize();
//        auto phyBody = PhysicsBody::createBox(size);
//        phyBody->setCollisionBitmask(0x03);
//        
//        this->_runMan->setPhysicsBody(phyBody);
        
        this->addChild(this->_runMan);
        
    }
//    
//    auto tiledLayer = _tiledMap->getLayer("layer1");
//    auto layerSize = tiledLayer->getLayerSize();
//    auto tiles = tiledLayer->getTiles();
//    auto tilesEnd = tiles + static_cast<uint32_t>(layerSize.width * layerSize.height);
//    Vec2 tiledPos = Vec2(0,0);
//    CCLOG("tiltteLayer %02f %02f",layerSize.width,layerSize.height);
//    for(int flag = 0;tiles < tilesEnd;flag++,tiles++){
//        if(*tiles != 0){
//            tiledPos.x = int(flag % int(layerSize.width));
//            tiledPos.y = int(flag / int(layerSize.width));
//            auto tileSprite = tiledLayer->getTileAt(tiledPos);
//            auto phyBodyTmp = PhysicsBody::createBox(Size(32,32));
//            phyBodyTmp->setDynamic(false);
//            tileSprite->setPhysicsBody(phyBodyTmp);
//        }
//    }
    
//    auto contactListener = EventListenerPhysicsContact::create();
//    contactListener->onContactBegin = CC_CALLBACK_1(GameRunScene::onContactBegin, this);
//    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    
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
    
    this->schedule(schedule_selector(GameRunScene::updateLogic), 0.3);
    return true;
}

bool GameRunScene::onContactBegin(PhysicsContact&  contact){
    
    return true;
}

void GameRunScene::updateLogic(float dt){
    Vec2 pos = this->_runMan->getPosition();
    pos.y += 5;
    Vec2 tiledPos = this->tiledCoorForPosition(pos);
    
    //tiledPos.y +=1;//tiled原点左上，脚下需要增加一格
    CCLOG("update %02f %02f",pos.x,pos.y);
    CCLOG("tiled %02f %02f",tiledPos.x,tiledPos.y);
    int gid = _tiledMap->getLayer("layer1")->getTileGIDAt(tiledPos);
    CCLOG("gid  %d",gid);
    if(gid > 0 && gid < 100){
        
        if( this->runStep == kStepOnAir){
            runStep = kStepOnGround;
            this->startRun();
        }
    }
    else if(tiledPos.y > _tiledMap->getMapSize().height){
         CCLOG("stop run");
        //auto jumpFall = JumpBy::create(0.2,Vec2(10,-80),20,1);
        this->_runMan->stopAllActions();
        this->getEventDispatcher()->removeEventListenersForTarget(this);
        this->unschedule(schedule_selector(GameRunScene::updateLogic));
        //this->_runMan->runAction(jumpFall);
      
    }
    else{
        
        if(this->runStep == kStepOnGround){
            this->runStep = kStepOnAir;
            CCLOG("fall run");
            this->_runMan->stopAllActions();
        }
        if(runStep == kStepOnAir){
            this->_runMan->setPositionY(this->_runMan->getPositionY() + this->worldForce.gravity * dt);
        }
    }
    
}

void GameRunScene::jumpOnce(){
    auto jumpTo = JumpBy::create(0.2, Vec2(80,240), 20, 1);
    //auto jumpFall = JumpBy::create(0.2,Vec2(10,-40),20,1);
    auto moveBy = RepeatForever::create(MoveBy::create(0.1, Vec2(20,0)));
                                        
    this->_runMan->runAction(Sequence::create(jumpTo,moveBy, NULL));
}


bool GameRunScene::onTouchBegan(Touch *touch, Event * event){
    if(actionState == kActionRun){
        this->_runMan->stopAllActionsByTag(kActionRun);
        actionState = kActionJump;
        this->jumpOnce();
    }
  
    
    return true;
}
void GameRunScene::onTouchMoved(Touch *touch, Event * event){
    

}
void GameRunScene::onTouchEnded(Touch *touch, Event * event){
    
}