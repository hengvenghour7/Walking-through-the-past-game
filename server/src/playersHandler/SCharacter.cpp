#include "SCharacter.h"

SCharacter::SCharacter(int id, SRectangle playerDimension): id(id), playerDimension(playerDimension) {}
int SCharacter::getId()
{
    return id;
}
std::string SCharacter::getDataAsString()
{
    std::string dataString = std::to_string(id) + "," + std::to_string(playerDimension.x) + "," + std::to_string(playerDimension.y);
    return dataString;
}
void SCharacter::updatePlayerDimension(SRectangle dimension) {
    playerDimension = dimension;
}