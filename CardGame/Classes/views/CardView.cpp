#include "CardView.h"
#include "configs/models/CardResConfig.h"

USING_NS_CC;
//布局常量初始化
const Vec2 CardView::_smallNumberPos(-80, 130);
const Vec2 CardView::_suitIconPos(80, 130);
const Vec2 CardView::_bigNumberPos(0, 0);
CardView* CardView::create(const CardModel& model, const Vec2& offset) {
    auto view = new (std::nothrow) CardView();
    if (view && view->init(model, offset)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}
bool CardView::init(const CardModel& model, const Vec2& offset) {
    if (!Node::init()) {
        return false;
    }
    _cardManager = new CardManager(model);
    setupVisuals(model);
    this->setPosition(model.getPosition() + offset);
    _cardManager->setCard(model, this);

    return true;
}
void CardView::setClickCallback(const ClickCallback& callback) {
    _clickCallback = callback;

    if (_cardManager) { // 空指针检查
        _cardManager->setCardClickedCallback([this](CardModel& model) {
            if (_clickCallback) {
                _clickCallback(this);
            }
            });
    }
}
bool CardView::isTouchInside(const Vec2& touchPos) const {
    if (!_background) {
        return false;
    }
    return _background->getBoundingBox().containsPoint(touchPos);
}
void CardView::setupVisuals(const CardModel& model) {
    _background = createAndAddSprite(CardResConfig::getBackGround(), Vec2::ZERO, Vec2::ANCHOR_MIDDLE);
    if (_background) {
        this->setContentSize(_background->getContentSize());
    }
    createAndAddSprite(CardResConfig::getSmallNumberRes(model.getSuit(), model.getFace()),
        _smallNumberPos, Vec2::ANCHOR_TOP_LEFT);
    createAndAddSprite(CardResConfig::getBigNumberRes(model.getSuit(), model.getFace()),
        _bigNumberPos, Vec2::ANCHOR_MIDDLE);
    createAndAddSprite(CardResConfig::getSuitRes(model.getSuit()),
        _suitIconPos, Vec2::ANCHOR_TOP_RIGHT);
}

Sprite* CardView::createAndAddSprite(const std::string& resourcePath, const Vec2& position, const Vec2& anchorPoint) {
    if (resourcePath.empty()) {
        CCLOG("CardView: 资源路径为空，无法创建 Sprite。");
        return nullptr;
    }

    auto sprite = Sprite::create(resourcePath);
    if (sprite) {
        sprite->setAnchorPoint(anchorPoint);
        sprite->setPosition(position);
        this->addChild(sprite);
    }
    else {
        CCLOG("CardView: 资源加载失败: %s", resourcePath.c_str());
    }
    return sprite;
}