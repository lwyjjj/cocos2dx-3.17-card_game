#pragma once
#include "models/GameModel.h"
#include "views/GameView.h"
#include "configs/models/LevelConfig.h"  
#include <vector>

USING_NS_CC;
//服务层：将静态关卡配置（LevelConfig）转换为运行时游戏模型（GameModel）

class GameModelFromLevelGenerator {
public:
    static GameModel generateGameModel(const std::string levelFile) {
        auto config = LevelConfigLoader::LevelConfig_Begin("level_1.json");
        GameModel gameModel(config);
        return gameModel;
    }
    static void generateGameView(GameModel& gameModel, Node* parent) {
        auto gameView = GameView::create(gameModel);
        if (gameView) {
            parent->addChild(gameView, 1); // 将 GameView 添加到场景中
        }
    }
private:

    GameModelFromLevelGenerator() = default;
};
