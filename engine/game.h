#ifndef GAME_H
#define GAME_H

#include "tank.h"

#include <SFML/Graphics.hpp>
#include <qDebug>

class Game
{
public:
    Game();

    void run();

private:
    void initialize();
    void handleEvents();
    void handleKeyboardInput();
    void update();
    void draw();

    sf::RenderWindow m_window;
    sf::Event m_event;

    Tank m_rudy;
};

#endif // GAME_H
