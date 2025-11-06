#pragma once
//record记录卡牌操作前的状态

#include "cocos2d.h"
#include "CardModel.h"  

USING_NS_CC;
struct UndoCardState {
    int id;
    Vec2 position;       // 操作前的位置
    CardZone zone;
};

class UndoModel {
private:
    std::vector<UndoCardState> _card_history;  // 操作历史（按时间顺序存储，回退时逆序弹出）
public:
    UndoModel() = default;
    bool canUndo() const { return !_card_history.empty(); }
    int getSize() const { return _card_history.size(); }
    void record(const UndoCardState& state) { _card_history.push_back(state); }//记录
    void UndoModel::clearHistory() { _card_history.clear(); } // 清空
    bool UndoModel::undo(UndoCardState& outState) {//进行回退
        if (_card_history.empty()) {
            return false;  
        }
        else {
            outState = _card_history.back();
            _card_history.pop_back();
            return true;
        }

    }
};
