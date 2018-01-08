#include "game.h"

#include <conio.h>

#include "resources.h"

const std::vector<StartPosition> START_POSITIONS = {
    StartPosition(sf::Vector2i( 1,  1), LEFT),
    StartPosition(sf::Vector2i( 1,  15), LEFT),
    StartPosition(sf::Vector2i( 15,  1), RIGHT),
    StartPosition(sf::Vector2i( 15,  15), RIGHT)
};

//int count = 0;

Game::Game() :
    m_window(sf::VideoMode(850, 850), "GL Tanks", sf::Style::Titlebar | sf::Style::Close),
    m_server(*this)
{

}

Game::~Game()
{
    m_server.close();

    std::cout << "Destructor map tank size: " << m_tanks.size();

    m_tanks.clear();
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

    m_messageText.setFont(Resources::getFont(FontType::Arial));
    m_messageText.setCharacterSize(60);
    m_messageText.setFillColor(sf::Color::White);

//    setMessageText("Waiting for " + std::to_string((MAX_PLAYER_NUMBER - m_tanks.size())) + " more players.");
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
        switch(m_tanks[1].getCurrentDirection())
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
        switch(m_tanks[1].getCurrentDirection())
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
    case sf::Keyboard::C:
        createBullet(1, m_tanks[1].getCurrentDirection());
        break;
    default:
        break;
    }
}

