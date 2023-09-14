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
    this->player.setTextureByPath("assets\\f2r.png");
    this->player.setPosition((2 * TILE_SIDE_SIZE), (2 * TILE_SIDE_SIZE));
    this->player.speed = 20 * SPRITE_SCALE.x;
    this->player.helper = &(this->helper);
    this->player.defineCell();

    this->player.frames_number = 4;

    this->player.animation_sprites_u = new std::string[player.frames_number];
    this->player.animation_sprites_u[0] = "assets\\f1.png";
    this->player.animation_sprites_u[1] = "assets\\f2u.png";
    this->player.animation_sprites_u[2] = "assets\\f3u.png";
    this->player.animation_sprites_u[3] = "assets\\f2u.png";
    this->player.animations[0] = &player.animation_sprites_u;

    this->player.animation_sprites_r = new std::string[player.frames_number];
    this->player.animation_sprites_r[0] = "assets\\f1.png";
    this->player.animation_sprites_r[1] = "assets\\f2r.png";
    this->player.animation_sprites_r[2] = "assets\\f3r.png";
    this->player.animation_sprites_r[3] = "assets\\f2r.png";
    this->player.animations[1] = &player.animation_sprites_r;

    this->player.animation_sprites_b = new std::string[player.frames_number];
    this->player.animation_sprites_b[0] = "assets\\f1.png";
    this->player.animation_sprites_b[1] = "assets\\f2b.png";
    this->player.animation_sprites_b[2] = "assets\\f3b.png";
    this->player.animation_sprites_b[3] = "assets\\f2b.png";
    this->player.animations[2] = &player.animation_sprites_b;

    this->player.animation_sprites_l = new std::string[player.frames_number];
    this->player.animation_sprites_l[0] = "assets\\f1.png";
    this->player.animation_sprites_l[1] = "assets\\f2l.png";
    this->player.animation_sprites_l[2] = "assets\\f3l.png";
    this->player.animation_sprites_l[3] = "assets\\f2l.png";
    this->player.animations[3] = &player.animation_sprites_l;
    
    // Инициализация приведения
    this->ghost.setTextureByPath("assets\\ghost1r.png");
    //this->ghost.setPosition((4 * TILE_SIDE_SIZE), (4 * TILE_SIDE_SIZE));
    //this->ghost.setPosition((10 * TILE_SIDE_SIZE), (3 * TILE_SIDE_SIZE));
    this->ghost.setPosition((13 * TILE_SIDE_SIZE), (11 * TILE_SIDE_SIZE));
    this->ghost.speed = 15 * SPRITE_SCALE.x;
    this->ghost.helper = &(this->helper);
    this->ghost.defineCell();

    this->ghost.frames_number = 2;

    this->ghost.animation_sprites_u = new std::string[ghost.frames_number];
    this->ghost.animation_sprites_u[0] = "assets\\ghost1u.png";
    this->ghost.animation_sprites_u[1] = "assets\\ghost2u.png";
    this->ghost.animations[0] = &ghost.animation_sprites_u;

    this->ghost.animation_sprites_r = new std::string[ghost.frames_number];
    this->ghost.animation_sprites_r[0] = "assets\\ghost1r.png";
    this->ghost.animation_sprites_r[1] = "assets\\ghost2r.png";
    this->ghost.animations[1] = &ghost.animation_sprites_r;

    this->ghost.animation_sprites_b = new std::string[ghost.frames_number];
    this->ghost.animation_sprites_b[0] = "assets\\ghost1b.png";
    this->ghost.animation_sprites_b[1] = "assets\\ghost2b.png";
    this->ghost.animations[2] = &ghost.animation_sprites_b;

    this->ghost.animation_sprites_l = new std::string[ghost.frames_number];
    this->ghost.animation_sprites_l[0] = "assets\\ghost1l.png";
    this->ghost.animation_sprites_l[1] = "assets\\ghost2l.png";
    this->ghost.animations[3] = &ghost.animation_sprites_l;
}

// Инициализация текста
void Game::textInit() {
    this->font.loadFromFile("assets\\emulogic.ttf");
    this->text.setFont(this->font);
    this->text.setString("0 0");
    this->text.setCharacterSize(15 * SPRITE_SCALE.x);

    this->win_text.setFont(this->font);
    this->win_text.setString("YOU WIN!");
    this->win_text.setCharacterSize(16 * SPRITE_SCALE.x);
    this->win_text.setPosition((GRID_SIDE_X + 2) * TILE_SIDE_SIZE / 4, (GRID_SIDE_Y + 2) * TILE_SIDE_SIZE / 4);
 
    this->lose_text.setFont(this->font);
    this->lose_text.setString("YOU LOSE!");
    this->lose_text.setCharacterSize(16 * SPRITE_SCALE.x);
    this->lose_text.setPosition((GRID_SIDE_X + 2) * TILE_SIDE_SIZE / 4 - 30, (GRID_SIDE_Y + 2) * TILE_SIDE_SIZE / 4);

    this->exit_text.setFont(this->font);
    this->exit_text.setString("ESC to exit");
    this->exit_text.setCharacterSize(10 * SPRITE_SCALE.x);
    this->exit_text.setPosition((GRID_SIDE_X + 2) * TILE_SIDE_SIZE / 4 + 30, (GRID_SIDE_Y + 2) * TILE_SIDE_SIZE / 4 + 75);
}

void Game::updatePoints(MovableObject& obj) {
    obj.defineCell();
    if (!obj.current_cell->was_visited) {
        obj.current_cell->was_visited = true;
        obj.current_cell->setTextureByPath("assets\\empty.png");
        obj.points++;
    }
}

void Game::changeAnimationFrame(MovableObject& obj) {
    if (static_cast<int>(obj.direction) == 0) {
        return;
    }
    obj.setTextureByPath((*obj.animations[static_cast<int>(obj.direction)- 1])[obj.frame_index]);
    if (obj.frame_index == obj.frames_number - 1) {
        obj.frame_index = 0;
    }
    else {
        obj.frame_index++;
    }
}

// Обновление delta time
void Game::update() {
    this->delta_time = delta_clock.restart().asSeconds();
    this->timer += delta_time;
    if (timer >= 0.1) {
        changeAnimationFrame(player);
        changeAnimationFrame(ghost);
        timer = 0;
    }
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
            obj.setMovingDirectionToNone();
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