#include "bulletgfx.h"

const int TILE_SIZE = 50;

BulletGFX::BulletGFX(const sf::Vector2f &size, const int& direction)
{
    m_bullets.setSize(size);
    m_bullets.setFillColor(sf::Color::Cyan);
    m_intDirection = direction;

    switch(direction)
    {
    case 0:
        m_direction.x = 0;
        m_direction.y = -TILE_SIZE;
        break;
    case 2:
        m_direction.x = 0;
        m_direction.y = TILE_SIZE;
        break;
    case 3:
        m_direction.x = -TILE_SIZE;
        m_direction.y = 0;
        break;
    case 1:
        m_direction.x = TILE_SIZE;
        m_direction.y = 0;
        break;
    }
}

void BulletGFX::fire(const float& speedx, const float& speedy)
{
    m_bullets.move(speedx, speedy);
}

void BulletGFX::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_bullets, states);
}

void BulletGFX::setPos(const sf::Vector2f &newPos)
{
    m_bullets.setPosition(newPos);
}

int BulletGFX::getRight() const
{
    return m_bullets.getPosition().x + m_bullets.getSize().x;
}

int BulletGFX::getLeft() const
{
    return m_bullets.getPosition().x;
}

int BulletGFX::getTop() const
{
    return m_bullets.getPosition().y;
}

int BulletGFX::getBottom() const
{
    return m_bullets.getPosition().y + m_bullets.getSize().y;
}

void BulletGFX::update(float elapsed)
{
    getTile();
    m_distance = m_direction * elapsed;

    fire(m_distance.x, m_distance.y);
}

sf::Vector2f BulletGFX::getTile() const
{
    return sf::Vector2f(getTop() / TILE_SIZE, getLeft() / TILE_SIZE);
}

int BulletGFX::getDirection() const
{
    return m_intDirection;
}
