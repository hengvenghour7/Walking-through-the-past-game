#include "playersHandler.h"

PlayersHandler::PlayersHandler(): allPlayers{}
{

}
SCharacter* PlayersHandler::getPlayerPointerById(int id)
{
    for (SCharacter& player: allPlayers)
    {
        if (player.getId() == id) return &player;
    }
    return nullptr;
}
std::vector<SCharacter>& PlayersHandler::getAllPlayer()
{
    return allPlayers;
}
void PlayersHandler::addNewPlayer(int* globalId)
{
    if (allPlayers.size() <= 0)
    {
        SCharacter newPlayer(*globalId, SRectangle{100 , 100, 40, 40});
        allPlayers.push_back(newPlayer);
        setGlobalID(globalId);
        return;
    }
    int len = allPlayers.size();
    int lastId = allPlayers[len - 1].getId();
    SCharacter newPlayer(lastId + 1, SRectangle{100 + (lastId + 1) * 20, 100, 40, 40});
    allPlayers.push_back(newPlayer);
}
std::string PlayersHandler::getDataAsString()
{
    std::string dataString = "";
    for (SCharacter& player: allPlayers)
    {
        dataString += player.getDataAsString() + ",";
    }
    return dataString;
}
void PlayersHandler::setGlobalID(int* globalId)
{
    int lastPlayerIndex = allPlayers.size();
    if (lastPlayerIndex == 0)
    {
        *globalId += 1;
    }
    *globalId = allPlayers[lastPlayerIndex - 1].getId() + 1;
}