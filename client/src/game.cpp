#include "game.h"
#include "raylib.h"

Game::Game(int id): playerId(id)
{
    allPlayers.push_back(playerDim);
}
void Game::tick(float deltaTime)
{
    BeginDrawing();
        ClearBackground(ORANGE);
        DrawRectangle(playerDim.x, playerDim.y, playerDim.width, playerDim.height, BLACK);
        if (IsKeyDown(KEY_D))
        {
            playerDim.x++;
        }
        EndDrawing();
}
Rectangle& Game::getPlayerLocation()
{
    return playerDim; 
}
void Game::addPlayer(Rectangle player)
{
    allPlayers.push_back(player);
}
std::string Game::getPlayerDataAsString()
{
    std::string dataString = std::to_string(playerId) + "," + std::to_string(playerDim.x) + "," + std::to_string(playerDim.y);
    return dataString;
}