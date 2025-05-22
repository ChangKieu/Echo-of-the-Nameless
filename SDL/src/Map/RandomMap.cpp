#include "RandomMap.h"
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <climits>

RandomMap::RandomMap() {
    opposite = {
        {UP, DOWN}, {DOWN, UP}, {LEFT, RIGHT}, {RIGHT, LEFT}
    };
}

bool RandomMap::isRoomOccupied(int x, int y) const {
    for (const auto& r : roomList)
        if (r.x == x && r.y == y)
            return true;
    return false;
}

std::vector<Direction> RandomMap::getAvailableDoors(const Room& room) const {
    std::vector<Direction> dirs;
    for (const auto& [dir, hasDoor] : room.doors)
        if (hasDoor) dirs.push_back(dir);
    return dirs;
}

//0 - sky
//1- wall
//2 - floor
//3 - pos character
//4 - final room
//5 - center room
//6 - pos torch
//7 - pos key

Room RandomMap::generateRoomWithDoors(std::map<Direction, bool> doors, int gridX, int gridY, int id) {
    Room room;
    room.id = id;
    room.x = gridX;
    room.y = gridY;
    room.doors = doors;

    for (int i = 0; i < ROOM_SIZE_Y; i++)
        for (int j = 0; j < ROOM_SIZE_X; j++)
            room.layout[i][j] = TILE_WALL;

    for (int i = 1; i < ROOM_SIZE_Y - 1; i++)
        for (int j = 1; j < ROOM_SIZE_X - 1; j++)
            room.layout[i][j] = TILE_FLOOR;

    int centerX = ROOM_SIZE_X / 2;
    int centerY = ROOM_SIZE_Y / 2;
    room.layout[centerY][centerX] = 5;

    if (doors[UP]) {
        for (int i = -1; i <= 1; ++i)
            room.layout[0][centerX + i] = TILE_FLOOR;
        room.layout[0][centerX - 2] = 6;
        room.layout[0][centerX + 2] = 6;
    }
    if (doors[DOWN]) {
        for (int i = -1; i <= 1; ++i)
            room.layout[ROOM_SIZE_Y - 1][centerX + i] = TILE_FLOOR;
        room.layout[ROOM_SIZE_Y - 1][centerX - 2] = 6;
        room.layout[ROOM_SIZE_Y - 1][centerX + 2] = 6;
    }
    if (doors[LEFT]) {
        for (int i = -1; i <= 1; ++i)
            room.layout[centerY + i][0] = TILE_FLOOR;
        room.layout[centerY - 2][0] = 6;
        room.layout[centerY + 2][0] = 6;
    }
    if (doors[RIGHT]) {
        for (int i = -1; i <= 1; ++i)
            room.layout[centerY + i][ROOM_SIZE_X - 1] = TILE_FLOOR;
        room.layout[centerY - 2][ROOM_SIZE_X - 1] = 6;
        room.layout[centerY + 2][ROOM_SIZE_X - 1] = 6;
    }

    return room;
}

void RandomMap::finalizeRooms() {
    for (auto& room : roomList) {
        int centerX = ROOM_SIZE_X / 2;
        int centerY = ROOM_SIZE_Y / 2;
        if (!room.doors[UP]) {
            for (int i = -1; i <= 1; ++i)
                room.layout[0][centerX + i] = TILE_WALL;
        }
        if (!room.doors[DOWN]) {
            for (int i = -1; i <= 1; ++i)
                room.layout[ROOM_SIZE_Y - 1][centerX + i] = TILE_WALL;
        }
        if (!room.doors[LEFT]) {
            for (int i = -1; i <= 1; ++i)
                room.layout[centerY + i][0] = TILE_WALL;
        }
        if (!room.doors[RIGHT]) {
            for (int i = -1; i <= 1; ++i)
                room.layout[centerY + i][ROOM_SIZE_X - 1] = TILE_WALL;
        }
    }
}