void Game::update()
{
	float elapsedTime = m_clock.getElapsedTime().asSeconds();
    m_elapsedTime += m_clock.restart().asMilliseconds();
	
	if (state == gameState::RUNNING)
    {
		for (auto& tank : m_tanks)
        {
            tank.second.update(elapsedTime);
        }
        for(auto& bulletss : m_vecbullets)
        {
            bulletss.update(elapsedTime);
            checkColBull();
        }
    }
	
	if (m_elapsedTime >= 1000)
    {
        if (state == gameState::RUNNING)
        {
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

    switch (state)
    {
    case RUNNING:
    {
		for(auto& bullet : m_vecbullets)
        {
            m_window.draw(bullet);
        }

        for (const auto& tank : m_tanks)
        {
            m_window.draw(tank.second);

//            std::cout << "Tank position: ( " << tank.second.getPosition().x << ", " << tank.second.getPosition().y << " )" << std::endl;
        }

        if (m_tanks.size() == 1)
        {
            setMessageText("Press any key to play again");
            m_window.draw(m_messageText);
            m_window.display();

            int winnerId = -1;

            for (const auto& tank : m_tanks)
            {
                winnerId = tank.first;
            }

            m_server.sendDataMatchEnd(winnerId);

            for (const auto& tank : m_tanks)
            {
                std::cout << "Tank ID:" << tank.first << std::endl;
            }

            waitForKeyPress();
            state = WAITING;
            reset();
        }
        break;
    }
    case WAITING:
    {
        if (m_tanks.size() == MAX_PLAYER_NUMBER)
            state = RUNNING;

        setMessageText("Waiting for " + std::to_string((MAX_PLAYER_NUMBER - m_server.connectedClientsCount())) + " more players");
        m_window.draw(m_messageText);
        break;
    }
    }

    m_window.display();
}

void Game::setMessageText(const std::string& text)
{
    m_messageText.setString(text);

    sf::FloatRect textRect = m_messageText.getLocalBounds();
    m_messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    m_messageText.setPosition(m_window.getSize().x / 2.0f,  m_window.getSize().y / 2.0f);
}

void Game::performTankShoot(int tankId)
{
    createBullet(tankId, m_tanks[tankId].getCurrentDirection());
}

void Game::waitForKeyPress()
{
    bool keyPressed = false;
    while (!keyPressed)
    {
        while (m_window.pollEvent(m_event))
        {
            if (m_event.type == sf::Event::Closed)
            {
                m_window.close();
                keyPressed = true;
                break;
            }

            if (m_event.type == sf::Event::KeyPressed)
                keyPressed = true;
        }
    }
}

void Game::reset()
{
    m_tanks.clear();

    for (int id = 0; id < MAX_PLAYER_NUMBER; ++id)
    {
        m_tanks[id] = Tank(START_POSITIONS[id]);
        m_tanks[id].setTexture(Resources::getTexture(static_cast<TextureType>(id)));
    }

    m_vecbullets.clear();

    if (m_server.connectedClientsCount() == MAX_PLAYER_NUMBER)
        state = RUNNING;
    else
        state = WAITING;
}

void Game::addTank(int id)
{
    StartPosition position = START_POSITIONS[id];

    std::cout << "Tank added" << std::endl;

    m_tanks[id] = Tank(position);
    m_tanks[id].setTexture(Resources::getTexture(static_cast<TextureType>(id))); 
}

void Game::moveTank(int id, int direction)
{
    switch (direction)
    {
    case 0:
    {
        switch (m_tanks[id].getCurrentDirection())
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
        switch (m_tanks[id].getCurrentDirection())
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
    std::cout << "deleteTank(): size before: " << m_tanks.size() << std::endl;
    m_tanks.erase(position);
    std::cout << "deleteTank(): size after: " << m_tanks.size() << std::endl;

//    for (const auto& tank : m_tanks)
//    {
//        std::cout << "Tank ID:" << tank.first << std::endl;
//    }

    std::cout << std::endl;
}

void Game::createBullet(int tankId, int direction)
{
    if(direction == 0 || direction == 2)
    {
        m_bullet = Bullets(sf::Vector2f(3, 15), direction);
        m_bullet = Bullets(sf::Vector2f(3, 15), direction);
    }else
    {
        m_bullet = Bullets(sf::Vector2f(15, 3), direction);
        m_bullet = Bullets(sf::Vector2f(15, 3), direction);
    }

    switch(direction)
    {
    case 0:
    {
        m_bullet.setPos(sf::Vector2f(m_tanks[tankId].getPosition().x * TANK_SIZE + TANK_SIZE / 2, m_tanks[tankId].getPosition().y * TANK_SIZE - 15));
        break;
    }
    case 1:
    {
        m_bullet.setPos(sf::Vector2f(m_tanks[tankId].getPosition().x * TANK_SIZE + TANK_SIZE , m_tanks[tankId].getPosition().y * TANK_SIZE + TANK_SIZE / 2));
        break;
    }
    case 2:
    {
        m_bullet.setPos(sf::Vector2f(m_tanks[tankId].getPosition().x * TANK_SIZE + TANK_SIZE / 2, m_tanks[tankId].getPosition().y * TANK_SIZE + TANK_SIZE));
        break;
    }
    case 3:
    {
        m_bullet.setPos(sf::Vector2f(m_tanks[tankId].getPosition().x * TANK_SIZE - 15, m_tanks[tankId].getPosition().y * TANK_SIZE + TANK_SIZE / 2));
        break;
    }
    }

    m_vecbullets.push_back(m_bullet);
}

void Game::checkColBull()
{
    for(auto it = m_vecbullets.begin(); it != m_vecbullets.end(); )
    {
        bool removeBullet = false;

        for(auto& tank : m_tanks)
        {
            if(it->getLeft() < tank.second.getPosition().x * TANK_SIZE + TANK_SIZE && it->getRight() > tank.second.getPosition().x * TANK_SIZE && it->getTop() < tank.second.getPosition().y * TANK_SIZE + TANK_SIZE && it->getBottom() > tank.second.getPosition().y * TANK_SIZE)
            {
                deleteTank(tank.first);
                removeBullet = true;

                std::cout << "m_tanks.size(): " << m_tanks.size() << std::endl;

                break;
            }
        }

        if(m_tilemap.getTileNumber(it->getTop() / TANK_SIZE, it->getRight() / TANK_SIZE) || m_tilemap.getTileNumber(it->getBottom() / TANK_SIZE, it->getLeft() / TANK_SIZE))
        {
            removeBullet = true;
        }

        if (removeBullet)
        {
            it = m_vecbullets.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool Game::isTankInGame(int id)
{
    return (m_tanks.find(id) != m_tanks.end());
}

unsigned Game::tanksCount() const
{
    return m_tanks.size();
}

TileMap &Game::getMap()
{
    return m_tilemap;
}

std::vector<Bullets> Game::getBullets()
{
	return m_vecbullets;
}
