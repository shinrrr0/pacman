#include <SFML/Graphics.hpp>
#include "Game.hpp"


int main() {
    Game* game = new Game; // Создание экземпляра класса игры
    game->window.setFramerateLimit(144);

    game->ghost.findPath(game->ghost.current_cell, game->player.current_cell);

    // Игровой цикл
    while (game->window.isOpen()) {
        sf::Event event;

        //Обработка SFML Events
        while (game->window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed: game->window.close(); break;
            case sf::Event::KeyPressed: game->inputHandler(event.key.code); break;
            }
        }
        if (game->is_game_going) {
            // Очищение игрового окна
            game->window.clear();

            // Рендер игровых клеток
            game->drawPlayableTiles();

            // Обновление времени между кадрами
            game->update();

            // Перемещение объекта
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

            // Проверка перехода через границу окна
            game->checkTransition(game->player);

            // Проверка столкновения игрока со стенами
            game->checkCollisions(game->player);

            // Вызов поиска пути приведения
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

            // Отрисовка двигающихся объектов
            game->window.draw(game->player);
            game->window.draw(game->ghost);

            // Рендер рамки
            game->drawBorderTiles();

            // Обновление строки
            game->text.setString(std::to_string(game->player.points) + " : " + std::to_string(game->helper.max_points));

            //game->text.setString(std::to_string(1 / game->delta_time)); // ФПС метр

            // Рендер текста
            game->window.draw(game->text);

            // Отображение всех объектов на окне
            game->window.display();
        }
    }
    delete game;
    return 0;
}
