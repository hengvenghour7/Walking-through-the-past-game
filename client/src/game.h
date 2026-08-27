#ifndef GAME_H
#define GAME_H

#include <raylib.h>

class Game
{
    float timeGap;
    Rectangle playerDim{10, 10, 20, 20};
    public:
    Game();
    void tick(float deltaTime);
    void draw();
    Rectangle& getPlayerLocation();
};

#endif