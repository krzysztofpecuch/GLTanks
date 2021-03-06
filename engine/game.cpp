#include "game.h"
#include "resources.h"

const std::vector<StartPosition> START_POSITIONS = {
    StartPosition(sf::Vector2i( 1,  1), LEFT),
    StartPosition(sf::Vector2i( 1,  15), LEFT),
    StartPosition(sf::Vector2i( 15,  1), RIGHT),
    StartPosition(sf::Vector2i( 15,  15), RIGHT)
};

Game::Game() :
    m_window(sf::VideoMode(850, 850), "GL Tanks", sf::Style::Titlebar | sf::Style::Close),
    m_server(*this)
{
    state = GameState::WAITING;
}

Game::~Game()
{
    m_server.close();
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

    m_tilemap.loadTextMap();
    if(!m_tilemap.loadTileMap("graphics/tilenew.png", sf::Vector2u(50, 50), 17, 17))
    {
        throw "Tiles could not be loaded";
    }

    m_messageText.setFont(Resources::getFont(FontType::Arial));
    m_messageText.setCharacterSize(60);
    m_messageText.setFillColor(sf::Color::White);
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
        case Direction::UP:
            if(m_tilemap.getTileNumber(m_tanks[1].getPosition().y - 1, m_tanks[1].getPosition().x) != 3
                    && m_tilemap.getTileNumber(m_tanks[1].getPosition().y - 1, m_tanks[1].getPosition().x) != 2
                    && m_tilemap.getTileNumber(m_tanks[1].getPosition().y - 1, m_tanks[1].getPosition().x) != 1)
            {
                m_tanks[1].moveStraight();
            }
            break;
        case Direction::DOWN:
            if(m_tilemap.getTileNumber(m_tanks[1].getPosition().y + 1, m_tanks[1].getPosition().x) != 3
                    && m_tilemap.getTileNumber(m_tanks[1].getPosition().y + 1, m_tanks[1].getPosition().x) != 2
                    && m_tilemap.getTileNumber(m_tanks[1].getPosition().y + 1, m_tanks[1].getPosition().x) != 1)
            {
                m_tanks[1].moveStraight();
            }
            break;
        case Direction::RIGHT:
            if(m_tilemap.getTileNumber(m_tanks[1].getPosition().y, m_tanks[1].getPosition().x + 1) != 3
                    && m_tilemap.getTileNumber(m_tanks[1].getPosition().y, m_tanks[1].getPosition().x + 1) != 2
                    && m_tilemap.getTileNumber(m_tanks[1].getPosition().y, m_tanks[1].getPosition().x + 1) != 1)
            {
                m_tanks[1].moveStraight();
            }
            break;
        case Direction::LEFT:
            if(m_tilemap.getTileNumber(m_tanks[1].getPosition().y, m_tanks[1].getPosition().x - 1) != 3
                    && m_tilemap.getTileNumber(m_tanks[1].getPosition().y, m_tanks[1].getPosition().x - 1) != 2
                    && m_tilemap.getTileNumber(m_tanks[1].getPosition().y, m_tanks[1].getPosition().x - 1) != 1)
            {
                m_tanks[1].moveStraight();
            }
            break;
        }
        break;
    case sf::Keyboard::Down:
        switch(m_tanks[1].getCurrentDirection())
        {
        case Direction::UP:
            if(m_tilemap.getTileNumber(m_tanks[1].getPosition().y + 1, m_tanks[1].getPosition().x) != 3
                    && m_tilemap.getTileNumber(m_tanks[1].getPosition().y + 1, m_tanks[1].getPosition().x) != 2
                    && m_tilemap.getTileNumber(m_tanks[1].getPosition().y + 1, m_tanks[1].getPosition().x) != 1)
            {
                m_tanks[1].moveBackward();
            }
            break;
        case Direction::DOWN:
            if(m_tilemap.getTileNumber(m_tanks[1].getPosition().y - 1, m_tanks[1].getPosition().x) != 3
                    && m_tilemap.getTileNumber(m_tanks[1].getPosition().y - 1, m_tanks[1].getPosition().x) != 2
                    && m_tilemap.getTileNumber(m_tanks[1].getPosition().y - 1, m_tanks[1].getPosition().x) != 1)
            {
                m_tanks[1].moveBackward();
            }
            break;
        case Direction::RIGHT:
            if(m_tilemap.getTileNumber(m_tanks[1].getPosition().y, m_tanks[1].getPosition().x - 1) != 3
                    && m_tilemap.getTileNumber(m_tanks[1].getPosition().y, m_tanks[1].getPosition().x - 1) != 2
                    && m_tilemap.getTileNumber(m_tanks[1].getPosition().y, m_tanks[1].getPosition().x - 1) != 1)
            {
                m_tanks[1].moveBackward();
            }
            break;
        case Direction::LEFT:
            if(m_tilemap.getTileNumber(m_tanks[1].getPosition().y, m_tanks[1].getPosition().x + 1) != 3
                    && m_tilemap.getTileNumber(m_tanks[1].getPosition().y, m_tanks[1].getPosition().x + 1) != 2
                    && m_tilemap.getTileNumber(m_tanks[1].getPosition().y, m_tanks[1].getPosition().x + 1) != 1)
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
//    std::cout << "Update" << std::endl;
//    std::cout << m_secondCounter2 << std::endl;
    if (state == GameState::RUNNING)
    {
		for (auto& tank : m_tanks)
        {
            tank.second.update(m_elapsedTime);
        }

        for(auto& bullet : m_bullets)
        {
            bullet.update(m_elapsedTime);
        }

        checkBulletCollisions();
    }


//    timer += m_clock.restart().asSeconds();
//    std::cout << m_secondCounter2 << std::endl;
    if (m_secondCounter >= 1.f)
    {

        if (state == GameState::RUNNING)
        {
            //            std::cout << "Second" << std::endl;
            updateTanks();
            m_server.sendData(m_tanks);
//            if(m_bullets.size() > 5)
//            {
//                m_tilemap.updateMap(0, 5);
//                m_tilemap.updateMap(0, 8);
//                m_tilemap.updateMap(0, 12);
//            }
        }

        m_secondCounter = 0.f;
    }
}

