#ifndef RANDOM_MAP_H
#define RANDOM_MAP_H

#include <iostream>
#include <vector>
#include <map>
#include <string>

const int ROOM_SIZE = 11;
const int ROOM_SIZE_X = 11;
const int ROOM_SIZE_Y = 9;
const int TILE_WALL = 1;
const int TILE_FLOOR = 2;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Room {
    int id;
    int x, y;
    int layout[ROOM_SIZE][ROOM_SIZE];
    std::map<Direction, bool> doors;
};

class RandomMap {
    private:
        std::vector<Room> roomList;
        int MAX_ROOMS = 10;
        std::map<Direction, Direction> opposite;

        Room generateRoomWithDoors(std::map<Direction, bool> doors, int gridX, int gridY, int id);
        bool isRoomOccupied(int x, int y) const;
        std::vector<Direction> getAvailableDoors(const Room& room) const;
        void finalizeRooms();
        void closeUnlinkedDoors();
        void saveCombinedMapToFile(const std::string& filename) const;

    public:
        RandomMap();
        void generateRooms();
        const std::vector<Room>& getRoomList() const;
        void setRoomList(const std::vector<Room>& rooms);
};

#endif // RANDOM_MAP_H
