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
    m_server.run();
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
    case sf::Keyboard::Space:
        addTank();
        break;
    case sf::Keyboard::Delete:
        deleteTank();
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
    //m_window.draw(m_rudy);
    for(const auto& tank: m_tanks)
    {
        m_window.draw(tank);
    }

    m_window.display();

//    qDebug() << "posssition" << m_rudy.getPossition().x << m_rudy.getPossition().y;
}

void Game::addTank()
{
    Tank tank;
    m_tanks.push_back(tank);
}

void Game::deleteTank()
{

}
