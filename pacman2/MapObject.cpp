#include "Game.hpp"

MapObject::MapObject() {
    this->is_border_cell = false;
    this->can_walk_trough = true;
    this->setTextureByPath("assets\\empty.png");
    this->x = 0;
    this->y = 0;
}