#include "../helpers/dataTypeHelper.h"
#include "SCharacter.h"
#include <vector>
#include <string>

class PlayersHandler {
    std::vector<SCharacter> allPlayers;
    public:
    PlayersHandler();
    SCharacter* getPlayerPointerById(int id);
    void addNewPlayer(int* globalId);
    void updatePlayerById(int id);
    std::vector<SCharacter>& getAllPlayer();
    std::string getDataAsString();
    void setGlobalID(int* globalId);
};