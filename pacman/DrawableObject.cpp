#include "Game.hpp"

DrawableObject::DrawableObject() {
    setTextureByPath("assets\\empty.png");
}

void DrawableObject::setTextureByPath(std::string path) {
    if (!this->texture.loadFromFile(path)) {
        std::cerr << "texture loading error";
    }
    this->setTexture(texture, true);
    this->setScale(SPRITE_SCALE);
}