#pragma warning( disable : 4244 ) 
#include "Game.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <iterator>

// DrawableObject ------------------------------------------------------------------------------------------

DrawableObject::DrawableObject() {
    // Размер ректа у текстуры равен размеру первоначальной текстуры, поэтому невозможно впоследствии поменять размер текстуры
    setTextureByPath("assets\\placeholder.png");
}

void DrawableObject::setTextureByPath(std::string path) {
    if (!this->texture.loadFromFile(path)) {
        std::cerr << "texture loading error";
    }
    this->setTexture(texture, true);
    this->setScale(SPRITE_SCALE);
}

// MovableObject -------------------------------------------------------------------------------------------

MovableObject::MovableObject() : DrawableObject() {
    this->setMovingDirectionToNone();
}

void MovableObject::changeMovingDirection(sf::Keyboard::Key key_code) {
    switch (key_code) {
    case sf::Keyboard::Up:
        this->direction = Direction::Up;
        this->normalized_moving_vector.x = 0;
        this->normalized_moving_vector.y = -1;
        break;
    case sf::Keyboard::Right:
        this->direction = Direction::Right;
        this->normalized_moving_vector.x = 1;
        this->normalized_moving_vector.y = 0;
        break;
    case sf::Keyboard::Down:
        this->direction = Direction::Down;
        this->normalized_moving_vector.x = 0;
        this->normalized_moving_vector.y = 1;
        break;
    case sf::Keyboard::Left:
        this->direction = Direction::Left;
        this->normalized_moving_vector.x = -1;
        this->normalized_moving_vector.y = 0;
        break;
    case sf::Keyboard::Space: // временно - удалить
        this->setMovingDirectionToNone();
        break;
    }
}

void MovableObject::setMovingDirectionToNone() {
    this->direction = Direction::None;
    this->normalized_moving_vector.x = 0;
    this->normalized_moving_vector.y = 0;
}

void MovableObject::defineCell() {
    int x = (((this->getPosition().x) + TILE_SIDE_SIZE / 2 + TILE_SIDE_SIZE) / TILE_SIDE_SIZE);
    int y = (((this->getPosition().y) + TILE_SIDE_SIZE / 2 + TILE_SIDE_SIZE) / TILE_SIDE_SIZE);
    current_cell = Game::getCell(x - 1, y - 1);
}

// MapObject -----------------------------------------------------------------------------------------------

MapObject::MapObject() {
    this->can_walk_trough = true;
    this->setTextureByPath("assets\\empty.png");
}

// Game ----------------------------------------------------------------------------------------------------

Game::Game() {
    spritesInit();
    mapInit();
    windowInit();
    textInit();
}

void Game::windowInit() {
    this->window.create(sf::VideoMode((GRID_SIDE_X + 2) * TILE_SIDE_SIZE, (GRID_SIDE_Y + 2) * TILE_SIDE_SIZE), L"Новый проект", sf::Style::None);
}

void Game::spritesInit() {
    this->player.setTextureByPath("assets\\f.png");
    this->player.setPosition((2 * TILE_SIDE_SIZE), (2 * TILE_SIDE_SIZE));
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

    this->getCell(1, 1)->setTextureByPath("assets\\wall.png");
    this->getCell(1, 1)->can_walk_trough = false;
    this->getCell(2, 1)->setTextureByPath("assets\\wall.png");
    this->getCell(2, 1)->can_walk_trough = false;
    this->getCell(3, 1)->setTextureByPath("assets\\wall.png");
    this->getCell(3, 1)->can_walk_trough = false;
    this->getCell(2, 3)->setTextureByPath("assets\\wall.png");
    this->getCell(2, 3)->can_walk_trough = false;
    this->getCell(4, 1)->setTextureByPath("assets\\wall.png");
    this->getCell(4, 1)->can_walk_trough = false;
    this->getCell(4, 2)->setTextureByPath("assets\\wall.png");
    this->getCell(4, 2)->can_walk_trough = false;
    this->getCell(4, 3)->setTextureByPath("assets\\wall.png");
    this->getCell(4, 3)->can_walk_trough = false;
    this->getCell(4, 4)->setTextureByPath("assets\\wall.png");
    this->getCell(4, 4)->can_walk_trough = false;

    for (int i = 0; i < (GRID_SIDE_X + 2); ++i) {
        this->map[i].setTextureByPath("assets\\empty1.png");
        this->border_tiles.push_back(&map[i]);
    }
    for (int i = (GRID_SIDE_X + 2) * (GRID_SIDE_Y + 1); i < (GRID_SIDE_X + 2) * (GRID_SIDE_Y + 2); ++i) {
        this->map[i].setTextureByPath("assets\\empty1.png");
        this->border_tiles.push_back(&map[i]);
    }
    for (int i = 1; i < (GRID_SIDE_Y + 1); ++i) {
        this->map[i * (GRID_SIDE_X + 2)].setTextureByPath("assets\\empty1.png");
        this->border_tiles.push_back(&map[i * (GRID_SIDE_X + 2)]);
    }
    for (int i = 2; i < GRID_SIDE_Y + 2; ++i) {
        this->map[i * (GRID_SIDE_X + 2) - 1].setTextureByPath("assets\\empty1.png");
        this->border_tiles.push_back(&map[i * (GRID_SIDE_X + 2) - 1]);
    }

    int tile_n = 0;
    while (tile_n < GRID_SIDE_Y) {
        for (int j = 0; j < GRID_SIDE_X; j++) {
            int index = j + GRID_SIDE_X + 2 + 1 + (tile_n * (GRID_SIDE_X + 2));
            this->playable_tiles.push_back(&map[index]);
        }
        ++tile_n;
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
    case sf::Keyboard::Escape: this->window.close(); break;
    default: this->player.changeMovingDirection(key_code); break;
    }
}

void Game::moveInCurrentDirection(MovableObject& obj) {
    obj.move(
        obj.normalized_moving_vector.x * PLAYER_SPEED_COEF * delta_time,
        obj.normalized_moving_vector.y * PLAYER_SPEED_COEF * delta_time);
    obj.defineCell();
}

sf::Vector2i Game::defineCellByCords(float x, float y) {
    return sf::Vector2i(
        x = ((x + (float)TILE_SIDE_SIZE) / (float)TILE_SIDE_SIZE) - 1.f,
        y = ((y + (float)TILE_SIDE_SIZE) / (float)TILE_SIDE_SIZE) - 1.f);
}

MapObject* Game::getCell(int x, int y) {
    //if ((x < 1 || x > GRID_SIDE_X) || (y < 1 || y > GRID_SIDE_Y)) {
    //    throw "Got index out of range while executing getCell()";
    //}
    return &(map[(y) * (GRID_SIDE_X + 2) + x]);
}

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
        bool modifiactor = obj.current_cell->x;
        obj.setPosition((GRID_SIDE_X - obj.current_cell->x + (modifiactor * 2)) * TILE_SIDE_SIZE - ((obj.normalized_moving_vector.x * TILE_SIDE_SIZE / 2) - obj.normalized_moving_vector.x), obj.getPosition().y);
        obj.defineCell();
    }
    if (obj.current_cell->y == 0 || obj.current_cell->y == GRID_SIDE_Y + 1) {
        bool modifiactor = obj.current_cell->y;
        obj.setPosition(obj.getPosition().x, (GRID_SIDE_Y - obj.current_cell->y + (modifiactor * 2)) * TILE_SIDE_SIZE - ((obj.normalized_moving_vector.y * TILE_SIDE_SIZE / 2) - obj.normalized_moving_vector.y));
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