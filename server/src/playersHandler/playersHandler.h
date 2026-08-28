#include "../helpers/dataTypeHelper.h"
#include "SCharacter.h"
#include <vector>
#include <string>
#include <unordered_map>

class PlayersHandler {
    std::unordered_map<int, SCharacter> allPlayers;
    public:
    PlayersHandler();
    SCharacter* getPlayerPointerById(int id);
    void addNewPlayer(int* globalId);
    void updatePlayerById(int id, SRectangle dimension);
    std::unordered_map<int, SCharacter>& getAllPlayer();
    std::string getDataAsString();
    void setGlobalID(int* globalId);
};