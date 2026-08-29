#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "modules/character.h"

class Game
{
    float timeGap;
    int playerId;
    TempPlayer playerDim{1, Rectangle {50 , 10, 20, 20}};
    std::unordered_map<int, TempPlayer> allPlayers{};
    bool isUpdateFromTheServer;
    public:
    Game(int id);
    void tick(float deltaTime);
    void draw();
    void addPlayer(TempPlayer player);
    std::string getPlayerDataAsString();
    Rectangle& getPlayerLocationById(int id);
    void updatePos(std::vector<Rectangle>);
    void updatePlayerbyId(int Id, Rectangle dimension);
    void setIsUpdateFromtheServer(bool isUpdate);
};

#endif