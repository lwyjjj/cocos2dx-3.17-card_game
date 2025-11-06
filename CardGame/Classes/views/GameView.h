#pragma once
// 游戏的主视图层，负责渲染所有游戏元素并处理用户输入。
#include "cocos2d.h"
#include <vector>
#include <memory> 

// 前向声明
class GameModel;
class CardView;
class GameController;
class GameView : public cocos2d::Node {
public:
    //静态工厂方法，用于创建 GameView 实例。
    static GameView* create(GameModel& model);
    ~GameView() override;

protected:
    bool init(GameModel& model);

private:
    void generateCardViews(GameModel& model);
    void setupUI();
    void registerTouchEvents();
    void onLabelClicked();
    std::vector<CardView*> _playfieldCardViews;
    std::vector<CardView*> _stackfieldCardViews;
    cocos2d::Label* _statusLabel = nullptr;
    std::unique_ptr<GameController> _gameController;
};
