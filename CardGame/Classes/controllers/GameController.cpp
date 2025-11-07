#include "controllers/GameController.h"
#include <iostream>
#include "services/CardIdManagerMap.h"
#include "cocos2d.h"

USING_NS_CC;

namespace {
    const float kCardMoveDuration = 0.5f;
    const Vec2 kHandPosition(700, 400);
}
GameController::GameController(GameModel gameModel)
    : _gameModel(std::move(gameModel)), _undoManager(_gameModel.getUndoModel()) {
    CCLOG(u8"GameController 已创建，当前可撤销步数：%d", _undoManager.getUndoSize());
}
GameController::~GameController() {}
bool GameController::selectCardFromPlayefieldAndMatch(CardModel& selectedCard) {
    if (_undoManager.getUndoSize() == 0) {
        CCLOG(u8"匹配失败：没有底牌可供匹配。");
        return false;
    }
    CardModel bottomCard = getBottomCard();
    CCLOG(u8"主牌堆点击：尝试用卡牌 %d 匹配底牌 %d", selectedCard._id, bottomCard._id);
    if (isCardMatch(selectedCard, bottomCard)) {
        UndoCardState state{ selectedCard._id, selectedCard.getPosition(), selectedCard.getZone() };
        _undoManager.recordUndoState(state);
        handleCardClicked(selectedCard);
        CCLOG(u8"匹配成功！");
        return true;
    }
    CCLOG(u8"匹配失败：点数不相邻。");
    return false;
}
void GameController::clickStackCard(CardModel& card) {
    CCLOG(u8"Stack区卡牌 %d 被点击。", card._id);
    UndoCardState state{ card._id, card.getPosition(), card.getZone() };
    _undoManager.recordUndoState(state);
    handleCardClicked(card);
}
bool GameController::undo() {
    UndoCardState state;
    if (_undoManager.undo(state)) {
        CCLOG(u8"执行撤销：恢复卡牌 %d 的状态。", state.id);
        moveCardToOriginalPosition(state);
        return true;
    }
    CCLOG(u8"撤销失败：没有可撤销的操作。");
    return false;
}
CardModel GameController::getBottomCard() {
    UndoCardState state;
    if (_undoManager.undo(state)) {
        _undoManager.recordUndoState(state); // 立即将状态放回，实现“窥探”效果
        const CardModel* card = findCardInModel(state.id);
        if (card) {
            return *card;
        }
    }
    return CardModel(CardFaceType::CFT_ACE, CardSuitType::CST_SPADES, Vec2::ZERO);
}
bool GameController::isCardMatch(const CardModel& card1, const CardModel& card2) const {
    return std::abs(static_cast<int>(card1.getFace()) - static_cast<int>(card2.getFace())) == 1;
}
void GameController::moveCardToOriginalPosition(const UndoCardState& state) {
    // 调用非 const 版本的 findCardInModel，因为需要修改卡牌数据
    CardModel* cardToMove = findCardInModel(state.id);
    if (!cardToMove) {
        CCLOG(u8"错误：在撤销时未能在模型中找到卡牌 %d。", state.id);
        return;
    }
    CardManager* cardManager = getCardManager(*cardToMove);
    if (!cardManager) {
        CCLOG(u8"错误：在撤销时未能找到卡牌 %d 的管理器。", state.id);
        return;
    }
    cardToMove->setPosition(state.position);
    cardToMove->setZone(state.zone);

    auto moveTo = MoveTo::create(kCardMoveDuration, state.position);
    cardManager->getView()->runAction(moveTo);
    cardManager->getView()->setLocalZOrder(0);

    CCLOG(u8"卡牌 %d 已恢复到原始位置 (%.1f, %.1f)，区域 %d。",
        state.id, state.position.x, state.position.y, static_cast<int>(state.zone));
}
CardManager* GameController::getCardManager(const CardModel& card) const {
    return CardIdManagerMap::getInstance().getCardManager(card._id);
}

void GameController::handleCardClicked(CardModel& card) {
    if (card.getZone() == CardZone::Hand) {
        return;
    }
    CardManager* cardManager = getCardManager(card);
    if (!cardManager) {
        CCLOG(u8"错误：处理卡牌 %d 点击时未能找到其管理器。", card._id);
        return;
    }
    card.setZone(CardZone::Hand);
    card.setPosition(kHandPosition);
    const float duration = 0.5f;              // 动画持续时间
    const float jumpHeight = 60.0f;           // 跳跃的高度，可以根据喜好调整
    const float rotationDegrees = 360.0f;     // 旋转角度，360度为一整圈
    // 创建跳跃动作 (JumpTo)
    auto jumpToAction = cocos2d::JumpTo::create(duration, kHandPosition, jumpHeight, 1);
    // 创建旋转动作 (RotateBy)
    auto rotateAction = cocos2d::RotateBy::create(duration, rotationDegrees);
    // 使用 Spawn 将跳跃和旋转组合起来，使它们同时执行
    auto combinedAction = cocos2d::Spawn::create(jumpToAction, rotateAction, nullptr);
    // 让卡牌视图执行这个组合后的动作
    cardManager->getView()->runAction(combinedAction);
    int newZOrder = 1;
    if (_undoManager.getUndoSize() > 0) {
        CardModel lastCard = getBottomCard();
        CardManager* lastCardManager = getCardManager(lastCard);
        if (lastCardManager && lastCardManager->getView()) {
            newZOrder = lastCardManager->getView()->getLocalZOrder() + 1;
        }
    }
    cardManager->getView()->setLocalZOrder(newZOrder);
}
void GameController::handleLabelClick() {
    undo();
}
// const 版本：用于只读操作
const CardModel* GameController::findCardInModel(int cardId) const {
    const auto& playfield = _gameModel.getPlayfield();
    for (const auto& card : playfield) {
        if (card._id == cardId) {
            return &card;
        }
    }
    const auto& stackfield = _gameModel.getStackfield();
    for (const auto& card : stackfield) {
        if (card._id == cardId) {
            return &card;
        }
    }
    return nullptr;
}

// 非 const 版本：用于写操作
CardModel* GameController::findCardInModel(int cardId) {
    // 调用 const 版本来查找
    const CardModel* const_card = static_cast<const GameController*>(this)->findCardInModel(cardId);
    // 使用 const_cast 去掉 const，以便进行修改
    // 这是在不修改 GameModel 接口的前提下，修复“修改副本”bug的必要步骤
    return const_cast<CardModel*>(const_card);
}
