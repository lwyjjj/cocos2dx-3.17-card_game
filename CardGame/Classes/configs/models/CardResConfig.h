#pragma once
//展示卡牌具体模样
#include <string>
#include <array>
#include "models/CardModel.h" 
class CardResConfig {
public:
    //每张卡牌的背景
    static std::string getBackGround() {
        return "res/card_general.png";
    }
    //卡牌花色
    static std::string getSuitRes(CardSuitType suit) {
        constexpr std::array<const char*, 4> suitPaths = {
            "res/suits/club.png",    // CST_CLUBS
            "res/suits/diamond.png", // CST_DIAMONDS
            "res/suits/heart.png",   // CST_HEARTS
            "res/suits/spade.png"    // CST_SPADES
        };

        const int index = static_cast<int>(suit);
        //花色是否合规
        if (index >= 0 && index < suitPaths.size()) {
            return suitPaths[index];
        }
        return ""; // 非法符号
    }
    //卡牌左上角数字获取
    static std::string getSmallNumberRes(CardSuitType suit, CardFaceType face) {
        return getNumberResByColorAndFace(suit, face, "small");
    }

    //卡牌中间大数字获取
    static std::string getBigNumberRes(CardSuitType suit, CardFaceType face) {
        return getNumberResByColorAndFace(suit, face, "big");
    }
private:
    //根据尺寸big还是small获取路径
    static std::string getNumberResByColorAndFace(CardSuitType suit, CardFaceType face, const std::string& sizePrefix) {
        std::string color = suitToColor(suit);
        std::string faceStr = faceToString(face);
        if (color.empty() || faceStr.empty()) {//非法参数
            return "";
        }
        return "res/number/" + sizePrefix + "_" + color + "_" + faceStr + ".png";        // 统一路径拼接逻辑
    }
    //数字转换
    static std::string faceToString(CardFaceType face) {
        constexpr std::array<const char*, 13> faceNames = {
            "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"
        };
        int index = static_cast<int>(face);
        // 校验枚举范围
        if (index >= 0 && index < faceNames.size()) {
            return faceNames[index];
        }
        else {
            return ""; // 非法符号
        }
    }
    //判断颜色是黑色还是红色
    static std::string suitToColor(CardSuitType suit) {
        if (suit == CardSuitType::CST_CLUBS || suit == CardSuitType::CST_SPADES) {
            return "black";
        }
        else if (suit == CardSuitType::CST_DIAMONDS|| suit == CardSuitType::CST_HEARTS) {
            return "red";
        }
        else {
            return "";
        }
    }
};
