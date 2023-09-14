#include <SFML/Graphics.hpp>
#include "Game.hpp"


int main() {
    Game* game = new Game; // �������� ���������� ������ ����
    game->window.setFramerateLimit(144);

    game->ghost.findPath(game->ghost.current_cell, game->player.current_cell);

    // ������� ����
    while (game->window.isOpen()) {
        sf::Event event;

        //��������� SFML Events
        while (game->window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed: game->window.close(); break;
            case sf::Event::KeyPressed: game->inputHandler(event.key.code); break;
            }
        }
        if (game->is_game_going) {
            // �������� �������� ����
            game->window.clear();

            // ������ ������� ������
            game->drawPlayableTiles();

            // ���������� ������� ����� �������
            game->update();

            // ����������� �������
            game->moveInCurrentDirection(game->player);
            game->moveInCurrentDirection(game->ghost);

            game->updatePoints(game->player);

            if (game->player.points >= game->helper.max_points) {
                game->drawPlayableTiles();
                game->is_game_going = false;
                game->window.draw(game->player);
                game->window.draw(game->ghost);
                game->window.draw(game->win_text);
                game->window.draw(game->exit_text);
                game->window.display();
                continue;
            }

            // �������� �������� ����� ������� ����
            game->checkTransition(game->player);

            // �������� ������������ ������ �� �������
            game->checkCollisions(game->player);

            // ����� ������ ���� ����������
            if (game->ghost.checkTargetCell()) {
                game->checkTransition(game->ghost);
                game->ghost.findPath(game->ghost.current_cell, game->player.current_cell);
            }

            if (game->ghost.current_cell == game->player.current_cell) {
                game->deaths++;
                if (game->deaths >= 3) {
                    game->is_game_going = false;
                    game->window.draw(game->player);
                    game->window.draw(game->ghost);
                    game->window.draw(game->lose_text);
                    game->window.draw(game->exit_text);
                    game->window.display();
                    continue;
                }
                game->player.setPosition((2 * TILE_SIDE_SIZE), (2 * TILE_SIDE_SIZE));
                game->player.defineCell();
                game->player.setMovingDirectionToNone();
                game->ghost.setPosition((13 * TILE_SIDE_SIZE), (11 * TILE_SIDE_SIZE));
                game->ghost.defineCell();
                game->ghost.findPath(game->ghost.current_cell, game->player.current_cell);
            }

            // ��������� ����������� ��������
            game->window.draw(game->player);
            game->window.draw(game->ghost);

            // ������ �����
            game->drawBorderTiles();

            // ���������� ������
            game->text.setString(std::to_string(game->player.points) + " : " + std::to_string(game->helper.max_points));

            //game->text.setString(std::to_string(1 / game->delta_time)); // ��� ����

            // ������ ������
            game->window.draw(game->text);

            // ����������� ���� �������� �� ����
            game->window.display();
        }
    }
    delete game;
    return 0;
}
