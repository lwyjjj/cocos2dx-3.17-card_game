
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "configs/models/CardResConfig.h"
#include "views/CardView.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "services/GameModelFromLevelGenerator.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    // ----------- 顶部区域 -----------
    {
        float topHeight = visibleSize.height * (2.0f / 3.0f);   // 上半区占 2/3 屏幕
        Color4B topColor(255, 215, 0, 255);                     // 金色

        auto* topLayer = LayerColor::create(topColor, visibleSize.width, topHeight);
        if (topLayer) {
            // 设置顶部位置（从下往上 1/3 处开始）
            topLayer->setPosition(Vec2(0.0f, visibleSize.height - topHeight));
            this->addChild(topLayer, 0); // zOrder = 0
        }
        else {
            CCLOG(u8"[警告] 顶部颜色层创建失败！");
        }
    }

    // ----------- 底部区域 -----------
    {
        float bottomHeight = visibleSize.height / 3.0f;         // 下半区占 1/3 屏幕
        Color4B bottomColor(0, 0, 255, 155);                    // 半透明蓝色

        auto* bottomLayer = LayerColor::create(bottomColor, visibleSize.width, bottomHeight);
        if (bottomLayer) {
            bottomLayer->setPosition(Vec2::ZERO);               // 左下角对齐
            this->addChild(bottomLayer, 0);
        }
        else {
            CCLOG(u8"[警告] 底部颜色层创建失败！");
        }
    }

    // 游戏模型与视图生成
    auto gameModel = GameModelFromLevelGenerator::generateGameModel("level_1.json");
    GameModelFromLevelGenerator::generateGameView(gameModel, this);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
