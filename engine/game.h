#ifndef GAME_H
#define GAME_H

#include "tank.h"
#include "server.h"

#include <SFML/Graphics.hpp>
#include "tilemap.h"

#include <map>

class Game
{
public:
    Game();

    void run();
    void addTank(int id);
    void deleteTank(int id);

private:
    void initialize();
    void handleEvents();
    void handleKeyboardInput();
    void update();
    void draw();

    sf::RenderWindow m_window;
    sf::Event m_event;
    Server m_server;
	TileMap m_tilemap;
    std::map<int, Tank> m_tanks;
};

#endif // GAME_H
