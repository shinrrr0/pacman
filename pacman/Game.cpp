#pragma warning( disable : 4244 ) 
#include "Game.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <iterator>

// Game ----------------------------------------------------------------------------------------------------

Game::Game() {
    windowInit();
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
    this->player.helper = &(this->helper);
    this->player.defineCell();
    
    this->ghost.setTextureByPath("assets\\ghost.png");
    //this->ghost.setPosition((4 * TILE_SIDE_SIZE), (4 * TILE_SIDE_SIZE));
    //this->ghost.setPosition((10 * TILE_SIDE_SIZE), (3 * TILE_SIDE_SIZE));
    this->ghost.setPosition((10 * TILE_SIDE_SIZE), (9 * TILE_SIDE_SIZE));
    this->ghost.helper = &(this->helper);
    this->ghost.defineCell();
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
    for (MapObject* tile : helper.playable_tiles) {
        this->window.draw(*tile);
    }
}

void Game::drawBorderTiles() {
    for (MapObject* tile : helper.border_tiles) {
        this->window.draw(*tile);
    }
}