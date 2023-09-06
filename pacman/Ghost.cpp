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
    sf::Vector2f difference = this->getPosition() - target_cell->getPosition();

    bool direction_conditions[4] {
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

bool Ghost::checkTargetCell() {
    if (!(this->*checkTargetFunc)()) return false;
    this->checkTargetFunc = &Ghost::pointerPlaceholder;
    this->setMovingDirectionToNone();
    this->setPosition(target_cell->getPosition());
    return true;
}

MapObject& Ghost::chooseNode(std::unordered_set<MapObject*> reachable, MapObject* goal_node) {
    int node_index = rand() % reachable.size();
    int counter = 0;
    for (MapObject* node : reachable) {
        if (counter == node_index) {
            return *node;
        }
        counter++;
    }
}

void Ghost::buildPath(MapObject* goal_node) {
    std::stack<MapObject*> path;
    while (goal_node != nullptr) {
        path.push(goal_node);
        goal_node = goal_node->previous;
    }
    path.pop();
    if(!path.empty()) this->setTargetCell(path.top());
}

void Ghost::findPath(MapObject* start_node, MapObject* goal_node) {
    std::unordered_set<MapObject*> reachable;
    std::unordered_set<MapObject*> new_reachable;
    std::unordered_set<MapObject*> explored;

    start_node->cost = 0;
    reachable.insert(start_node);
    
    MapObject* node;

    while (!reachable.empty()) {
        node = &chooseNode(reachable, goal_node);

        if (node == goal_node) {
            buildPath(goal_node);
            break;
        }
        reachable.erase(node);
        explored.insert(node);

        new_reachable.clear();

        for (MapObject* new_node : node->connected_with) {
            if (!helper->in(new_node, explored)) {
                new_reachable.insert(new_node);
            }
        }

        for (MapObject* adjacent : new_reachable) {
            if (!helper->in(adjacent, reachable)) {
                reachable.insert(adjacent);
            }
            if (node->cost + 1 < adjacent->cost) {
                adjacent->previous = node;
                adjacent->cost = node->cost + 1;
            }
        }
    }
    for (MapObject& node : helper->map) {
        node.previous = nullptr;
        node.cost = GRID_SIDE_X * GRID_SIDE_Y;
    }
};

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