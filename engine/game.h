#ifndef GAME_H
#define GAME_H

#include "tank.h"

#include <SFML/Graphics.hpp>

#include "server.h"

class Game
{
public:
    Game();

    void run();
    void addTank();

private:
    void initialize();
    void handleEvents();
    void handleKeyboardInput();
    void update();
    void draw();
    void deleteTank();

    sf::RenderWindow m_window;
    sf::Event m_event;
    Server m_server;
    std::vector<Tank> m_tanks;
};

#endif // GAME_H
