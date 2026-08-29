#include "game.h"
#include <raylib.h>
#include <raymath.h>

#include <iostream>

Game::Game(int id): playerId(id), isUpdateFromTheServer(false)
{
    // allPlayers.push_back(playerDim);
}
void Game::tick(float deltaTime)
{
    BeginDrawing();
        ClearBackground(ORANGE);
        float speed = deltaTime * 300;
        auto& targetPlayer = allPlayers[playerId];
        if (IsKeyDown(KEY_D))
        {
            targetPlayer.dimension.x+=speed;
        }
        if (IsKeyDown(KEY_A))
        {
            targetPlayer.dimension.x-=speed;
        }
        if (IsKeyDown(KEY_W))
        {
            targetPlayer.dimension.y-=speed;
        }
        if (IsKeyDown(KEY_S))
        {
            targetPlayer.dimension.y+=speed;
        }
            for (auto& [key, value] : allPlayers)
            {
                Vector2 current = {
                        value.dimension.x,
                        value.dimension.y
                    };

                    if (Vector2Distance(current, value.targetPosition) < 1.0f)
                    {
                        value.isNeedUpdate = false;
                    }
                if (value.isNeedUpdate)
                {
                    float smoothing = 10.0f;
                    Vector2 tarPos = 
                            {
                                value.targetPosition.x - value.dimension.x, 
                                value.targetPosition.y - value.dimension.y
                            };
                    // Vector2 tarScale = Vector2Scale(tarPos, speed);
                    value.dimension.x += tarPos.x * smoothing * deltaTime;
                    value.dimension.y += tarPos.y * smoothing * deltaTime;
                }
            }
    draw();
    EndDrawing();
}
Rectangle& Game::getPlayerLocationById(int id)
{
    return allPlayers[id].dimension;
}
void Game::addPlayer(TempPlayer player)
{
    allPlayers[player.Id] = player;
}
std::string Game::getPlayerDataAsString()
{
    std::string dataString = std::to_string(playerId) + "," + std::to_string(playerDim.dimension.x) + "," + std::to_string(playerDim.dimension.y);
    return dataString;
}
void Game::draw()
{
    for (auto& [id, value]: allPlayers)
    {
        DrawRectangle(value.dimension.x, value.dimension.y, 40, 40, BLACK);
    }
    if (IsKeyDown(KEY_D))
    {
    }
}
void Game::updatePlayerbyId(int Id, Rectangle dimension)
{
    allPlayers[Id].targetPosition = {dimension.x, dimension.y};
}
void Game::setIsUpdateFromtheServer(bool isUpdate)
{
    for (auto& [key, value]: allPlayers)
    {
        value.isNeedUpdate = isUpdate;
    }
}