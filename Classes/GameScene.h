//
//  GameScene.h
//  sample_game
//
//  Created by 岩崎和幸 on 2015/02/20.
//
//

#ifndef __sample_game__GameScene__
#define __sample_game__GameScene__

#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
private:
    int nextNumber;
    float gameTime;
    
    void makeButtons();
    void makeTimerLabel();
    void makeHighScoreLabel();
    void countTimer(float time);
    void afterGame();
    
    void menuRetryButton(Ref* node);
    
public:
    virtual bool init();
    static cocos2d::Scene* createScene();
    CREATE_FUNC(GameScene);
    
    bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    
};

#endif /* defined(__sample_game__GameScene__) */
