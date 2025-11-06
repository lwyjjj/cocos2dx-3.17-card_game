// 维护游戏中两个核心区域的卡牌集合：
//- _playfield：主牌区卡牌列表，对应LevelConfig中的Playfield配置（）；
//- _stackfield：备用牌堆卡牌列表，对应LevelConfig中的Stack配置（）。

#pragma once
#include "cocos2d.h"
#include "CardModel.h"
#include "UndoModel.h"
#include "configs/models/LevelConfig.h"
#include "configs/loaders/LevelConfigLoader.h"
#include <vector>
USING_NS_CC;

class GameModel {
private:
    std::vector<CardModel> _playfield; // 游戏场地区域的卡片向量
    std::vector<CardModel> _stackfield; // 堆叠区域的卡片向量
    UndoModel _undoModel; // 用于记录撤销历史的对象
public:
    GameModel(LevelConfig* config) {
        if (config) {//初始化
            _stackfield = config->getStack();
            _undoModel.clearHistory();
            _playfield = config->getPlayfield();
        }
        else {
            //cout<<""<<endl;
        }
    }

    const std::vector<CardModel>& getPlayfield() const {return _playfield;}    // get playfield 
    const std::vector<CardModel>& getStackfield() const {return _stackfield;}    // get stackfield 
    UndoModel& getUndoModel() {return _undoModel;}//get UndoModel

    void addCardToPlayfield(const CardModel& card) {_playfield.push_back(card); }    // 向playfield添加卡片
    // 从playfield移除卡片
    void removeCardFromPlayfield(int id) {
        for (auto it = _playfield.begin(); it != _playfield.end(); it++) {
            if (it->_id == id) {
                _playfield.erase(it);
                break;
            }
        }
    }
    void addCardToStackfield(const CardModel& card) {_stackfield.push_back(card);}    // 向stackfield添加卡片
    // 从stackfield移除卡片
    void removeCardFromStackfield(int id) {
        for (auto it = _stackfield.begin(); it != _stackfield.end(); it++) {
            if (it->_id == id) {
                _stackfield.erase(it);
                break;
            }
        }
    }
};
