#ifndef SCHARACTER_H
#define SCHARACTER_H

#include "../helpers/dataTypeHelper.h"
#include <string>

class SCharacter {
    int id;
    SRectangle playerDimension;
    public:
    SCharacter(int id, SRectangle playerDimension);
    int getId();
    void updatePlayerDimension(SRectangle dimension);
    std::string getDataAsString();
};

#endif