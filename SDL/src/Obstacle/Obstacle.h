#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <string>
#include <vector>

struct Obstacle {
    int x, y;     
    int w, h;      
    std::string type; 
};

struct RoomLayout {
    std::vector<Obstacle> obstacles;
};

class ObstacleManager {
public:
    static std::vector<RoomLayout> loadLayoutsFromFile(const std::string& filename);


private:
    
};

#endif // OBSTACLE_H
