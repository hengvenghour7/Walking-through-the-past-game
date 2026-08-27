#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"

class Character {
    Rectangle dimension;
    Texture2D texture;
    public:
    Character(Rectangle dimension, const char * textureSrc);
    void draw();
};

#endif