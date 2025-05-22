#include "Obstacle.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<RoomLayout> ObstacleManager::loadLayoutsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<RoomLayout> layouts;
    RoomLayout currentLayout;
    layouts.clear();
    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t\n\r"));
        line.erase(line.find_last_not_of(" \t\n\r") + 1);

        if (line.empty()) continue;

        if (line.rfind("STT", 0) == 0) {
            if (!currentLayout.obstacles.empty()) {
                layouts.push_back(currentLayout);
                currentLayout.obstacles.clear();
            }
        }
        else {
            std::istringstream iss(line);
            int x, y, w, h;
            std::string type;
            if (iss >> x >> y >> w >> h >> type) {
                currentLayout.obstacles.push_back({ x, y, w, h, type });
            }
        }
    }

    if (!currentLayout.obstacles.empty()) {
        layouts.push_back(currentLayout);
    }
    return layouts;
}
