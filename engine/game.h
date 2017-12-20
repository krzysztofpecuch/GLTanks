#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

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
};

#endif // GAME_H
