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



// �������, ������ - camelCase
// ����������, ���� - snake_case
// ������ - PascalCase
// ��������� - UPPER_CASE_SNAKE_CASE


const int TILE_SIDE_SIZE = 64; // ������ ������� ������ �������� ���� � ��������
const int TEXTURE_SIDE_SIZE = 16; // ������ ������� �������� �������� ���� � ��������
const int GRID_SIDE_X = 14; // ����� �������� ���� �� ��� X
const int GRID_SIDE_Y = 12; // ����� �������� ���� �� ��� Y
const sf::Vector2f SPRITE_SCALE{ float(TILE_SIDE_SIZE) / TEXTURE_SIDE_SIZE,
                                 float(TILE_SIDE_SIZE) / TEXTURE_SIDE_SIZE }; // ���������� ��������� ������� ��������
enum class Direction { None = 0, Up = 1, Right = 2, Down = 3, Left = 4 }; // ������������ ����������� ��������

// ������� ����� ��� ������ ������������� �������
class DrawableObject : public sf::Sprite {
private:
    sf::Texture texture; // ��������
public:
    void setTextureByPath(std::string path);
    DrawableObject();
};

// ����� ������� ������
class MapObject : public DrawableObject {
public:
    bool is_border_cell; // ����������� �� ������ �����
    bool can_walk_trough; // ����� �� ������ ����� ������
    std::vector<MapObject*> connected_with; // ������ ���������� �� �������� ������
    MapObject* previous; // ������ �� ����������� ������
    int cost; // ���� �������� �� ��� ������
    int x; // ������� � ����� ����� �� x
    int y; // ������� � ����� ����� �� y

    MapObject();
};

// ��������������� �����, ������� ������ � ����: ����� � �������������� �������
class HelperClass {
public:
    std::array<MapObject, (GRID_SIDE_X + 2)* (GRID_SIDE_Y + 2)> map{}; // ������ ���� ������
    std::vector<MapObject*> playable_tiles{}; // ������ ���������� �� ������� ������
    std::vector<MapObject*> border_tiles{}; // ������ ���������� �� ������ �����

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

// ������ ����������� ��������
class MovableObject : public DrawableObject {
public:
    float speed; // ���������� ��������
    sf::Vector2i normalized_moving_vector; // ��������������� ������ ��������
    Direction direction; // ����������� ��������
    MapObject* current_cell; // ��������� �� ������� ������
    HelperClass* helper; // ��������� �� ��������������� �����


    MovableObject();
    void changeDirectionByInput(sf::Keyboard::Key key_code);
    void changeDirection(Direction direction);
    void changeDirection(int direction);
    void setMovingDirectionToNone();
    void defineCell();
};

// ����� ����������
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

// ����� ����
class Game {
private:
    sf::Clock delta_clock;
    sf::Font font;

    void windowInit();
    void spritesInit();
    void textInit();

public:
    float delta_time;
    sf::RenderWindow window;
    HelperClass helper;
    MovableObject player;
    Ghost ghost;
    sf::Text text;

    Game();
    void update();
    void inputHandler(sf::Keyboard::Key key_code);
    void moveInCurrentDirection(MovableObject& obj);
    void checkCollisions(MovableObject& obj);
    void checkTransition(MovableObject& obj);
    void drawPlayableTiles();
    void drawBorderTiles();
};

#endif