#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"

struct TempPlayer {
    int Id;
    Rectangle dimension;
};
class Character {
    Rectangle dimension;
    Texture2D texture;
    public:
    Character(Rectangle dimension, const char * textureSrc);
    
    void updatePos(Rectangle dimension);
    void draw();
};

#endif