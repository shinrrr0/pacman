#include "Game.hpp"

sf::Vector2i HelperClass::defineCellByCoords(float x, float y) {
    return sf::Vector2i(
        x = ((x + (float)TILE_SIDE_SIZE) / (float)TILE_SIDE_SIZE) - 1.f,
        y = ((y + (float)TILE_SIDE_SIZE) / (float)TILE_SIDE_SIZE) - 1.f);
}

MapObject* HelperClass::getCell(int x, int y) {
    return &(map[(y) * (GRID_SIDE_X + 2) + x]);
}

MapObject* HelperClass::getTopCell(MapObject* cell) {
    return getCell(cell->x, cell->y - 1);
};
MapObject* HelperClass::getRightCell(MapObject* cell) {
    return getCell(cell->x + 1, cell->y);
};
MapObject* HelperClass::getBottomCell(MapObject* cell) {
    return getCell(cell->x, cell->y + 1);
};
MapObject* HelperClass::getLeftCell(MapObject* cell) {
    return getCell(cell->x - 1, cell->y);
};

HelperClass::HelperClass() {
    this->playable_tiles.reserve(GRID_SIDE_X * GRID_SIDE_Y);
    this->border_tiles.reserve((2 * GRID_SIDE_X) + 4 + (2 * GRID_SIDE_Y));

    int number = 0;
    for (int i = 0; i < (GRID_SIDE_Y + 2); ++i) {
        for (int j = 0; j < (GRID_SIDE_X + 2); ++j) {
            int index = i * (GRID_SIDE_X + 2) + j;
            map[index].setPosition(j * TILE_SIDE_SIZE, i * TILE_SIDE_SIZE);
            map[index].y = i;
            map[index].x = j;
        }
    }

    //------------------------------------------------------------------------------------------
    for (int i = 1; i < GRID_SIDE_X + 1; ++i) {
        this->getCell(i, 1)->setTextureByPath("assets\\wall.png");
        this->getCell(i, 1)->can_walk_trough = false;
    }
    for (int i = 1; i < GRID_SIDE_X + 1; ++i) {
        this->getCell(i, GRID_SIDE_Y)->setTextureByPath("assets\\wall.png");
        this->getCell(i, GRID_SIDE_Y)->can_walk_trough = false;
    }
    for (int i = 1; i < (GRID_SIDE_Y / 2); ++i) {
        this->getCell(1, i)->setTextureByPath("assets\\wall.png");
        this->getCell(1, i)->can_walk_trough = false;
    }
    for (int i = (GRID_SIDE_Y / 2) + 2; i < GRID_SIDE_Y; ++i) {
        this->getCell(1, i)->setTextureByPath("assets\\wall.png");
        this->getCell(1, i)->can_walk_trough = false;
    }
    for (int i = 1; i < (GRID_SIDE_Y / 2); ++i) {
        this->getCell(GRID_SIDE_X, i)->setTextureByPath("assets\\wall.png");
        this->getCell(GRID_SIDE_X, i)->can_walk_trough = false;
    }
    for (int i = (GRID_SIDE_Y / 2) + 2; i < GRID_SIDE_Y; ++i) {
        this->getCell(GRID_SIDE_X, i)->setTextureByPath("assets\\wall.png");
        this->getCell(GRID_SIDE_X, i)->can_walk_trough = false;
    }
    for (int i = 3; i < GRID_SIDE_Y - 1; ++i) {
        this->getCell(4, i)->setTextureByPath("assets\\wall.png");
        this->getCell(4, i)->can_walk_trough = false;
    }
    for (int i = 3; i < GRID_SIDE_Y - 1; ++i) {
        this->getCell(GRID_SIDE_X - 3, i)->setTextureByPath("assets\\wall.png");
        this->getCell(GRID_SIDE_X - 3, i)->can_walk_trough = false;
    }
    //for (int i = 1; i < GRID_SIDE_X ; ++i) {
    //    this->getCell(i, 4)->setTextureByPath("assets\\wall.png");
    //    this->getCell(i, 4)->can_walk_trough = false;
    //}
    //for (int i = 2; i < GRID_SIDE_X + 1; ++i) {
    //    this->getCell(i, 7)->setTextureByPath("assets\\wall.png");
    //    this->getCell(i, 7)->can_walk_trough = false;
    //}
    //------------------------------------------------------------------------------------------

    for (int i = 0; i < (GRID_SIDE_X + 2); ++i) {
        this->map[i].setTextureByPath("assets\\empty.png");
        this->border_tiles.push_back(&map[i]);
        map[i].is_border_cell = true;
    }
    for (int i = (GRID_SIDE_X + 2) * (GRID_SIDE_Y + 1); i < (GRID_SIDE_X + 2) * (GRID_SIDE_Y + 2); ++i) {
        this->map[i].setTextureByPath("assets\\empty.png");
        this->border_tiles.push_back(&map[i]);
        map[i].is_border_cell = true;
    }
    for (int i = 1; i < (GRID_SIDE_Y + 1); ++i) {
        this->map[i * (GRID_SIDE_X + 2)].setTextureByPath("assets\\empty.png");
        this->border_tiles.push_back(&map[i * (GRID_SIDE_X + 2)]);
        map[i * (GRID_SIDE_X + 2)].is_border_cell = true;
    }
    for (int i = 2; i < GRID_SIDE_Y + 2; ++i) {
        this->map[i * (GRID_SIDE_X + 2) - 1].setTextureByPath("assets\\empty.png");
        this->border_tiles.push_back(&map[i * (GRID_SIDE_X + 2) - 1]);
        map[i * (GRID_SIDE_X + 2) - 1].is_border_cell = true;
    }

    int tile_n = 0;
    while (tile_n < GRID_SIDE_Y) {
        for (int j = 0; j < GRID_SIDE_X; j++) {
            int index = j + GRID_SIDE_X + 2 + 1 + (tile_n * (GRID_SIDE_X + 2));
            this->playable_tiles.push_back(&map[index]);
        }
        ++tile_n;
    }

    for (MapObject& cell : map) {
        if (cell.is_border_cell) {
            int index = 0;
            if ((cell.x == 0 || cell.x == GRID_SIDE_X + 1) && (cell.y == 0 || cell.y == GRID_SIDE_Y + 1)) {
                cell.connected_with.resize(0);
                continue;
            }
            if (cell.x == 0 || cell.x == GRID_SIDE_X + 1) {
                bool modifier = cell.x;
                number = GRID_SIDE_X - cell.x + (modifier * 2);
                if (getCell(number, cell.y)->can_walk_trough) {
                    cell.connected_with.push_back(getCell(number, cell.y));
                }
            }
            if (cell.y == 0 || cell.y == GRID_SIDE_Y + 1) {
                bool modifier = cell.y;
                number = GRID_SIDE_Y - cell.y + (modifier * 2);
                if (getCell(cell.x, number)->can_walk_trough) {
                    cell.connected_with.push_back(getCell(cell.x, number));
                }
            }
        }
        else {
            if (!cell.can_walk_trough) {
                cell.connected_with.resize(0);
                continue;
            }
            if (getTopCell(&cell)->can_walk_trough) {
                cell.connected_with.push_back(getTopCell(&cell));
            }
            if (getRightCell(&cell)->can_walk_trough) {
                cell.connected_with.push_back(getRightCell(&cell));
            }
            if (getBottomCell(&cell)->can_walk_trough) {
                cell.connected_with.push_back(getBottomCell(&cell));
            }
            if (getLeftCell(&cell)->can_walk_trough) {
                cell.connected_with.push_back(getLeftCell(&cell));
            }
        }
        cell.connected_with.resize(cell.connected_with.size());
    }
}