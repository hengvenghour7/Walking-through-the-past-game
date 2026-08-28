#include "character.h"

Character::Character(Rectangle dimension, const char * textureSrc): texture(LoadTexture(textureSrc))
{

}
void Character::updatePos(Rectangle dimension)
{
    this->dimension = dimension;
}