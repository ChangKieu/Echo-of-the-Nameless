// GameData.h
#pragma once
#include <vector>
#include "Map/RandomMap.h"  

class GameData {
public:
    static GameData& GetInstance() {
        static GameData instance;
        return instance;
    }

    std::vector<Room> roomList;
    bool isRetry = false;

    GameData(GameData const&) = delete;
    void operator=(GameData const&) = delete;

    bool isBgmOn = true;
    bool isSfxOn = true;

private:
    GameData() = default;
};
