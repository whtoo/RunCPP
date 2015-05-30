#include "MenuScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameRunScene.h"
#include "audio/include/SimpleAudioEngine.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* MenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");
    setAlertNode( rootNode->getChildByName("alertNode"));
    this->addChild(rootNode,-1);
    
    MenuItemFont::setFontName( "宋体" );
    MenuItemFont::setFontSize(48);
    auto title2 = MenuItemFont::create( "菲菲快跑" );
    title2->setEnabled(false);
    title2->setColor(Color3B(255,255,200));
  
    // Font Item
    auto spriteNormal = Sprite::create("start_normal.png");
    auto spriteSelected = Sprite::create("start_select.png");
    
    auto item1 = MenuItemSprite::create(spriteNormal, spriteSelected, CC_CALLBACK_1(MenuScene::onStartClicked, this) );
    startBtn = item1;
    auto menu = Menu::create(title2,item1,nullptr );
    menu->alignItemsInColumns(1, 1, NULL);
    addChild( menu);
    
    auto s = Director::getInstance()->getWinSize();
    menu->setAnchorPoint(Vec2(0.5,0.5));
    menu->setPosition(Vec2(s.width/2, s.height/2));
    alertNode->retain();
    alertNode->removeFromParent();
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("background.mp3",true);
    return true;
}

void MenuScene::onEnter(){
    Layer::onEnter();
    //按键控制（检测onKeyReleased有反应）
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(MenuScene::onKeyReleased, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    auto lis1 = EventListenerTouchOneByOne::create();
    lis1->onTouchEnded = CC_CALLBACK_1(MenuScene::onStartClicked, this) ;
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(lis1, startBtn);
}

void MenuScene::onExit(){
    this->getEventDispatcher()->removeAllEventListeners();
    Layer::onExit();
}

void MenuScene::onExitClicked(Ref* sender){
    CCLOG("onexit");
    Director::getInstance()->end();
}

void MenuScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch(keyCode)
    {
            //监听返回键
        case EventKeyboard::KeyCode::KEY_ESCAPE:
             Director::getInstance()->end();
            break;
            //监听menu键
        case EventKeyboard::KeyCode::KEY_MENU:
            break;
    }
    
}


void MenuScene::onStartClicked(Ref* sender){
    CCLOG("selected 1");
    auto director = Director::getInstance();
    auto gameScene = GameRunScene::createScene();
    
    director->pushScene(gameScene);
    
}
void MenuScene::menuCallback(Ref* sender)
{
    CCLOG("selected 2");
}