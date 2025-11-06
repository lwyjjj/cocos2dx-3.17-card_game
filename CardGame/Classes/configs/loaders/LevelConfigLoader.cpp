#include "LevelConfigLoader.h"
//载入资源
int LevelConfigLoader::card_current_id = 0;
bool LevelConfigLoader::parseCardModel(const rapidjson::Value& cardNode, std::vector<CardModel>& target, CardZone zone) {
    if (cardNode.IsObject() == false) return false; 
    if (cardNode.HasMember("CardFace") == false) return false;
    if (cardNode["CardFace"].IsInt() == false) return false;
    if (cardNode.HasMember("CardSuit") == false) return false;
    if (cardNode["CardSuit"].IsInt() == false)  return false;
    if (cardNode.HasMember("Position") == false) return false;
    if (cardNode["Position"].IsObject() == false) return false;

    const rapidjson::Value& posNode = cardNode["Position"];

    if (posNode.HasMember("x") == false) return false;
    if (posNode["x"].IsInt() == false) return false;
    if (posNode.HasMember("y") == false)  return false;
    if (posNode["y"].IsInt() == false) return false;

    const int faceInt = cardNode["CardFace"].GetInt();
    const int suitInt = cardNode["CardSuit"].GetInt();
    const float x = posNode["x"].GetFloat();
    const float y = posNode["y"].GetFloat();
    if (faceInt < 0|| faceInt > 12|| suitInt < 0|| suitInt > 3) {
        return false;
    }

    // 整数转换成定义类型
   const CardFaceType face = static_cast<CardFaceType>(faceInt);
   const CardSuitType suit = static_cast<CardSuitType>(suitInt);
    // 创建坐标对象
    cocos2d::Vec2 pos(x, y);

    // 根据卡牌所在的区域（zone），给坐标加上偏移量
    if (zone == CardZone::Stack) {
        // 在备用牌堆，向右下移动
        pos = pos + cocos2d::Vec2(300, 400);
    }
    else {
        // 在主玩区，向上移动
        pos = pos + cocos2d::Vec2(0, 550);
    }
    const int id = card_current_id++;    // 获取唯一ID
    CardModel newCard(face, suit, pos, id, zone);
    target.push_back(newCard);    // 添加到列表vector的末尾
    return true;
}
LevelConfig* LevelConfigLoader::LevelConfig_Begin(std::string fileName) {
    //读取文件内容
    std::string jsonStr = cocos2d::FileUtils::getInstance()->getStringFromFile(fileName);
    // 检查是否为空
    if (jsonStr.empty()) {
        CCLOG("LevelConfigLoader: 文件读取失败，或者文件内容为空: %s", fileName.c_str());
        return nullptr;
    }
    // 解析 JSON 字符串
    rapidjson::Document d;
    d.Parse<rapidjson::kParseDefaultFlags>(jsonStr.c_str());

    if (d.HasParseError()) {
        // 报错
        CCLOG("LevelConfigLoader: JSON 解析出错! 错误信息: %s\n", d.GetParseError());
        return nullptr;
    }
    // 配置对象
    const auto config = new LevelConfig();

    // 检查 JSON 的最外层是不是一个对象
    if (d.IsObject() == false) {
        CCLOG("LevelConfigLoader: JSON文件的根节点不是一个对象 (不是以 {} 开头和结尾).");
        delete config;
        return nullptr;
    }
    // 全局的卡牌ID计数器重置为0。
    card_current_id = 0;
    // 解析 "Stack" 区域的卡牌
    if (d.HasMember("Stack")) {
        if (d["Stack"].IsArray()) {
            const rapidjson::Value& stackArray = d["Stack"];
            for (SizeType i = 0; i < stackArray.Size(); ++i) {
                const rapidjson::Value& cardNode = stackArray[i];
                const bool parseSuccess = parseCardModel(cardNode, config->_list_home_card, CardZone::Stack);
            }
        }
    }
    // 解析 "Playfield" 区域的卡牌
    if (d.HasMember("Playfield")) {
        if (d["Playfield"].IsArray()) {
            const rapidjson::Value& playfieldArray = d["Playfield"];
            for (SizeType i = 0; i < playfieldArray.Size(); ++i) {
                const rapidjson::Value& cardNode = playfieldArray[i];
                const bool parseSuccess = parseCardModel(cardNode, config->_list_main_card, CardZone::Playfield);
            }
        }
    }
    return config;
}