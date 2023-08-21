#include "Game.hpp"

Ghost::Ghost() {
    this->target_cell = nullptr;
    this->checkTargetFunc = &Ghost::pointerPlaceholder;
    targetFunctions[0] = &Ghost::checkTargetCellOnTop;
    targetFunctions[1] = &Ghost::checkTargetCellOnRight;
    targetFunctions[2] = &Ghost::checkTargetCellOnBottom;
    targetFunctions[3] = &Ghost::checkTargetCellOnLeft;
}

void Ghost::setTargetCell(MapObject* target_cell) {
    target_cell->setTextureByPath("assets\\target.png");
    sf::Vector2f difference = this->getPosition() - target_cell->getPosition();

    bool direction_conditions[4]{
        difference.y > 0 && !difference.x,      // move up
        difference.x < 0 && !difference.y,      // move right
        difference.y < 0 && !difference.x,      // move down
        difference.x > 0 && !difference.y       // move left
    };
    for (int i = 0; i < 4; ++i) {
        if (direction_conditions[i]) {
            this->target_cell = target_cell;
            this->checkTargetFunc = targetFunctions[i];
            this->changeDirection(i + 1);
        }
    }
}

void Ghost::checkTargetCell() {
    if (!(this->*checkTargetFunc)()) return;
    this->checkTargetFunc = &Ghost::pointerPlaceholder;
    this->setMovingDirectionToNone();
    this->setPosition(target_cell->getPosition());
}

bool Ghost::checkTargetCellOnTop() {
    return static_cast<int>(this->getPosition().y) == static_cast<int>(target_cell->getPosition().y) || static_cast<int>(this->getPosition().y) < static_cast<int>(target_cell->getPosition().y);
}

bool Ghost::checkTargetCellOnRight() {
    return static_cast<int>(this->getPosition().x) == static_cast<int>(target_cell->getPosition().x) || static_cast<int>(this->getPosition().x) > static_cast<int>(target_cell->getPosition().x);
}

bool Ghost::checkTargetCellOnBottom() {
    return static_cast<int>(this->getPosition().y) == static_cast<int>(target_cell->getPosition().y) || static_cast<int>(this->getPosition().y) > static_cast<int>(target_cell->getPosition().y);
}

bool Ghost::checkTargetCellOnLeft() {
    return static_cast<int>(this->getPosition().x) == static_cast<int>(target_cell->getPosition().x) || static_cast<int>(this->getPosition().x) < static_cast<int>(target_cell->getPosition().x);
}

bool Ghost::pointerPlaceholder() { return false; }
