#ifndef CARD_MODEL_H_
#define CARD_MODEL_H_
#include "cocos2d.h"
USING_NS_CC;
enum class CardZone {
    Playfield,  // 主牌区
    Stack,      // 备用牌堆
    Hand,
    OutSide
};
enum class CardSuitType {
    CST_NONE = -1,
    CST_CLUBS,      // 梅花
    CST_DIAMONDS,   // 方块
    CST_HEARTS,     // 红桃
    CST_SPADES,     // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};
//点数
enum class CardFaceType {
    CFT_NONE = -1,
    CFT_ACE,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};
class CardModel {
private:
    CardFaceType _face;
    CardSuitType _suit;
    CardZone _zone;
    cocos2d::Vec2 _position;
public:
    CardModel(CardFaceType face,CardSuitType suit,const cocos2d::Vec2& position,int id = -1, CardZone zone = CardZone::OutSide) 
    {
        _face = face;
        _suit = suit;
        _position = position;
        _id = id;
        _zone = zone;
    }
    CardFaceType getFace()const{ return _face;}
    CardSuitType getSuit()const {return _suit; }
    const cocos2d::Vec2& getPosition() const { return _position; }
    const CardZone getZone() const { return _zone; }
    void setZone(CardZone zone) { _zone = zone; }
    void setPosition(Vec2 position) { _position = position; }
    int _id;
};

#endif // CARD_MODEL_H_