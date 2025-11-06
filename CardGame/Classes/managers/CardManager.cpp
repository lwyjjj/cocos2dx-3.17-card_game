#include "managers/CardManager.h"
#include "views/CardView.h"
#include "services/CardIdManagerMap.h"
#include <iostream>

USING_NS_CC;

CardManager::CardManager(const CardModel& model)
    : _model(model),
    _view(nullptr),
    _isSelected(false) {
    // 在创建时注册当前卡牌管理器
    CardIdManagerMap::getInstance().addCardManager(model._id, this);
}
CardManager::~CardManager() = default;
void CardManager::setCard(const CardModel& model, CardView* view) {
    if (!view) {
        CCLOG(u8"[警告] setCard 传入的视图指针为空，无法绑定。");
        return;
    }
    _model = model;
    _view = view;
    CardIdManagerMap::getInstance().addCardManager(model._id, this);
    setupTouchEvents();  // 初始化触摸事件监听
}

void CardManager::setupTouchEvents() {
    if (_view == nullptr) {
        CCLOG(u8"[警告] CardManager::setupTouchEvents - _view 为空，无法注册触摸事件。");
        return;
    }
    // 创建并配置触摸监听器
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true); // 阻止事件向下传递
    // 注册触摸事件回调
    listener->onTouchBegan = CC_CALLBACK_2(CardManager::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(CardManager::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(CardManager::onTouchCancelled, this);

    // 绑定监听器到视图
    _view->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, _view);
}
bool CardManager::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (_view == nullptr) return false;
    const auto touchPos = _view->convertToNodeSpace(touch->getLocation());
    if (!_view->isTouchInside(touchPos)) return false;
    CCLOG(u8"CardManager::onTouchBegan -> 点击开始，卡片放大");
    _view->setScale(1.0f); 
    _isSelected = true;
    return true;
}

void CardManager::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (_view == nullptr) return;
    _view->setScale(1.0f);
    CCLOG(u8"CardManager::onTouchEnded -> 点击结束，恢复原大小");
    // 若有注册点击回调，则触发
    if (_cardClickedCallback) {
        CCLOG(u8"CardManager::onTouchEnded -> 触发点击回调");
        _cardClickedCallback(_model);
    }
    _isSelected = false;
}
void CardManager::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event) {
    if (_view == nullptr) return;
    _view->setScale(1.0f);
    _isSelected = false;
    CCLOG(u8"CardManager::onTouchCancelled -> 触摸取消，恢复原大小");
}
void CardManager::setCardClickedCallback(const std::function<void(CardModel&)>& callback) {
    _cardClickedCallback = callback;
}

