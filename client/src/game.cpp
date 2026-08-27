#include "game.h"
#include "raylib.h"

Game::Game()
{

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