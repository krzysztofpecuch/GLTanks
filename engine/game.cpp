#include "game.h"

Game::Game() :
    m_window(sf::VideoMode(800, 800), "GL Tanks", sf::Style::Titlebar | sf::Style::Close),
    m_server(*this)
{

}

void Game::run()
{
    m_server.run();

    initialize();

    while (m_window.isOpen())
    {
        while (m_window.pollEvent(m_event))
        {
            handleEvents();
        }

        update();
        draw();
    }

}

void Game::initialize()
{

}

void Game::handleEvents()
{
    switch (m_event.type)
    {
    case sf::Event::Closed:
        m_window.close();
        break;

    case sf::Event::KeyPressed:
        handleKeyboardInput();
        break;

    default:
        break;
    }
}

void Game::handleKeyboardInput()
{
    switch (m_event.key.code)
    {
    case sf::Keyboard::Escape:
        m_window.close();
        break;
    case sf::Keyboard::Up:
        m_tanks[0].moveStraight();
        break;
    case sf::Keyboard::Down:
        m_tanks[0].moveBackward();
        break;
    case sf::Keyboard::Left:
        m_tanks[0].turnLeft();
        break;
    case sf::Keyboard::Right:
        m_tanks[0].turnRight();
        break;
    default:
        break;
    }
}

void Game::update()
{

}

void Game::draw()
{
    m_window.clear(sf::Color::Black);

    //draw things here

    for(const auto& tank: m_tanks)
    {
        m_window.draw(tank.second);
    }

    m_window.display();
}

void Game::addTank(int id)
{
    static int x = 0;
    static int y = 0;

    Tank tank(sf::Vector2i(x++, y++));
    m_tanks[id] = tank;
}

void Game::deleteTank(int id)
{
    const auto& position = m_tanks.find(id);
    m_tanks.erase(position);
}
