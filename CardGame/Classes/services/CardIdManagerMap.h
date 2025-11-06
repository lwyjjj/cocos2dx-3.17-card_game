#pragma once
#include "managers/CardManager.h"
#include <unordered_map>
#include <mutex>
// ID 与管理器的全局映射服务（单例）
class CardIdManagerMap final {
public:
    //获取全局唯一实例（线程安全）
    static CardIdManagerMap& getInstance() noexcept {
        static CardIdManagerMap instance;
        return instance;
    }
    void addCardManager(int cardId, CardManager* manager) {
        if (manager == nullptr) {
            CCLOG(u8"[警告] CardIdManagerMap::addCardManager - 尝试注册空指针，已忽略。");
            return;
        }

        std::lock_guard<std::mutex> lock(_mutex);
        _cardIdManagerMap[cardId] = manager;
    }
    //param cardId 卡牌唯一 ID
    [[nodiscard]] CardManager* getCardManager(int cardId) noexcept {
        std::lock_guard<std::mutex> lock(_mutex);
        const auto it = _cardIdManagerMap.find(cardId);
        return (it != _cardIdManagerMap.end()) ? it->second : nullptr;
    }
    void removeCardManager(int cardId) noexcept {
        std::lock_guard<std::mutex> lock(_mutex);
        _cardIdManagerMap.erase(cardId);
    }

private:
    // 私有构造与析构，防止外部实例化
    CardIdManagerMap() = default;
    ~CardIdManagerMap() = default;
    // 禁止拷贝与赋值
    CardIdManagerMap(const CardIdManagerMap&) = delete;
    CardIdManagerMap& operator=(const CardIdManagerMap&) = delete;
private:
    std::unordered_map<int, CardManager*> _cardIdManagerMap; // 卡牌ID→管理器映射
    mutable std::mutex _mutex; // 保证多线程安全
};
