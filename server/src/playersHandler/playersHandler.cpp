#include "playersHandler.h"

PlayersHandler::PlayersHandler(): allPlayers{}
{

}
SCharacter* PlayersHandler::getPlayerPointerById(int id)
{
    for (auto& [id, player]: allPlayers)
    {
        if (player.getId() == id) return &player;
    }
    return nullptr;
}
std::unordered_map<int, SCharacter>& PlayersHandler::getAllPlayer()
{
    return allPlayers;
}
void PlayersHandler::addNewPlayer(int* globalId)
{
    int len = allPlayers.size();
    // int lastId = *globalId;
    SCharacter newPlayer(*globalId, SRectangle{100 + (*globalId) * 20, 100, 40, 40});
    allPlayers.emplace(*globalId, newPlayer);
    (*globalId)++;
}
std::string PlayersHandler::getDataAsString()
{
    std::string dataString = "";
    for (auto& [id, value]: allPlayers)
    {
        dataString += value.getDataAsString() + ",";
    }
    return dataString;
}
void PlayersHandler::setGlobalID(int* globalId)
{
    
}
void PlayersHandler::updatePlayerById(int id, SRectangle dimension)
{
    auto it = allPlayers.find(id);
    if (it != allPlayers.end())
    {
        it->second.updatePlayerDimension(dimension);
    }
}