#include "game.h"
#include "raylib.h"

#include <iostream>

Game::Game(int id): playerId(id)
{
    // allPlayers.push_back(playerDim);
}
void Game::tick(float deltaTime)
{
    BeginDrawing();
        ClearBackground(ORANGE);
        float speed = deltaTime * 150;
        if (IsKeyDown(KEY_D))
        {
            allPlayers[playerId].dimension.x+=speed;
        }
        if (IsKeyDown(KEY_A))
        {
            allPlayers[playerId].dimension.x-=speed;
        }
        if (IsKeyDown(KEY_W))
        {
            allPlayers[playerId].dimension.y-=speed;
        }
        if (IsKeyDown(KEY_S))
        {
            allPlayers[playerId].dimension.y+=speed;
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
    allPlayers[Id].dimension = dimension;
}