void Game::updateClock()
{
	m_elapsedTime = m_clock.getElapsedTime().asSeconds();
    m_secondCounter += m_clock.restart().asSeconds();
}

void Game::draw()
{
    m_window.clear(sf::Color::Black);

    m_window.draw(m_tilemap);


    switch (state)
    {
    case GameState::RUNNING:
    {
        updateClock();
        for(auto& bullet : m_bullets)
        {
            m_window.draw(bullet);
        }

        for (const auto& tank : m_tanks)
        {
            m_window.draw(tank.second);
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

            waitForKeyPress();
            state = GameState::WAITING;
            reset();
        }
        break;
    }
    case GameState::WAITING:
    {
		resetClock();

        if (m_tanks.size() == MAX_PLAYER_NUMBER)
            state = GameState::RUNNING;

        setMessageText("Waiting for " + std::to_string((MAX_PLAYER_NUMBER - m_server.connectedClientsCount())) + " more players");
        m_window.draw(m_messageText);
        break;
    }
    }

    m_window.display();
}

void Game::resetClock()
{
	m_elapsedTime = 0.f;
    m_secondCounter = 0.f;
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

    for (int id = 0; id < m_server.connectedClientsCount(); ++id)
    {
        m_tanks[id] = Tank(START_POSITIONS[id]);
        m_tanks[id].setTexture(Resources::getTexture(static_cast<TextureType>(id)));
    }

    m_bullets.clear();

    if (m_server.connectedClientsCount() == MAX_PLAYER_NUMBER)
        state = GameState::RUNNING;
    else
        state = GameState::WAITING;
}

void Game::addTank(int id)
{
    StartPosition position = START_POSITIONS[id];

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
        case Direction::UP:
            if(m_tilemap.getTileNumber(m_tanks[id].getPosition().y - 1, m_tanks[id].getPosition().x) != 3
                    && m_tilemap.getTileNumber(m_tanks[id].getPosition().y - 1, m_tanks[id].getPosition().x) != 2
                    && m_tilemap.getTileNumber(m_tanks[id].getPosition().y - 1, m_tanks[id].getPosition().x) != 1)
            {
                m_tanks[id].moveStraight();
            }
            break;
        case Direction::DOWN:
            if(m_tilemap.getTileNumber(m_tanks[id].getPosition().y + 1, m_tanks[id].getPosition().x) != 3
                    && m_tilemap.getTileNumber(m_tanks[id].getPosition().y + 1, m_tanks[id].getPosition().x) != 2
                    && m_tilemap.getTileNumber(m_tanks[id].getPosition().y + 1, m_tanks[id].getPosition().x) != 1)
            {
                m_tanks[id].moveStraight();
            }
            break;
        case Direction::RIGHT:
            if(m_tilemap.getTileNumber(m_tanks[id].getPosition().y, m_tanks[id].getPosition().x + 1) != 3
                    && m_tilemap.getTileNumber(m_tanks[id].getPosition().y, m_tanks[id].getPosition().x + 1) != 2
                    && m_tilemap.getTileNumber(m_tanks[id].getPosition().y, m_tanks[id].getPosition().x + 1) != 1)
            {
                m_tanks[id].moveStraight();
            }
            break;
        case Direction::LEFT:
            if(m_tilemap.getTileNumber(m_tanks[id].getPosition().y, m_tanks[id].getPosition().x - 1) != 3
                    && m_tilemap.getTileNumber(m_tanks[id].getPosition().y, m_tanks[id].getPosition().x - 1) != 2
                    && m_tilemap.getTileNumber(m_tanks[id].getPosition().y, m_tanks[id].getPosition().x - 1) != 1)
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
        case Direction::UP:
            if(m_tilemap.getTileNumber(m_tanks[id].getPosition().y + 1, m_tanks[id].getPosition().x) != 3
                    && m_tilemap.getTileNumber(m_tanks[id].getPosition().y + 1, m_tanks[id].getPosition().x) != 2
                    && m_tilemap.getTileNumber(m_tanks[id].getPosition().y + 1, m_tanks[id].getPosition().x) != 1)
            {
                m_tanks[id].moveBackward();
            }
            break;
        case Direction::DOWN:
            if(m_tilemap.getTileNumber(m_tanks[id].getPosition().y - 1, m_tanks[id].getPosition().x) != 3
                    && m_tilemap.getTileNumber(m_tanks[id].getPosition().y - 1, m_tanks[id].getPosition().x) != 2
                    && m_tilemap.getTileNumber(m_tanks[id].getPosition().y - 1, m_tanks[id].getPosition().x) != 1)
            {
                m_tanks[id].moveBackward();
            }
            break;
        case Direction::RIGHT:
            if(m_tilemap.getTileNumber(m_tanks[id].getPosition().y, m_tanks[id].getPosition().x - 1) != 3
                    && m_tilemap.getTileNumber(m_tanks[id].getPosition().y, m_tanks[id].getPosition().x - 1) != 2
                    && m_tilemap.getTileNumber(m_tanks[id].getPosition().y, m_tanks[id].getPosition().x - 1) != 1)
            {
                m_tanks[id].moveBackward();
            }
            break;
        case Direction::LEFT:
            if(m_tilemap.getTileNumber(m_tanks[id].getPosition().y, m_tanks[id].getPosition().x + 1) != 3
                    && m_tilemap.getTileNumber(m_tanks[id].getPosition().y, m_tanks[id].getPosition().x + 1) != 2
                    && m_tilemap.getTileNumber(m_tanks[id].getPosition().y, m_tanks[id].getPosition().x + 1) != 1)
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

    if (position != m_tanks.end())
        m_tanks.erase(position);
}

