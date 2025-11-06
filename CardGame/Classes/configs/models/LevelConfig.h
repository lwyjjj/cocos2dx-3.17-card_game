#pragma once
#include "cocos2d.h"
#include <vector>
#include "json/rapidjson.h"
#include "models/CardModel.h"
//存放卡牌的静态数据
using namespace std;
class LevelConfig{
private:
    LevelConfig() = default;
    ~LevelConfig() = default;
    vector<CardModel> _list_main_card;  // 主牌区卡牌配置（对应文档Playfield字段）
    vector<CardModel> _list_home_card;      // 备用牌堆配置（对应文档Stack字段）

    friend class LevelConfigLoader;  ///< 允许配置加载器访问私有成员
public:
    vector<CardModel> getPlayfield() {
        return _list_main_card;
    }
    vector<CardModel> getStack() {
        return _list_home_card;
    }
};
