#include "Game.hpp"

// Конструктор MapObject
MapObject::MapObject() {
    this->is_border_cell = false;
    this->can_walk_trough = true;

    this->setTextureByPath("assets\\empty.png");
    this->connected_with.reserve(4);
    this->previous = nullptr;
    this->cost = GRID_SIDE_X * GRID_SIDE_Y;
    this->x = 0;
    this->y = 0;
}