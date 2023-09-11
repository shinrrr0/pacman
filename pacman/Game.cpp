#pragma warning( disable : 4244 ) 
#include "Game.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <iterator>

// Game ----------------------------------------------------------------------------------------------------

//Конструктор для основополагающего класса Game
Game::Game() {
    windowInit();
    textInit();
    spritesInit();
}

//Инициализация окна при запуске
void Game::windowInit() {
    //this->window.create(sf::VideoMode((GRID_SIDE_X + 2) * TILE_SIDE_SIZE, (GRID_SIDE_Y + 2) * TILE_SIDE_SIZE), L"Новый проект", sf::Style::None);
    this->window.create(sf::VideoMode((GRID_SIDE_X + 2) * TILE_SIDE_SIZE, (GRID_SIDE_Y + 2) * TILE_SIDE_SIZE), L"Pacman");
}

// Инициализация объектов
void Game::spritesInit() {
    // Инициализация игрока
    this->player.setTextureByPath("assets\\f.png");
    this->player.setPosition((2 * TILE_SIDE_SIZE), (9 * TILE_SIDE_SIZE));
    this->player.speed = 20 * SPRITE_SCALE.x;
    this->player.helper = &(this->helper);
    this->player.defineCell();
    
    // Инициализация приведения
    this->ghost.setTextureByPath("assets\\ghost.png");
    //this->ghost.setPosition((4 * TILE_SIDE_SIZE), (4 * TILE_SIDE_SIZE));
    //this->ghost.setPosition((10 * TILE_SIDE_SIZE), (3 * TILE_SIDE_SIZE));
    this->ghost.setPosition((9 * TILE_SIDE_SIZE), (9 * TILE_SIDE_SIZE));
    this->ghost.speed = 15 * SPRITE_SCALE.x;
    this->ghost.helper = &(this->helper);
    this->ghost.defineCell();
}

// Инициализация текста
void Game::textInit() {
    this->font.loadFromFile("assets\\emulogic.ttf");
    this->text.setFont(this->font);
    this->text.setString("0 0");
    this->text.setCharacterSize(15 * SPRITE_SCALE.x);
}

// Обновление delta time
void Game::update() {
    this->delta_time = delta_clock.restart().asSeconds();
}

// Обработчик ввода
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
 // Перемещение объекта
void Game::moveInCurrentDirection(MovableObject& obj) {
    obj.move(
        obj.normalized_moving_vector.x * obj.speed * delta_time,
        obj.normalized_moving_vector.y * obj.speed * delta_time);
    obj.defineCell();
}

// Обработка столкновений со стенами
void Game::checkCollisions(MovableObject& obj) {
    MapObject* cells_to_check_collisions[8]{
        helper.getCell(obj.current_cell->x, obj.current_cell->y - 1),
        helper.getCell(obj.current_cell->x + 1, obj.current_cell->y - 1),
        helper.getCell(obj.current_cell->x + 1, obj.current_cell->y),
        helper.getCell(obj.current_cell->x + 1, obj.current_cell->y + 1),
        helper.getCell(obj.current_cell->x, obj.current_cell->y + 1),
        helper.getCell(obj.current_cell->x - 1, obj.current_cell->y + 1),
        helper.getCell(obj.current_cell->x - 1, obj.current_cell->y),
        helper.getCell(obj.current_cell->x - 1, obj.current_cell->y - 1),
    };

    for (MapObject* cell : cells_to_check_collisions) {
        if (obj.getGlobalBounds().intersects(cell->getGlobalBounds()) && !cell->can_walk_trough) {
            if (obj.direction == Direction::Up || obj.direction == Direction::Down) obj.setPosition(obj.getPosition().x, obj.current_cell->y * TILE_SIDE_SIZE);
            else obj.setPosition(obj.current_cell->x * TILE_SIDE_SIZE, obj.getPosition().y);
        }
    }
}

// Обработка перехода через границу экрана
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

// Рендер игрового поля
void Game::drawPlayableTiles() {
    for (MapObject* tile : helper.playable_tiles) {
        this->window.draw(*tile);
    }
}

// Рендер рамки
void Game::drawBorderTiles() {
    for (MapObject* tile : helper.border_tiles) {
        this->window.draw(*tile);
    }
}