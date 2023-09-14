#pragma warning( disable : 4244 )
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <unordered_set>
#include <stack>

#ifndef __GAME_HPP__
#define __GAME_HPP__ 



// Функции, методы - camelCase
// Переменные, поля - snake_case
// Классы - PascalCase
// Константы - UPPER_CASE_SNAKE_CASE


const int TILE_SIDE_SIZE = 64; // Размер стороны клетки игрового поля в пикселях
const int TEXTURE_SIDE_SIZE = 16; // Размер стороны текстуры игрового поля в пикселях
const int GRID_SIDE_X = 14; // Длина игрового поля по оси X
const int GRID_SIDE_Y = 12; // Длина игрового поля по оси Y
const sf::Vector2f SPRITE_SCALE{ float(TILE_SIDE_SIZE) / TEXTURE_SIDE_SIZE,
                                 float(TILE_SIDE_SIZE) / TEXTURE_SIDE_SIZE }; // Коэффицент изменения размера текстуры
enum class Direction { None = 0, Up = 1, Right = 2, Down = 3, Left = 4 }; // Перечисление направлений движения

// Базовый класс для любого отображаемого объекта
class DrawableObject : public sf::Sprite {
private:
    sf::Texture texture; // Текстура
public:
    void setTextureByPath(std::string path);
    DrawableObject();
};

// Класс игровой клетки
class MapObject : public DrawableObject {
public:
    bool is_border_cell; // Принадлежит ли клетка рамке
    bool can_walk_trough; // Можно ли пройти через клетку
    std::vector<MapObject*> connected_with; // Вектор указателей на соседние клетки
    MapObject* previous; // Ссылка на предыдующую клетку
    int cost; // Цена перехода на эту клетку
    int x; // Позиция в общей сетке по x
    int y; // Позиция в общей сетке по y

    MapObject();
};

// Вспомагательный класс, который хранит в себе: карту и вспомгательные функции
class HelperClass {
public:
    std::array<MapObject, (GRID_SIDE_X + 2)* (GRID_SIDE_Y + 2)> map{}; // Массив всех клеток
    std::vector<MapObject*> playable_tiles{}; // Вектор указателей на игровые клетки
    std::vector<MapObject*> border_tiles{}; // Вектор указателей на клетки рамки

    sf::Vector2i defineCellByCoords(float x, float y);
    MapObject* getCell(int x, int y);
    MapObject* getTopCell(MapObject* cell);
    MapObject* getRightCell(MapObject* cell);
    MapObject* getBottomCell(MapObject* cell);
    MapObject* getLeftCell(MapObject* cell);
    template<typename val, typename container>
    bool in(val elem, container list) {
        return list.find(elem) != list.end();
    }

    HelperClass();
};

// Классы двигающихся объектов
class MovableObject : public DrawableObject {
public:
    float speed; // Коэффицент скорости
    sf::Vector2i normalized_moving_vector; // Нормализованный вектор движения
    Direction direction; // Направление движения
    MapObject* current_cell; // Указатель на текущую клетку
    HelperClass* helper; // Указатель на вспомагательный класс
    
    int frames_number;
    int frame_index;
    std::string** animations[4];
    std::string* animation_sprites_u;
    std::string* animation_sprites_r;
    std::string* animation_sprites_b;
    std::string* animation_sprites_l;

    MovableObject();
    void changeDirectionByInput(sf::Keyboard::Key key_code);
    void changeDirection(Direction direction);
    void changeDirection(int direction);
    void setMovingDirectionToNone();
    void defineCell();
};

// Класс приведения
class Ghost : public MovableObject {
private:
    MapObject* target_cell;

    bool (Ghost::* checkTargetFunc)();
    bool (Ghost::* targetFunctions[4])();
    bool checkTargetCellOnTop();
    bool checkTargetCellOnRight();
    bool checkTargetCellOnBottom();
    bool checkTargetCellOnLeft();
    bool pointerPlaceholder();
    MapObject& chooseNode(std::unordered_set<MapObject*> reachable, MapObject* goal_node);
    void buildPath(MapObject* goal_node);
public:
   
    void findPath(MapObject* start_cell, MapObject* goal_cell);
    
    Ghost();
    void setTargetCell(MapObject* target_cell);
    bool checkTargetCell();
};

// Класс игры
class Game {
private:
    sf::Clock delta_clock;
    sf::Font font;

    void windowInit();
    void spritesInit();
    void textInit();

public:
    float delta_time;
    float timer;
    sf::RenderWindow window;
    HelperClass helper;
    MovableObject player;
    Ghost ghost;
    sf::Text text;

    Game();
    void changeAnimationFrame(MovableObject& obj);
    void update();
    void inputHandler(sf::Keyboard::Key key_code);
    void moveInCurrentDirection(MovableObject& obj);
    void checkCollisions(MovableObject& obj);
    void checkTransition(MovableObject& obj);
    void drawPlayableTiles();
    void drawBorderTiles();
};

#endif