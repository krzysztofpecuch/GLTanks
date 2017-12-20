#ifndef GAME_H
#define GAME_H

#include "tank.h"

#include <SFML/Graphics.hpp>
#include <qDebug>

#include "server.h"

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
    void addTank();
    void deleteTank();

    sf::RenderWindow m_window;
    sf::Event m_event;
    Server m_server;
    std::vector<Tank> m_tanks;
    Tank m_rudy;
};

#endif // GAME_H
