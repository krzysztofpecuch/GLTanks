#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include "tank.h"
#include "server.h"
#include "bulletgfx.h"
#include "tilemap.h"

#include <map>

const int MAX_PLAYER_NUMBER = 4;

enum class GameState
{
    RUNNING,
    WAITING
};

class Game
{
public:
    Game();
    ~Game();

    void run();
    void addTank(int id);
    void moveTank(int id, int direction);
    void deleteTank(int id);
    void setMessageText(const std::string &text);
    void performTankShoot(int tankId);
    bool isTankInGame(int id) const;
	float getElapsedTime();
    unsigned tanksCount() const;


    TileMap& getMap();
    const std::vector<BulletGFX>& getBullets();

    GameState state;
    void reset();

private:
    void initialize();
    void handleEvents();
    void handleKeyboardInput();
    void update();
	void updateClock();
    void updateTanks();
    void draw();
	void resetClock();

    void waitForKeyPress();
    void createBullet(int tankId, int direction);
    void checkBulletCollisions();

    sf::RenderWindow m_window;
    sf::Event m_event;
    
	sf::Clock m_clock;
	float m_elapsedTime;
	int m_secondCounter = 0;
    float m_secondCounter2 = 0;

	Server m_server;
    TileMap m_tilemap;
    std::map<int, Tank> m_tanks;
    std::vector<BulletGFX> m_bullets;
    BulletGFX m_bullet;
	sf::Text m_messageText;
    
};

#endif // GAME_H
