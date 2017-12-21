#include "tank.h"
#include "resources.h"


Tank::Tank(sf::Vector2i tile)
{
    qDebug() << "drugi konstuktor";
    m_sprite.setTexture(Resources::getTexture(TextureType::Tank));

    m_sprite.setPosition(tile.x * TILE_SIZE ,tile.y * TILE_SIZE);

    m_currnetDir = Directions::DOWN;
}

void Tank::shot()
{

}

void Tank::moveStraight()
{
    switch (m_currnetDir) {
    case UP:
        m_sprite.move(0, -TILE_SIZE);
        break;
    case DOWN:
        m_sprite.move(0, TILE_SIZE);
        break;
    case LEFT:
        m_sprite.move(- TILE_SIZE ,0);
        break;
    case RIGHT:
        m_sprite.move(  TILE_SIZE ,0);
        break;
    default:
        break;
    }
}

void Tank::moveBackward()
{
    switch (m_currnetDir) {
    case UP:
        m_sprite.move(0, TILE_SIZE);
        break;
    case DOWN:
        m_sprite.move(0, -TILE_SIZE);
        break;
    case LEFT:
        m_sprite.move( TILE_SIZE ,0);
        break;
    case RIGHT:
        m_sprite.move(  -TILE_SIZE ,0);
        break;
    default:
        break;
    }
}

void Tank::turnLeft()
{
    m_currnetDir -=1;
    if(m_currnetDir < UP)
    {
        m_currnetDir = LEFT;
    }

    m_sprite.rotate(-90);

    switch (m_currnetDir) {
    case UP:
        m_sprite.setPosition(m_sprite.getPosition().x + TANK_SIZE, m_sprite.getPosition().y);
        break;
    case DOWN:
        m_sprite.setPosition(m_sprite.getPosition().x - TANK_SIZE, m_sprite.getPosition().y);
        break;
    case LEFT:
        m_sprite.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y - TANK_SIZE);
        break;
    case RIGHT:
        m_sprite.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y + TANK_SIZE);
        break;
    default:
        break;
    }
}

void Tank::turnRight()
{
    m_currnetDir +=1;
    if(m_currnetDir == NUMBER_DIR_STATES)
    {
        m_currnetDir = UP;
    }

    m_sprite.rotate(90);

    switch (m_currnetDir) {
    case UP:
        m_sprite.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y + TANK_SIZE);
        break;
    case DOWN:
        m_sprite.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y - TANK_SIZE );
        break;
    case LEFT:
        m_sprite.setPosition(m_sprite.getPosition().x + TANK_SIZE , m_sprite.getPosition().y);
        break;
    case RIGHT:
        m_sprite.setPosition(m_sprite.getPosition().x - TANK_SIZE , m_sprite.getPosition().y);
        break;
    default:
        break;
    }

}

sf::Vector2i Tank::getPossition() const
{
    switch (m_currnetDir) {
    case UP:
        return(sf::Vector2i(m_sprite.getPosition().x / TANK_SIZE - 1 , m_sprite.getPosition().y / TANK_SIZE - 1));
        break;
    case DOWN:
        return(sf::Vector2i(m_sprite.getPosition().x / TANK_SIZE , m_sprite.getPosition().y / TANK_SIZE));
        break;
    case LEFT:
        return(sf::Vector2i(m_sprite.getPosition().x / TANK_SIZE - 1 , m_sprite.getPosition().y / TANK_SIZE));
        break;
    case RIGHT:
        return(sf::Vector2i(m_sprite.getPosition().x / TANK_SIZE , m_sprite.getPosition().y / TANK_SIZE - 1));
        break;
    default:
        return(sf::Vector2i(-1,-1));
        break;
    }
}

int Tank::getCurrnetDir() const
{
    return m_currnetDir;
}

void Tank::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_sprite);
}
