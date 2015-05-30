#include "MenuScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GameRunScene.h"

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

    this->addChild(rootNode);
    
    MenuItemFont::setFontName( "宋体" );
    MenuItemFont::setFontSize(48);
    auto title2 = MenuItemFont::create( "菲菲快跑" );
    title2->setEnabled(false);
    title2->setColor(Color3B(255,255,200));
  
    // Font Item
    auto spriteNormal = Sprite::create("start_normal.png");
    auto spriteSelected = Sprite::create("start_select.png");
    
    auto item1 = MenuItemSprite::create(spriteNormal, spriteSelected, CC_CALLBACK_1(MenuScene::onStartClicked, this) );
    auto menu = Menu::create(title2,item1,nullptr );
    menu->alignItemsInColumns(1, 1, NULL);
    addChild( menu);
    
    auto s = Director::getInstance()->getWinSize();
    menu->setAnchorPoint(Vec2(0.5,0.5));
    menu->setPosition(Vec2(s.width/2, s.height/2));
   

    return true;
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