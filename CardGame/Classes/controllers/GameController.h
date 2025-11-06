#pragma once
//ÓÎÏ·¿ØÖÆÆ÷Àà
#include "models/GameModel.h"
#include "managers/UndoManager.h"
#include "managers/CardManager.h"
#include <vector>

class CardManager;
class GameController {
private:
    CardManager* getCardManager(const CardModel& card) const;
    const CardModel* findCardInModel(int cardId) const;
    GameModel _gameModel;
    UndoManager _undoManager;
    CardModel getBottomCard();
    bool isCardMatch(const CardModel& card1, const CardModel& card2) const;
    void moveCardToOriginalPosition(const UndoCardState& state);
    CardModel* findCardInModel(int cardId);
public:
    explicit GameController(GameModel gameModel);
    ~GameController();
    bool selectCardFromPlayefieldAndMatch(CardModel& selectedCard);
    void clickStackCard(CardModel& card);
    void handleCardClicked(CardModel& card);
    bool undo();
    void handleLabelClick();
};
