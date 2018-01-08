#ifndef GAME_H
#define GAME_H

#include "tank.h"
#include "server.h"
#include "bullets.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "tilemap.h"

#include <map>
#include <string>

const int MAX_PLAYER_NUMBER = 4;

enum gameState {RUNNING, WAITING};

class Game
{
public:
    Game();

    void run();
    void addTank(int id);
    void moveTank(int id, int direction);
    void deleteTank(int id);
    void setMessageText(const std::string &text);
    void performTankShoot(int tankId);
    void checkColBull();
    bool isTankInGame(int id);

    TileMap& getMap();
	std::vector<Bullets> getBullets();

    gameState state = gameState::WAITING;
    void reset();

private:
    void initialize();
    void handleEvents();
    void handleKeyboardInput();
    void update();
    void draw();

    void waitForKeyPress();
    void createBullet(int tankId, int direction);

    sf::RenderWindow m_window;
    sf::Event m_event;
    sf::Clock m_clock;
	Server m_server;
    TileMap m_tilemap;
    std::map<int, Tank> m_tanks;
    std::vector<Bullets> m_vecbullets;
    Bullets m_bullet;

	sf::Text m_messageText;

    int m_elapsedTime = 0;

};

#endif // GAME_H
