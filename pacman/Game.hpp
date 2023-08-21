#pragma warning( disable : 4244 )
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <array>
#include <vector>

#ifndef __GAME_HPP__
#define __GAME_HPP__ 



// ������� - camelCase
// ���� - snake_case
// ������, ��������� � �.� - PascalCase
// ��������� - UPPER_CASE_SNAKE_CASE


const int TILE_SIDE_SIZE = 40;      // - ������ � ��������, ������� ����� ����� ���� ������ 
const int TEXTURE_SIDE_SIZE = 40;   // - �������� ������ ������� �������� � ��������
const int GRID_SIDE_X = 10;         // - ���������� ������� ������ �� �����������
const int GRID_SIDE_Y = 10;         // - ���������� ������� ������ �� ���������
const sf::Vector2f SPRITE_SCALE{ float(TILE_SIDE_SIZE) / TEXTURE_SIDE_SIZE, float(TILE_SIDE_SIZE) / TEXTURE_SIDE_SIZE }; // ��������� ������ ��� ��������������� ������� ��� ����� ������
const int PLAYER_SPEED_COEF = 50 * SPRITE_SCALE.x; // - �������� ������, �������������� �� ������ ������
enum class Direction { None = 0, Up = 1, Right = 2, Down = 3, Left = 4 };

class DrawableObject : public sf::Sprite {
private:
    sf::Texture texture;
public:
    void setTextureByPath(std::string path);
    DrawableObject();
    DrawableObject(std::string texture_path, sf::Vector2f position = sf::Vector2f(0, 0)); // ��������� ����������� ��� bg, ����� ����� �������
};

class MapObject : public DrawableObject {
public:
    bool is_transition_cell;
    bool is_border_cell;
    bool can_walk_trough;
    std::vector<MapObject*> connected_with;
    int x;
    int y;

    MapObject();
};

class MovableObject : public DrawableObject {
public:
    sf::Vector2i normalized_moving_vector;
    Direction direction;
    MapObject* current_cell;
    std::array<MapObject, (GRID_SIDE_X + 2)* (GRID_SIDE_Y + 2)>* map;

    MovableObject();
    void changeDirectionByInput(sf::Keyboard::Key key_code);
    void changeDirection(Direction direction);
    void changeDirection(int direction);
    void setMovingDirectionToNone();
    void defineCell();
};

class Ghost : public MovableObject {
private:
    MapObject* target_cell;
    bool (Ghost::* checkTargetFunc)();
    bool (Ghost::* targetFunctions[4])(void);
    bool checkTargetCellOnTop();
    bool checkTargetCellOnRight();
    bool checkTargetCellOnBottom();
    bool checkTargetCellOnLeft();
    bool pointerPlaceholder();


public:
    Ghost();
    void setTargetCell(MapObject* target_cell);
    void checkTargetCell();
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
    MovableObject player;
    Ghost ghost;
    std::array<MapObject, (GRID_SIDE_X + 2) * (GRID_SIDE_Y + 2)> map{};
    std::vector<MapObject*> playable_tiles{};
    std::vector<MapObject*> border_tiles{};
    sf::Text text;

    Game();
    void update();
    void inputHandler(sf::Keyboard::Key key_code);
    void moveInCurrentDirection(MovableObject& obj);
    //-------------------------------------------------------------------
    // ���� ���� ����� ����� ������� � ��������� ��������������� �����
    sf::Vector2i defineCellByCoords(float x, float y);
    MapObject* getCell(int x, int y); // - �� ����������� ������
    MapObject* getTopCell(MapObject* cell);
    MapObject* getRightCell(MapObject* cell);
    MapObject* getBottomCell(MapObject* cell);
    MapObject* getLeftCell(MapObject* cell);
    //-------------------------------------------------------------------
    void checkCollisions(MovableObject& obj);
    void checkTransition(MovableObject& obj);
    void drawPlayableTiles();
    void drawBorderTiles();
};

#endif