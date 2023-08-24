#include <SFML/Graphics.hpp>
#include "Game.hpp"


int main() {
    Game* game = new Game;
    game->window.setFramerateLimit(144);

    //game->ghost.setTargetCell(game->getCell(4, 6));
    //for (auto elem: game->getCell(5, 5)->connected_with) {
    //    elem->setTextureByPath("assets\\target.png");
    //}
    //game->ghost.findPath(game->ghost.current_cell, game->player.current_cell);
    //game->ghost.findPath(game->getCell(3, 3), game->getCell(3, 3));

    while (game->window.isOpen()) {
        sf::Event event;
        while (game->window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed: game->window.close(); break;
            case sf::Event::KeyPressed: game->inputHandler(event.key.code); break;
            }
        }
        game->window.clear();
        game->drawPlayableTiles();

        game->update();

        game->moveInCurrentDirection(game->player);
        game->moveInCurrentDirection(game->ghost);

        game->checkTransition(game->player);
        game->checkCollisions(game->player);

        game->ghost.checkTargetCell();

        game->window.draw(game->player);// - ��������
        game->window.draw(game->ghost);// - ��������
        game->drawBorderTiles();


        game->text.setString(std::to_string(game->player.current_cell->x) + ' ' + std::to_string(game->player.current_cell->y));
        //sf::Vector2i pos = game->defineCellByCoords(game->player.getPosition().x, game->player.getPosition().y);
        //game->text.setString(std::to_string(pos.x) + ' ' + std::to_string(pos.y));
        //game->text.setString(std::to_string(1 / game->delta_time)); // ��� ����
        game->window.draw(game->text);
        game->window.display();
    }

    delete game;
    return 0;
}

//std::cout << static_cast<int>(Direction::Right);
//sf::CircleShape shape(100.f);
//shape.setPosition(100, 100);
//shape.setFillColor(sf::Color::Red);
//shape.move(sf::Vector2f(20.f * deltaTime, 20.f * deltaTime));
//window.draw(shape);
//window.setFramerateLimit(5);