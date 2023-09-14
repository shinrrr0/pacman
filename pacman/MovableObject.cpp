#include "Game.hpp"

// ����������� MovableObject
MovableObject::MovableObject() : DrawableObject() {
    this->setMovingDirectionToNone();
    this->speed = 0;
    this->frame_index = 0;
    this->points = 0;
}

// ���������� ����������� ������� �� ���� �������
void MovableObject::changeDirectionByInput(sf::Keyboard::Key key_code) {
    switch (key_code) {
    case sf::Keyboard::Up:
        changeDirection(Direction::Up);
        break;
    case sf::Keyboard::Right:
        changeDirection(Direction::Right);
        break;
    case sf::Keyboard::Down:
        changeDirection(Direction::Down);
        break;
    case sf::Keyboard::Left:
        changeDirection(Direction::Left);
        break;
    case sf::Keyboard::Space:
        changeDirection(Direction::None);
        break;
    }
}

// ��������� �����������, ��������� ���������������� �������
void MovableObject::changeDirection(Direction direction) {
    switch (direction) {
    case Direction::Up:
        this->normalized_moving_vector.x = 0;
        this->normalized_moving_vector.y = -1;
        break;
    case Direction::Right:
        this->normalized_moving_vector.x = 1;
        this->normalized_moving_vector.y = 0;
        break;
    case Direction::Down:
        this->normalized_moving_vector.x = 0;
        this->normalized_moving_vector.y = 1;
        break;
    case Direction::Left:
        this->normalized_moving_vector.x = -1;
        this->normalized_moving_vector.y = 0;
        break;
    case Direction::None:
        this->setMovingDirectionToNone();
        break;
    }
    this->direction = direction;
}

// ��������� �������� �� ������ �����������
void MovableObject::changeDirection(int direction) {
    this->changeDirection(static_cast<Direction>(direction));
}

// ����� ����������� �������� � ���������������� �������
void MovableObject::setMovingDirectionToNone() {
    this->direction = Direction::None;
    this->normalized_moving_vector.x = 0;
    this->normalized_moving_vector.y = 0;
}

// ���������� ������� ������ �������
void MovableObject::defineCell() {
    int x = (((this->getPosition().x) + TILE_SIDE_SIZE / 2 + TILE_SIDE_SIZE) / TILE_SIDE_SIZE);
    int y = (((this->getPosition().y) + TILE_SIDE_SIZE / 2 + TILE_SIDE_SIZE) / TILE_SIDE_SIZE);
    current_cell = helper->getCell(x - 1, y - 1);
}