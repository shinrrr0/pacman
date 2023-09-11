#include "Game.hpp"


// Конструктор DrawableObject
DrawableObject::DrawableObject() {
    setTextureByPath("assets\\empty.png");
}

// Назначение объекту текстуры по пути файла текстуры
void DrawableObject::setTextureByPath(std::string path) {
    if (!this->texture.loadFromFile(path)) {
        std::cerr << "texture loading error";
    }
    this->setTexture(texture, true);
    this->setScale(SPRITE_SCALE);
}