void RandomMap::closeUnlinkedDoors() {
    for (auto& room : roomList) {
        for (auto& [dir, hasDoor] : room.doors) {
            if (hasDoor) {
                int newX = room.x, newY = room.y;
                if (dir == UP) newY--;
                else if (dir == DOWN) newY++;
                else if (dir == LEFT) newX--;
                else if (dir == RIGHT) newX++;

                if (!isRoomOccupied(newX, newY)) {
                    room.doors[dir] = false;
                    int centerX = ROOM_SIZE_X / 2;
                    int centerY = ROOM_SIZE_Y / 2;
                    if (dir == UP) {
                        for (int i = -1; i <= 1; ++i)
                            room.layout[0][centerX + i] = TILE_WALL;
                    }
                    if (dir == DOWN) {
                        for (int i = -1; i <= 1; ++i)
                            room.layout[ROOM_SIZE_Y - 1][centerX + i] = TILE_WALL;
                    }
                    if (dir == LEFT) {
                        for (int i = -1; i <= 1; ++i)
                            room.layout[centerY + i][0] = TILE_WALL;
                    }
                    if (dir == RIGHT) {
                        for (int i = -1; i <= 1; ++i)
                            room.layout[centerY + i][ROOM_SIZE_X - 1] = TILE_WALL;
                    }
                }
            }
        }
    }
}

void RandomMap::saveCombinedMapToFile(const std::string& filename) const {
    int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;
    for (const auto& room : roomList) {
        minX = std::min(minX, room.x);
        minY = std::min(minY, room.y);
        maxX = std::max(maxX, room.x);
        maxY = std::max(maxY, room.y);
    }

    int gridW = maxX - minX + 1;
    int gridH = maxY - minY + 1;
    int tileW = gridW * ROOM_SIZE_X;
    int tileH = gridH * ROOM_SIZE_Y;

    std::vector<std::vector<int>> bigMap(tileH, std::vector<int>(tileW, 0));

    for (const auto& room : roomList) {
        int offsetX = (room.x - minX) * ROOM_SIZE_X;
        int offsetY = (room.y - minY) * ROOM_SIZE_Y;

        for (int i = 0; i < ROOM_SIZE_Y; ++i)
            for (int j = 0; j < ROOM_SIZE_X; ++j)
                bigMap[offsetY + i][offsetX + j] = room.layout[i][j];
    }

    std::ofstream out(filename);
    for (const auto& row : bigMap) {
        for (int val : row) {
            out << val << ' ';
        }
        out << "\n";
    }
    out.close();

}

void RandomMap::generateRooms() {
    roomList.clear();
	MAX_ROOMS = 10 + rand() % 8;
    int center = 0;
    std::map<Direction, bool> startDoors = { {UP, true}, {DOWN, true}, {LEFT, true}, {RIGHT, true} };
    Room startRoom = generateRoomWithDoors(startDoors, center, center, 0);
    roomList.push_back(startRoom);
    roomList[0].layout[ROOM_SIZE_Y / 2][ROOM_SIZE_X / 2] = 3;
    int currentRooms = 1;

    while (currentRooms < MAX_ROOMS) {
        std::vector<Room> toProcess = roomList;

        for (Room& baseRoom : toProcess) {
            std::vector<Direction> dirs = getAvailableDoors(baseRoom);
            for (Direction dir : dirs) {
                int newX = baseRoom.x;
                int newY = baseRoom.y;

                if (dir == UP) newY--;
                else if (dir == DOWN) newY++;
                else if (dir == LEFT) newX--;
                else if (dir == RIGHT) newX++;

                if (isRoomOccupied(newX, newY)) continue;
                if (currentRooms >= MAX_ROOMS) break;

                std::map<Direction, bool> newDoors = {
                    {UP, false}, {DOWN, false}, {LEFT, false}, {RIGHT, false}
                };
                newDoors[opposite[dir]] = true;

                for (int d = 0; d < 4; d++) {
                    if (d == opposite[dir]) continue;
                    newDoors[(Direction)d] = (rand() % 2 == 0);
                }

                Room newRoom = generateRoomWithDoors(newDoors, newX, newY, currentRooms);
                roomList.push_back(newRoom);
                baseRoom.doors[dir] = false;
                currentRooms++;
            }
        }
    }

    finalizeRooms();
    closeUnlinkedDoors();

    if (!roomList.empty()) {
        Room& lastRoom = roomList.back();
        lastRoom.layout[ROOM_SIZE_Y / 2][ROOM_SIZE_X / 2] = 4;

        int randomIndex = rand() % (roomList.size() - 2) + 1;
        Room& keyRoom = roomList[randomIndex];
        keyRoom.layout[ROOM_SIZE_Y / 2][ROOM_SIZE_X / 2] = 7;
    }
    
    saveCombinedMapToFile("random_map.txt");
}

const std::vector<Room>& RandomMap::getRoomList() const {
    return roomList;
}

void RandomMap::setRoomList(const std::vector<Room>& rooms) {
    roomList = rooms;
}