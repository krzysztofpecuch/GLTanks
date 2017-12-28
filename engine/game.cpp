#include "game.h"

const std::vector<StartPosition> START_POSITIONS = {{sf::Vector2i{ 1,  1}, LEFT},
                                                    {sf::Vector2i{ 1, 15}, LEFT},
                                                    {sf::Vector2i{15,  1}, RIGHT},
                                                    {sf::Vector2i{15, 15}, RIGHT}
                                                   };

//int count = 0;

Game::Game() :
    m_window(sf::VideoMode(850, 850), "GL Tanks", sf::Style::Titlebar | sf::Style::Close),
    m_server(*this)
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

    m_tilemap.loadFile();
    if(!m_tilemap.load("graphics/tilenew.png", sf::Vector2u(50, 50), 17, 17))
    {
        throw "Tiles could not be loaded";
    }

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
        switch(m_tanks[1].getCurrentDir())
        {
        case Directions::UP:
            if(m_tilemap.getTileNumber(m_tanks[1].getPosition().y - 1, m_tanks[1].getPosition().x) != 1)
            {
                m_tanks[1].moveStraight();
            }
            break;
        case Directions::DOWN:
            if(m_tilemap.getTileNumber(m_tanks[1].getPosition().y + 1, m_tanks[1].getPosition().x) != 1)
            {
                m_tanks[1].moveStraight();
            }
            break;
        case Directions::RIGHT:
            if(m_tilemap.getTileNumber(m_tanks[1].getPosition().y, m_tanks[1].getPosition().x + 1) != 1)
            {
                m_tanks[1].moveStraight();
            }
            break;
        case Directions::LEFT:
            if(m_tilemap.getTileNumber(m_tanks[1].getPosition().y, m_tanks[1].getPosition().x - 1) != 1)
            {
                m_tanks[1].moveStraight();
            }
            break;
        }
        break;
    case sf::Keyboard::Down:
        switch(m_tanks[1].getCurrentDir())
        {
        case Directions::UP:
            if(m_tilemap.getTileNumber(m_tanks[1].getPosition().y + 1, m_tanks[1].getPosition().x) != 1)
            {
                m_tanks[1].moveBackward();
            }
            break;
        case Directions::DOWN:
            if(m_tilemap.getTileNumber(m_tanks[1].getPosition().y - 1, m_tanks[1].getPosition().x) != 1)
            {
                m_tanks[1].moveBackward();
            }
            break;
        case Directions::RIGHT:
            if(m_tilemap.getTileNumber(m_tanks[1].getPosition().y, m_tanks[1].getPosition().x - 1) != 1)
            {
                m_tanks[1].moveBackward();
            }
            break;
        case Directions::LEFT:
            if(m_tilemap.getTileNumber(m_tanks[1].getPosition().y, m_tanks[1].getPosition().x + 1) != 1)
            {
                m_tanks[1].moveBackward();
            }
            break;
        }
        break;
    case sf::Keyboard::Left:
        m_tanks[1].turnLeft();
        break;
    case sf::Keyboard::Right:
        m_tanks[1].turnRight();
        break;
        //testowo
//    case sf::Keyboard::Space:
//        addTank(count);
//        count++;
//        break;
    default:
        break;
    }
}

void Game::update()
{
	float elapsedTime = m_clock.getElapsedTime().asSeconds();
    m_elapsedTime += m_clock.restart().asMilliseconds();

	if (state == gameState::RUNNING) {
		for (auto& tank : m_tanks)
		{
			tank.second.update(elapsedTime);
		}
	}

    if (m_elapsedTime >= 1000)
    {
		if (state == gameState::RUNNING) {
			m_server.sendData(m_tanks);
		}
        m_elapsedTime = 0;
    }
}

void Game::draw()
{
	m_window.clear(sf::Color::Black);

	//draw things here
	m_window.draw(m_tilemap);

	if (state == gameState::RUNNING){
		for (const auto& tank : m_tanks)
		{
			m_window.draw(tank.second);
		}
	}
	else {
		sf::Text messageText;

		sf::Font font;
		font.loadFromFile("fonts/arial.ttf");

		messageText.setFont(font);

		messageText.setString("Waiting for " + std::to_string((4-m_tanks.size())) + " more players.");

		messageText.setCharacterSize(60);

		messageText.setFillColor(sf::Color::White);

		sf::FloatRect textRect = messageText.getLocalBounds();
		messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		messageText.setPosition(m_window.getSize().x / 2.0f,  m_window.getSize().y / 2.0f);

		m_window.draw(messageText);
	}

    m_window.display();
}

void Game::addTank(int id)
{
       StartPosition position = START_POSITIONS[m_server.connectedClientsCount() - 1];
//    StartPosition position = START_POSITIONS[count];

    m_tanks[id] = Tank(position);
    
}

void Game::moveTank(int id, int direction)
{
    switch (direction)
    {
    case 0:
    {
        switch (m_tanks[id].getCurrentDir())
        {
        case Directions::UP:
            if(m_tilemap.getTileNumber(m_tanks[id].getPosition().y - 1, m_tanks[id].getPosition().x) != 1)
            {
                m_tanks[id].moveStraight();
            }
            break;
        case Directions::DOWN:
            if(m_tilemap.getTileNumber(m_tanks[id].getPosition().y + 1, m_tanks[id].getPosition().x) != 1)
            {
                m_tanks[id].moveStraight();
            }
            break;
        case Directions::RIGHT:
            if(m_tilemap.getTileNumber(m_tanks[id].getPosition().y, m_tanks[id].getPosition().x + 1) != 1)
            {
                m_tanks[id].moveStraight();
            }
            break;
        case Directions::LEFT:
            if(m_tilemap.getTileNumber(m_tanks[id].getPosition().y, m_tanks[id].getPosition().x - 1) != 1)
            {
                m_tanks[id].moveStraight();
            }
            break;
        }
    }
    break;
    case 1:
        m_tanks[id].turnLeft();
        break;
    case 2:
        m_tanks[id].turnRight();
        break;
    case 3:
        switch (m_tanks[id].getCurrentDir())
        {
        case Directions::UP:
            if(m_tilemap.getTileNumber(m_tanks[id].getPosition().y + 1, m_tanks[id].getPosition().x) != 1)
            {
                m_tanks[id].moveBackward();
            }
            break;
        case Directions::DOWN:
            if(m_tilemap.getTileNumber(m_tanks[id].getPosition().y - 1, m_tanks[id].getPosition().x) != 1)
            {
                m_tanks[id].moveBackward();
            }
            break;
        case Directions::RIGHT:
            if(m_tilemap.getTileNumber(m_tanks[id].getPosition().y, m_tanks[id].getPosition().x - 1) != 1)
            {
               m_tanks[id].moveBackward();
            }
            break;
        case Directions::LEFT:
            if(m_tilemap.getTileNumber(m_tanks[id].getPosition().y, m_tanks[id].getPosition().x + 1) != 1)
            {
                m_tanks[id].moveBackward();
            }
            break;
        }

        break;
    default:
        break;
    }
}

void Game::deleteTank(int id)
{
    const auto& position = m_tanks.find(id);
    m_tanks.erase(position);
}

TileMap &Game::getMap()
{
    return m_tilemap;
}
