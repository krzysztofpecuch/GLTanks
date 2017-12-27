#ifndef GAME_H
#define GAME_H

#include "tank.h"
#include "server.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "tilemap.h"

#include <map>
#include <string>

enum gameState {RUNNING, WAITING};

class Game
{
public:
    Game();

    void run();
    void addTank(int id);
    void moveTank(int id, int direction);
    void deleteTank(int id);

    TileMap& getMap();

	gameState state = gameState::WAITING;

private:
    void initialize();
    void handleEvents();
    void handleKeyboardInput();
    void update();
    void draw();

    sf::RenderWindow m_window;
    sf::Event m_event;
    sf::Clock m_clock;
	Server m_server;
    TileMap m_tilemap;
    std::map<int, Tank> m_tanks;

    int m_elapsedTime = 0;
	
};

#endif // GAME_H
