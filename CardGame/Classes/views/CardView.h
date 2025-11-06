#pragma once
#include "cocos2d.h"
#include "models/CardModel.h"
#include "managers/CardManager.h" 
#include <functional>
class CardManager;
//卡牌的视图表现层，负责渲染和响应触摸。
class CardView : public cocos2d::Node {
public:

    static CardView* create(const CardModel& model, const cocos2d::Vec2& offset);    // 静态工厂方法，创建 CardView 实例。
    using ClickCallback = std::function<void(CardView* cardView)>;
    void setClickCallback(const ClickCallback& callback);    //设置当卡牌被点击时触发的回调函数。
    bool isTouchInside(const cocos2d::Vec2& touchPos) const;    //检查一个触摸点是否落在卡牌的背景区域内。
    CardManager* _cardManager = nullptr;

protected:
    bool init(const CardModel& model, const cocos2d::Vec2& offset);

private:
    void setupVisuals(const CardModel& model);
    cocos2d::Sprite* createAndAddSprite(const std::string& resourcePath, const cocos2d::Vec2& position, const cocos2d::Vec2& anchorPoint);
    cocos2d::Sprite* _background = nullptr;

    // 布局常量
    static const cocos2d::Vec2 _smallNumberPos;
    static const cocos2d::Vec2 _suitIconPos;
    static const cocos2d::Vec2 _bigNumberPos;

    ClickCallback _clickCallback;
    bool _isSelected = false;
};

