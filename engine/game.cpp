#include "game.h"

Game::Game() :
    m_window(sf::VideoMode(800, 800), "GL Tanks", sf::Style::Titlebar | sf::Style::Close)
{

}

void Game::run()
{
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

    m_window.display();
}
