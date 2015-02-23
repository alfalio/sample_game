//
//  GameScene.cpp
//  sample_game
//
//  Created by 岩崎和幸 on 2015/02/20.
//
//

#include "GameScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

enum {
    kTagTimerLabel = 100,
    kTagHighScoreLabel = 200,
};

USING_NS_CC;

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool GameScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    nextNumber = 1;
    gameTime = 0;
    srand((unsigned)time(NULL));
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    makeButtons();
    makeHighScoreLabel();
    makeTimerLabel();
    
    this->schedule(schedule_selector(GameScene::countTimer));
    
    return true;
    
}

void GameScene::makeButtons()
{
    auto bgSize = Director::getInstance()->getWinSize();
    
    vector<int> number;
    for (int i = 1; i <= 9; i++) {
        number.push_back(i);
    }
    
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            int index = rand() % number.size();
            
            auto fileName = String::createWithFormat("card_%01d.png",number[index]);
            auto button = Sprite::create(fileName->getCString());
            button->setPosition(Point(x * 128 + bgSize.width / 2 - 128, y * 128 + bgSize.height / 2 - 128));
            button->setTag(number[index]);
            this->addChild(button);
            
            number.erase(number.begin() + index);
        }
    }
}

void GameScene::makeTimerLabel()
{
    auto bgSize = Director::getInstance()->getWinSize();
    auto timerLabel = LabelTTF::create("0.000s", "", 32.0f);
    timerLabel->setPosition(Point(bgSize.width * 0.9f, bgSize.height * 0.9f));
    timerLabel->setTag(kTagTimerLabel);
    this->addChild(timerLabel);
}

void GameScene::makeHighScoreLabel()
{
    float highScore = UserDefault::getInstance()->getFloatForKey("highscore", 99.999f);
    
    auto bgSize = Director::getInstance()->getWinSize();
    auto highScoreString = String::createWithFormat("%8.3fs", highScore);
    auto highScoreLabel = LabelTTF::create(highScoreString->getCString(), "", 32.0f);
    highScoreLabel->setPosition(Point(bgSize.width * 0.9f, bgSize.height * 0.1f));
    highScoreLabel->setTag(kTagHighScoreLabel);
    this->addChild(highScoreLabel);
}

void GameScene::countTimer(float time)
{
    gameTime += time;
    
    auto timeString = String::createWithFormat("%8.3fs", gameTime);
    auto timerLabel = (LabelTTF*)this->getChildByTag(kTagTimerLabel);
    timerLabel->setString(timeString->getCString());
}

void GameScene::afterGame()
{
    this->unschedule(schedule_selector(GameScene::countTimer));
    
    float highScore = UserDefault::getInstance()->getFloatForKey("highscore", 99.999f);
    if (highScore > gameTime) {
        UserDefault::getInstance()->setFloatForKey("highscore", gameTime);
        UserDefault::getInstance()->flush();
        
        auto highScoreString = String::createWithFormat("%8.3fs", gameTime);
        auto highScoreLable = (LabelTTF*)this->getChildByTag(kTagHighScoreLabel);
        highScoreLable->setString(highScoreString->getCString());
    }
    
    auto bgSize = Director::getInstance()->getWinSize();
    auto *retryLabel = MenuItemFont::create("Retry", CC_CALLBACK_1(GameScene::menuRetryButton, this));
    retryLabel->setPosition(Point(bgSize.width * 0.9f, bgSize.height * 0.2f));
    
    auto menu = Menu::create(retryLabel, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
}

void GameScene::menuRetryButton(Ref* node)
{
    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
    auto gameScene = (Scene*)GameScene::createScene();
    Director::getInstance()->replaceScene(gameScene);
}

bool GameScene::onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
    return true;
}

void GameScene::onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent)
{
    auto touchPoint = Director::getInstance()->convertToGL(pTouch->getLocationInView());
    
    auto node = this->getChildByTag(nextNumber);
    if (node->boundingBox().containsPoint(touchPoint)) {
        auto button = Sprite::create("card_back.png");
        button->setPosition(node->getPosition());
        button->setTag(nextNumber);
        this->addChild(button);
        
        node->removeFromParentAndCleanup(true);
        
        if (nextNumber >= 9) {
            afterGame();
            return;
        }
        
        nextNumber++;
    }
}
