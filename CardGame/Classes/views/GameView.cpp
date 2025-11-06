#include "GameView.h"
#include "models/GameModel.h"
#include "views/CardView.h"
#include "controllers/GameController.h"

USING_NS_CC;
namespace {
    // UI 布局常量
    const char* kButtonFont = "Microsoft YaHei";
    const float kButtonFontSize = 36.0f;
    const Vec2 kButtonPosition(900, 400);
    const Color4B kButtonTextColor = Color4B::WHITE;
    const float kButtonTouchScale = 1.2f;
    const int kButtonZOrder = 100;
    // 触摸卡牌时的视觉反馈
    const GLubyte kCardTouchedOpacity = 180;
}

GameView* GameView::create(GameModel& model) {
    auto view = new (std::nothrow) GameView();
    if (view && view->init(model)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}
GameView::~GameView() {}

bool GameView::init(GameModel& model) {
    if (!Node::init()) {
        return false;
    }
    _gameController = std::make_unique<GameController>(model);
    //创建所有卡牌视图
    generateCardViews(model);
    //创建UI元素
    setupUI();
    //注册触摸事件
    registerTouchEvents();
    return true;
}

void GameView::generateCardViews(GameModel& model) {
    auto handleCardClick = [this](CardView* cardView) {
        // 提供视觉反馈
        cardView->setOpacity(kCardTouchedOpacity);
        // 调用 GameController 处理逻辑
        CardModel& cardModel = cardView->_cardManager->getModel();
        CCLOG("CardView: 卡牌 %d 被点击，区域为 %d", cardModel._id, static_cast<int>(cardModel.getZone()));

        if (cardModel.getZone() == CardZone::Playfield) {
            _gameController->selectCardFromPlayefieldAndMatch(cardModel);
        }
        else if (cardModel.getZone() == CardZone::Stack) {
            _gameController->clickStackCard(cardModel);
        }
        };

    const auto& playfield = model.getPlayfield();
    for (const auto& cardModel : playfield) { // 使用 const auto& 避免不必要的拷贝
        if (auto cardView = CardView::create(cardModel, Vec2::ZERO)) {
            this->addChild(cardView);
            _playfieldCardViews.push_back(cardView);
            cardView->setClickCallback(handleCardClick);
        }
    }
    // 优化循环：一次性完成 Stackfield 卡牌的创建、添加和回调设置
    const auto& stackfield = model.getStackfield();
    for (const auto& cardModel : stackfield) { // 使用 const auto&
        if (auto cardView = CardView::create(cardModel, Vec2::ZERO)) {
            this->addChild(cardView);
            _stackfieldCardViews.push_back(cardView);
            cardView->setClickCallback(handleCardClick);
        }
    }
}
void GameView::setupUI() {
    _statusLabel = Label::createWithSystemFont(u8"回退", kButtonFont, kButtonFontSize);
    _statusLabel->setPosition(kButtonPosition);
    _statusLabel->setTextColor(kButtonTextColor);
    this->addChild(_statusLabel, kButtonZOrder);
}

void GameView::registerTouchEvents() {
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true); // 阻止触摸事件向下层传递

    // onTouchBegan: 检查触摸点是否在标签内，并提供视觉反馈
    touchListener->onTouchBegan = [this](Touch* touch, Event* event) {
        if (_statusLabel && _statusLabel->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation()))) {
            _statusLabel->setScale(kButtonTouchScale); // 高亮效果
            return true; // 表示消费此事件
        }
        return false; // 不处理此事件
        };

    // onTouchEnded: 触摸结束时，如果仍在标签内，则触发点击逻辑
    touchListener->onTouchEnded = [this](Touch* touch, Event* event) {
        _statusLabel->setScale(1.0f); // 恢复正常大小
        if (_statusLabel && _statusLabel->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation()))) {
            onLabelClicked();
        }
        };

    // onTouchCancelled: 触摸被取消（如电话接入），恢复视觉状态
    touchListener->onTouchCancelled = [this](Touch* touch, Event* event) {
        if (_statusLabel) {
            _statusLabel->setScale(1.0f);
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void GameView::onLabelClicked() {
    CCLOG(u8"“回退”按钮被点击");
    _gameController->handleLabelClick();
}