void Game::createBullet(int tankId, int direction)
{
    if(direction == 0 || direction == 2)
    {
        m_bullet = BulletGFX(sf::Vector2f(3, 15), direction);
        m_bullet = BulletGFX(sf::Vector2f(3, 15), direction);
    }
    else
    {
        m_bullet = BulletGFX(sf::Vector2f(15, 3), direction);
        m_bullet = BulletGFX(sf::Vector2f(15, 3), direction);
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

    m_bullets.push_back(m_bullet);
}

void Game::checkBulletCollisions()
{
    for(auto it = m_bullets.begin(); it != m_bullets.end(); )
    {
        bool removeBullet = false;

        for(auto& tank : m_tanks)
        {
            if(it->getLeft() < tank.second.getPosition().x * TANK_SIZE + TANK_SIZE && it->getRight() > tank.second.getPosition().x * TANK_SIZE && it->getTop() < tank.second.getPosition().y * TANK_SIZE + TANK_SIZE && it->getBottom() > tank.second.getPosition().y * TANK_SIZE)
            {
                deleteTank(tank.first);
                removeBullet = true;
                break;
            }
        }

        if(m_tilemap.getTileNumber(it->getTop() / TANK_SIZE, it->getRight() / TANK_SIZE) || m_tilemap.getTileNumber(it->getBottom() / TANK_SIZE, it->getLeft() / TANK_SIZE))
        {
            if(m_tilemap.getTileNumber(it->getTop() / TANK_SIZE, it->getRight() / TANK_SIZE))
            {
                if(m_tilemap.getTileNumber(it->getTop() / TANK_SIZE, it->getRight() / TANK_SIZE) != 3 )
                {

                    m_tilemap.updateMap(it->getTop() / TANK_SIZE, it->getRight() / TANK_SIZE);
                    std::cout << m_tilemap.getTileNumber(it->getTop() / TANK_SIZE, it->getRight() / TANK_SIZE) << std::endl;
                }
            }else
            if(m_tilemap.getTileNumber(it->getBottom() / TANK_SIZE, it->getLeft() / TANK_SIZE))
            {
                if(m_tilemap.getTileNumber(it->getBottom() / TANK_SIZE, it->getLeft() / TANK_SIZE) != 3 )
                {

                    m_tilemap.updateMap(it->getBottom() / TANK_SIZE, it->getLeft() / TANK_SIZE);
                    std::cout << m_tilemap.getTileNumber(it->getBottom() / TANK_SIZE, it->getLeft() / TANK_SIZE) << std::endl;
                }
            }
            removeBullet = true;
        }

        if (removeBullet)
        {
            it = m_bullets.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool Game::isTankInGame(int id) const
{
    return (m_tanks.find(id) != m_tanks.end());
}

float Game::getElapsedTime()
{
	return m_elapsedTime;
}

unsigned Game::tanksCount() const
{
    return m_tanks.size();
}

void Game::updateTanks()
{
    std::map<int, Action> actions = m_server.getActionsToPerform();

    if (actions.size() != m_tanks.size())
    {
        std::cout << "Invalid actions map size!" << std::endl;
        return;
    }

    for (const auto& tank : m_tanks)
    {
        int id = tank.first;

        if (actions[id] == Stay)
            continue;

        else if (actions[id] == Shoot)
            performTankShoot(id);

        else
            moveTank(id, actions[id]);
    }
}

TileMap &Game::getMap()
{
    return m_tilemap;
}

const std::vector<BulletGFX> &Game::getBullets()
{
    return m_bullets;
}
