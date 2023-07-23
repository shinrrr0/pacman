#pragma warning( disable : 4244 )
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <array>
#include <vector>

#ifndef __GAME_HPP__
#define __GAME_HPP__ 

// функции - camelCase
// поля - snake_case
// классы, структуры и т.п - PascalCase
// константы - UPPER_CASE_SNAKE_CASE


const int TILE_SIDE_SIZE = 40;      // - размер в пикселях, который будет иметь одна клетка 
const int TEXTURE_SIDE_SIZE = 40;   // - реальный размер стороны текстуры в пикселях
const int GRID_SIDE_X = 10;         // - количество игровых клеток по горизонтали
const int GRID_SIDE_Y = 10;         // - количество игровых клеток по вертикали
const sf::Vector2f SPRITE_SCALE{ float(TILE_SIDE_SIZE) / TEXTURE_SIDE_SIZE, float(TILE_SIDE_SIZE) / TEXTURE_SIDE_SIZE }; // константа нужная для масштабирования текстур под разер клеток
const int PLAYER_SPEED_COEF = 50 * SPRITE_SCALE.x; // - скорость игрока, масштабируется от общего скейла
enum class Direction { None = 0, Up = 1, Right = 2, Down = 3, Left = 4 };

class DrawableObject : public sf::Sprite {
private:
    sf::Texture texture;
public:
    void setTextureByPath(std::string path);
    DrawableObject();
    DrawableObject(std::string texture_path, sf::Vector2f position = sf::Vector2f(0, 0)); // временный конструктор для bg, нужно будет удалить
};

class MapObject : public DrawableObject {
public:
    bool can_walk_trough;
    int x;
    int y;

    MapObject();
};

class MovableObject : public DrawableObject {
public:
    sf::Vector2i normalized_moving_vector;
    Direction direction;
    MapObject* current_cell;

    MovableObject();
    void changeMovingDirection(sf::Keyboard::Key key_code);
    void setMovingDirectionToNone();
    void defineCell();
};

class Game {
private:
    sf::Clock delta_clock;
    sf::Font font;

    void windowInit();
    void spritesInit();
    void mapInit();
    void textInit();

public:
    float delta_time;
    sf::RenderWindow window;
    static inline MovableObject player;
    static inline std::array<MapObject, (GRID_SIDE_X + 2)* (GRID_SIDE_Y + 2)> map{};
    std::vector<MapObject*> playable_tiles{};
    std::vector<MapObject*> border_tiles{};
    sf::Text text;

    Game();
    void update();
    void inputHandler(sf::Keyboard::Key key_code);
    void moveInCurrentDirection(MovableObject& obj);
    //-------------------------------------------------------------------
    // этот блок нужно будет вынести в отдельный вспомогательный класс
    static sf::Vector2i defineCellByCords(float x, float y);
    static MapObject* getCell(int x, int y); // - по координатам клетки
    //-------------------------------------------------------------------
    void checkCollisions(MovableObject& obj);
    void checkTransition(MovableObject& obj);
    void drawPlayableTiles();
    void drawBorderTiles();


};

#endif