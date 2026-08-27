#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <vector>
#include <string>

class Game
{
    float timeGap;
    int playerId;
    Rectangle playerDim{10, 10, 20, 20};
    std::vector<Rectangle> allPlayers{};
    public:
    Game(int id);
    void tick(float deltaTime);
    void draw();
    void addPlayer(Rectangle player);
    std::string getPlayerDataAsString();
    Rectangle& getPlayerLocation();
};

#endif