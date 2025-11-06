#pragma once
#include "configs/models/LevelConfig.h"
#include "models/CardModel.h"          
#include "json/document.h"         
#include <string> 
#include <vector>
using namespace rapidjson; 
class LevelConfigLoader  {
private:
    LevelConfigLoader() = default;
    ~LevelConfigLoader() = default;
    static int card_current_id;//每张card的id，用于标记
    static bool parseCardModel(const rapidjson::Value& cardNode, std::vector<CardModel>& target, CardZone zone);
public:
    static LevelConfig* LevelConfig_Begin(std::string fileName);//开始解析json文件

};
