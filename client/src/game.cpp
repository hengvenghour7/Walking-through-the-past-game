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
        if (IsKeyDown(KEY_D))
        {
            allPlayers[playerId].dimension.x++;
            std::cout<< "update player " << allPlayers[playerId].dimension.x << std::fflush;
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
        // if (IsKeyDown(KEY_D))
        // {
        //     std::cout<< "draw test " << value.dimension.x << std::fflush;
        // }
    }
    if (IsKeyDown(KEY_D))
    {
        std::cout<< "size " << allPlayers.size() << " size --" << std::fflush;
    }
    // auto& targetDraw = allPlayers[playerId];
    // DrawRectangle(targetDraw.dimension.x, targetDraw.dimension.y, 40, 40, BLACK);
    //     if (IsKeyDown(KEY_D))
    //     {
    //         std::cout<< "draw test " << targetDraw.dimension.x << std::fflush;
    //     }
}
void Game::updatePlayerbyId(int Id, Rectangle dimension)
{
    std::cout << "UPDATE: id=" << Id
              << " x=" << dimension.x
              << std::endl;

    allPlayers[Id].dimension = dimension;

    std::cout << "STORED: id=" << Id
              << " x=" << allPlayers[Id].dimension.x
              << std::endl;
}