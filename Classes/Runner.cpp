//
//  Runner.cpp
//  RunCpp
//
//  Created by hong bo on 15/5/25.
//
//

#include "Runner.h"

USING_NS_CC;

RunnerSprite::RunnerSprite():xSpeed(12),ySpeed(10),constXSpeed(xSpeed),constYSpeed(ySpeed),gravity(0.9){};
RunnerSprite::~RunnerSprite(){};

bool RunnerSprite::isDoubleJumped(){
    return doubleJumpFlag;
}

void RunnerSprite::setDoubledJumped(bool flag){
    this->doubleJumpFlag = flag;
}

void RunnerSprite::extralInit(){
    auto roleGroup = this->mMap->getObjectGroup("role");
    ValueMap obj = static_cast<ValueMap>(roleGroup->getObject("player"));
    
    float spX = obj["x"].asFloat();
    float spY = obj["y"].asFloat();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("playerrun.plist", "playerrun.png");
    mRunner = cocos2d::Sprite::createWithSpriteFrameName("image1.png");
    mRunner->setPosition(Vec2(spX,spY));
    mRunner->setAnchorPoint(Vec2(0.5,0.0));
    
    mMap->addChild(mRunner);
    auto runAnim = Animation::create();
    for(int i = 1;i<17;i++){
        char nameBuf[100] = {0};
        sprintf(nameBuf, "image%d.png",i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(nameBuf);
        runAnim->addSpriteFrame(frame);
    }
    runAnim->setDelayPerUnit(0.06f);
    runAnim->setRestoreOriginalFrame(true);
    AnimationCache::getInstance()->addAnimation(runAnim,"runAction");
    
    
    
    auto colGroup = mMap->getObjectGroup("collision");
    auto colObj = colGroup->getObjects();
    CCLOG("objs %lu",colObj.size());
    
    barriers.reserve(colObj.size());
    
    for(ValueVector::iterator it  = colObj.begin(); it != colObj.end(); ++it) {
        ValueMap mp = it->asValueMap();
        barriers.push_back(Rect(mp["x"].asFloat(), mp["y"].asFloat(), mp["width"].asFloat(), mp["height"].asFloat()));
    }
    
    setRunState(kROLERUN);
    
    auto runAim = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("runAction"))) ;
    this->mRunner->runAction(runAim);

}
RunnerSprite* RunnerSprite::createWithTMX(cocos2d::experimental::TMXTiledMap* map){
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
void RunnerSprite::fixCollision(CollisionFace face,Rect box){
    CCLOG("fix face %d,x %f y %f  w %f  h %f",face,box.origin.x,box.origin.y,box.size.width,box.size.height);
    //Director::getInstance()->pause();
    Rect interRect = mRunner->getBoundingBox();
    float maxX = MIN(interRect.getMaxX(), box.getMaxX());
    float minX = MAX(interRect.getMinX(), box.getMinX());
    float maxY = MIN(interRect.getMaxY(), box.getMaxY());
    float minY = MAX(interRect.getMinY(), box.getMinY());
    float width = maxX - minX;
    float height = maxY - minY;
    
    if(face == kBOTTON){
        setRolePos(Vec2(getRolePos().x,getRolePos().y + height));
        return;
    }
    if(face == kTOP){
        setRolePos(Vec2(getRolePos().x,getRolePos().y - height));
          return;
    }
    if(face == kLEFT){
        setRolePos(Vec2(getRolePos().x + width,getRolePos().y ));
        return;
    }
    if(face == kRIGHT){
        if(getRunState() == kROLEJUMP){
            setRolePos(Vec2(getRolePos().x - width ,getRolePos().y ));
        }
        else{
            setRolePos(Vec2(getRolePos().x - width,getRolePos().y ));
        }
        
        return;
    }
    
    if(face == kRTOP){
        if(getRunState() == kROLEJUMP || getRunState() == kROLEJUMP2){
            setRolePos(Vec2(getRolePos().x - width ,getRolePos().y - height));
        }
        else{
            setRolePos(Vec2(getRolePos().x - width ,getRolePos().y ));
        }
        
    }
    
    if(face == kRBOTTON){
        if(getRunState() == kROLEJUMPDOWN){
            setRolePos(Vec2(getRolePos().x - width ,getRolePos().y ));
        }
        else{
            setRolePos(Vec2(getRolePos().x - width ,getRolePos().y +height));

        }
    
    }
    
};


void RunnerSprite::runner_update(float dt){
    runner_logic();
};

void RunnerSprite::runLogic(){
    CCLOG("runner runLogic");
    if(isCollisionWithBarriers(kRIGHT)){
        setRunState(kROLESTANDBY);
        return;
    }
    if (isCollisionWithBarriers(kRTOP)) {
            setRunState(kROLESTANDBY);
            return;
    }
  
    if(!isCollisionWithBarriers(kBOTTON) ){
        setRunState(kROLEJUMPDOWN);
    }
    
    CCLOG("cont");
    Vec2 preXPos = getRolePos();
    Vec2 targetXPos = Vec2(preXPos.x+xSpeed,preXPos.y);
    setRolePos(targetXPos);
    
    
};

void RunnerSprite::jumpTwiceUpLogic(){
    ySpeed += 6;
    setRunState(kROLEJUMP);

};

void RunnerSprite::jumpUpLogic(){
    Vec2 curPos = getRolePos();
    CCLOG("ySpeed %f",ySpeed);
    
    if(isCollisionWithBarriers(kTOP)){
        setRunState(kROLEJUMPDOWN);
        return;
    }
    
    if(isCollisionWithBarriers(kRIGHT)){
        xSpeed = 0;
    }
    else if(isCollisionWithBarriers(kRBOTTON)){
        xSpeed = constXSpeed;
    }
    else if(isCollisionWithBarriers(kRTOP)){
        
        setRunState(kROLEJUMPDOWN);
        return;
    
    }
    
    ySpeed -= gravity;

    if(ySpeed <=0){
        ySpeed = 0;
    }
    
    if(ySpeed == 0){
        setRunState(kROLEJUMPDOWN);
        return;
    }
    
    float xPos = xSpeed + curPos.x;
    float yPos = ySpeed + curPos.y;
    
    setRolePos(Vec2(xPos,yPos));
    
};

void RunnerSprite::jumpDownLogic(){
    
    if(isCollisionWithBarriers(kBOTTON)){
        ySpeed = constYSpeed;
        CCLOG("isCollisionWithBarriers");
        setDoubledJumped(false);
        setRunState(kROLERUN);
        return;
    }
    
    if(isCollisionWithBarriers(kRBOTTON)){
        CCLOG("isCollisionWithBarriers jumpDownLogic");
        
       
    }
    
    Vec2 curPos = getRolePos();
    
    ySpeed += gravity;
    if(ySpeed >= 15){
        ySpeed = 15;
    }
    
    Vec2 rlt = curPos + Vec2(xSpeed,-ySpeed);
    setRolePos(rlt);
};

void RunnerSprite::runner_logic(){
    switch (getRunState()) {
        case kROLERUN:
            this->runLogic();
            break;
        case kROLEJUMP:
            this->jumpUpLogic();
            break;
        case kROLEJUMPDOWN:
            this->jumpDownLogic();
            break;
        case kROLESTANDBY:
            
            break;
        case kROLEJUMP2:
            this->jumpTwiceUpLogic();
            break;
        default:
            break;
    }
    Size size = Director::getInstance()->getVisibleSize();
    if(getRolePos().y < 0){
        auto lb = Label::createWithTTF("GameOver", "fonts/Marker Felt.ttf", 48.0f);
        lb->setAnchorPoint(Vec2(0.5,0.5));
        lb->setPosition(size.width *0.5,size.height * 0.5);
        this->getParent()->addChild(lb);
        this->unschedule(schedule_selector(RunnerSprite::runner_update));
        return;
    }
    float maxX = getMMap()->getTileSize().width * getMMap()->getMapSize().width;
    if(getRolePos().x >= maxX){
        auto lb = Label::createWithTTF("GameWins", "fonts/Marker Felt.ttf", 48.0f);
        lb->setAnchorPoint(Vec2(0.5,0.5));
        lb->setPosition(size.width *0.5,size.height * 0.5);
        this->getParent()->addChild(lb);
        this->unschedule(schedule_selector(RunnerSprite::runner_update));

        return;
    }
    
}
bool RunnerSprite::init(){
    
    return true;
};

bool RunnerSprite::isCollisionWithBarriers(CollisionFace face){
    
    bool bFlag = false;
    CCLOG("face %d",face);
    switch (face) {
        case kTOP:
            for(unsigned int i = 0;i<barriers.size(); i++){
                Rect box = barriers[i];
                bFlag = this->isCollisionWithTop(box);
                if(bFlag) {
                    fixCollision(face,box);
                    break;
                };
            }
            break;
       case kRTOP:
            for(unsigned int i = 0;i<barriers.size(); i++){
                Rect box = barriers[i];
                bFlag = this->isCollisionWithRTop(box);
                if(bFlag) {
                    fixCollision(face,box);
                    break;
                };
            }
            
            break;
        case kBOTTON:
            for(unsigned int i = 0;i<barriers.size(); i++){
                Rect box = barriers[i];
                bFlag = this->isCollisionWithBottom(box);
                if(bFlag) {
                    fixCollision(face,box);
                    break;
                };
            }
             break;
        case kRBOTTON:
            for(unsigned int i = 0;i<barriers.size(); i++){
                Rect box = barriers[i];
                bFlag = this->isCollisionWithRBottom(box);
                if(bFlag) {
                    fixCollision(face,box);
                    break;
                };
            }
            break;
        case kRIGHT:
            for(unsigned int i = 0;i<barriers.size(); i++){
                Rect box = barriers[i];
                bFlag = this->isCollisionWithRight(box);
                if(bFlag) {
                    fixCollision(face,box);
                    break;
                };
            }
             break;
        case kLEFT:
            for(unsigned int i = 0;i<barriers.size(); i++){
                Rect box = barriers[i];
                bFlag = this->isCollisionWithLeft(box);
                if(bFlag) {
                    fixCollision(face,box);
                    break;
                };
            }
             break;
        default:
            CCLOG("invalid face");
            break;
    }
    
    if(bFlag)CCLOG("collisions at %d",face);
    return bFlag;
}
bool RunnerSprite::isCollisionWithRBottom(cocos2d::Rect box){
    auto manBox = mRunner->boundingBox();
    Vec2 manPoint = Vec2(manBox.getMaxX(),manBox.getMinY());
    return box.containsPoint(manPoint);
    
}
bool RunnerSprite::isCollisionWithRTop(cocos2d::Rect box){
    auto manBox = mRunner->boundingBox();
    Vec2 manPoint = Vec2(manBox.getMaxX(),manBox.getMaxY());
    return box.containsPoint(manPoint);
}

bool RunnerSprite::isCollisionWithTop(cocos2d::Rect box){
    auto manBox = mRunner->boundingBox();
    Vec2 manPoint = Vec2(manBox.getMidX(),manBox.getMaxY());
    return box.containsPoint(manPoint);
}

bool RunnerSprite::isCollisionWithBottom(cocos2d::Rect box){
    auto manBox = mRunner->boundingBox();
    Vec2 manPoint = Vec2(manBox.getMidX(),manBox.getMinY());
    return box.containsPoint(manPoint);
}
bool RunnerSprite::isCollisionWithLeft(cocos2d::Rect box){
    auto manBox = mRunner->boundingBox();
    Vec2 manPoint = Vec2(manBox.getMinX(),manBox.getMidY());
    return box.containsPoint(manPoint);
}
bool RunnerSprite::isCollisionWithRight(cocos2d::Rect box){
    auto manBox = mRunner->boundingBox();
    Vec2 manPoint = Vec2(manBox.getMaxX(),manBox.getMidY());
    return box.containsPoint(manPoint);
}
void RunnerSprite::onEnter(){
    Node::onEnter();
    CCLOG("runner onEnter");
    setMapViewByRunner();

    this->schedule(schedule_selector(RunnerSprite::runner_update), 0.016f);
    this->schedule(schedule_selector(RunnerSprite::camera_update), 0.01);
};

void RunnerSprite::setMapViewByRunner(){
    Size visibleSize = CCDirector::getInstance()->getVisibleSize();
    
    Size mapTiledNum =  this->getMMap()->getMapSize();
    Size tiledSize  = getMMap()->getTileSize();
    
    Size mapSize = Size(mapTiledNum.width * tiledSize.width,mapTiledNum.height*mapTiledNum.height );//计算地图大小
    
    Vec2 rolePoint = this->mRunner->getPosition();
    
    float x = rolePoint.x>visibleSize.width/4?rolePoint.x:visibleSize.width /4 ;
    x = x<mapSize.width-visibleSize.width * .8?x:mapSize.width - visibleSize.width * .8;
    
    float y = rolePoint.y>visibleSize.height/3?rolePoint.y:visibleSize.height;
    y= y<mapSize.height/2	? y:mapSize.height/2;
    
    Vec2 desPoint = Vec2(x,y);
    Vec2 centerPoint  = Vec2(visibleSize.width/5,visibleSize.height/4);
    
    Vec2 rltPoint = centerPoint - desPoint;
    
    getMMap()->setPosition(rltPoint);
    CCLOG("map view %f,%f",rltPoint.x,rltPoint.y);
}

void RunnerSprite::camera_update(float dt){
    setMapViewByRunner();
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