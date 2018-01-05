#include "bullets.h"
#include <iostream>

Bullets::Bullets()
{

}

Bullets::Bullets(sf::Vector2f size, int direction)
{
    m_bullet.setSize(size);
    m_bullet.setFillColor(sf::Color::Cyan);

    switch(direction)
    {
    case 0:
        m_dir.x = 0;
        m_dir.y = -TILE_SIZE;
        break;
    case 2:
        m_dir.x = 0;
        m_dir.y = TILE_SIZE;
        break;
    case 3:
        m_dir.x = -TILE_SIZE;
        m_dir.y = 0;
        break;
    case 1:
        m_dir.x = TILE_SIZE;
        m_dir.y = 0;
        break;
    }
}

void Bullets::fire(float speedx, float speedy)
{
    m_bullet.move(speedx, speedy);
}

void Bullets::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_bullet, states);
}

void Bullets::setPos(sf::Vector2f newPos)
{
    m_bullet.setPosition(newPos);
}

int Bullets::getRight()
{
    return m_bullet.getPosition().x + m_bullet.getSize().x;
}

int Bullets::getLeft()
{
    return m_bullet.getPosition().x;
}

int Bullets::getTop()
{
    return m_bullet.getPosition().y;
}

int Bullets::getBottom()
{
    return m_bullet.getPosition().y + m_bullet.getSize().y;
}

void Bullets::update(float elapsed)
{
    m_dist = m_dir * (elapsed / 2);

    fire(m_dist.x, m_dist.y);
}
