#pragma warning( disable : 4244 ) 
#include "Game.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <iterator>

// Game ----------------------------------------------------------------------------------------------------

Game::Game() {
    windowInit();
    mapInit();
    textInit();
    spritesInit();
}

void Game::windowInit() {
    //this->window.create(sf::VideoMode((GRID_SIDE_X + 2) * TILE_SIDE_SIZE, (GRID_SIDE_Y + 2) * TILE_SIDE_SIZE), L"Новый проект", sf::Style::None);
    this->window.create(sf::VideoMode((GRID_SIDE_X + 2) * TILE_SIDE_SIZE, (GRID_SIDE_Y + 2) * TILE_SIDE_SIZE), L"Новый проект");
}

void Game::spritesInit() {
    this->player.setTextureByPath("assets\\f.png");
    this->player.setPosition((2 * TILE_SIDE_SIZE), (2 * TILE_SIDE_SIZE));
    this->player.map = &map;
    this->player.defineCell();
    
    this->ghost.setTextureByPath("assets\\ghost.png");
    this->ghost.setPosition((4 * TILE_SIDE_SIZE), (4 * TILE_SIDE_SIZE));
    this->ghost.map = &map;
    this->ghost.defineCell();
}

void Game::mapInit() {
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
    //------------------------------------------------------------------------------------------

    for (int i = 0; i < (GRID_SIDE_X + 2); ++i) {
        this->map[i].setTextureByPath("assets\\empty1.png");
        this->border_tiles.push_back(&map[i]);
        map[i].is_border_cell = true;
    }
    for (int i = (GRID_SIDE_X + 2) * (GRID_SIDE_Y + 1); i < (GRID_SIDE_X + 2) * (GRID_SIDE_Y + 2); ++i) {
        this->map[i].setTextureByPath("assets\\empty1.png");
        this->border_tiles.push_back(&map[i]);
        map[i].is_border_cell = true;
    }
    for (int i = 1; i < (GRID_SIDE_Y + 1); ++i) {
        this->map[i * (GRID_SIDE_X + 2)].setTextureByPath("assets\\empty1.png");
        this->border_tiles.push_back(&map[i * (GRID_SIDE_X + 2)]);
        map[i * (GRID_SIDE_X + 2)].is_border_cell = true;
    }
    for (int i = 2; i < GRID_SIDE_Y + 2; ++i) {
        this->map[i * (GRID_SIDE_X + 2) - 1].setTextureByPath("assets\\empty1.png");
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

void Game::textInit() {
    this->font.loadFromFile("assets\\emulogic.ttf");
    this->text.setFont(this->font);
    this->text.setString("0 0");
    this->text.setCharacterSize(35 * SPRITE_SCALE.x);
}

void Game::update() {
    this->delta_time = delta_clock.restart().asSeconds();
}

void Game::inputHandler(sf::Keyboard::Key key_code) {
    switch (key_code) {
    case sf::Keyboard::Escape:
        this->window.close();
        break;
    default:
        this->player.changeDirectionByInput(key_code);
        break;
    }
}

void Game::moveInCurrentDirection(MovableObject& obj) {
    obj.move(
        obj.normalized_moving_vector.x * PLAYER_SPEED_COEF * delta_time,
        obj.normalized_moving_vector.y * PLAYER_SPEED_COEF * delta_time);
    obj.defineCell();
}

sf::Vector2i Game::defineCellByCoords(float x, float y) {
    return sf::Vector2i(
        x = ((x + (float)TILE_SIDE_SIZE) / (float)TILE_SIDE_SIZE) - 1.f,
        y = ((y + (float)TILE_SIDE_SIZE) / (float)TILE_SIDE_SIZE) - 1.f);
}

MapObject* Game::getCell(int x, int y) {
    return &(map[(y) * (GRID_SIDE_X + 2) + x]);
}

MapObject* Game::getTopCell(MapObject* cell) {
    return getCell(cell->x, cell->y - 1);
};
MapObject* Game::getRightCell(MapObject* cell) {
    return getCell(cell->x + 1, cell->y);
};
MapObject* Game::getBottomCell(MapObject* cell) {
    return getCell(cell->x, cell->y + 1);
};
MapObject* Game::getLeftCell(MapObject* cell) {
    return getCell(cell->x - 1, cell->y);
};

void Game::checkCollisions(MovableObject& obj) {
    MapObject* cells_to_check_collisions[8]{
        getCell(obj.current_cell->x, obj.current_cell->y - 1),
        getCell(obj.current_cell->x + 1, obj.current_cell->y - 1),
        getCell(obj.current_cell->x + 1, obj.current_cell->y),
        getCell(obj.current_cell->x + 1, obj.current_cell->y + 1),
        getCell(obj.current_cell->x, obj.current_cell->y + 1),
        getCell(obj.current_cell->x - 1, obj.current_cell->y + 1),
        getCell(obj.current_cell->x - 1, obj.current_cell->y),
        getCell(obj.current_cell->x - 1, obj.current_cell->y - 1),
    };

    for (MapObject* cell : cells_to_check_collisions) {
        if (obj.getGlobalBounds().intersects(cell->getGlobalBounds()) && !cell->can_walk_trough) {
            if (obj.direction == Direction::Up || obj.direction == Direction::Down) obj.setPosition(obj.getPosition().x, obj.current_cell->y * TILE_SIDE_SIZE);
            else obj.setPosition(obj.current_cell->x * TILE_SIDE_SIZE, obj.getPosition().y);
        }
    }
}

void Game::checkTransition(MovableObject& obj) {
    if (obj.current_cell->x == 0 || obj.current_cell->x == GRID_SIDE_X + 1) {
        bool modifier = obj.current_cell->x;
        obj.setPosition((GRID_SIDE_X - obj.current_cell->x + (modifier * 2)) * TILE_SIDE_SIZE - ((obj.normalized_moving_vector.x * TILE_SIDE_SIZE / 2) - obj.normalized_moving_vector.x), obj.getPosition().y);
        obj.defineCell();
    }
    if (obj.current_cell->y == 0 || obj.current_cell->y == GRID_SIDE_Y + 1) {
        bool modifier = obj.current_cell->y;
        obj.setPosition(obj.getPosition().x, (GRID_SIDE_Y - obj.current_cell->y + (modifier * 2)) * TILE_SIDE_SIZE - ((obj.normalized_moving_vector.y * TILE_SIDE_SIZE / 2) - obj.normalized_moving_vector.y));
        obj.defineCell();
    }
}

void Game::drawPlayableTiles() {
    for (MapObject* tile : playable_tiles) {
        this->window.draw(*tile);
    }
}

void Game::drawBorderTiles() {
    for (MapObject* tile : border_tiles) {
        this->window.draw(*tile);
    }
}