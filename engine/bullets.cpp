#include "bullets.h"

Bullets::Bullets()
{

}

Bullets::Bullets(sf::Vector2f size, int direction)
{
    bullet.setSize(size);
    bullet.setFillColor(sf::Color::Cyan);

    switch(direction)
    {
    case 0:
        dir.x = 0;
        dir.y = -1;
        break;
    case 2:
        dir.x = 0;
        dir.y = 1;
        break;
    case 3:
        dir.x = -1;
        dir.y = 0;
        break;
    case 1:
        dir.x = 1;
        dir.y = 0;
        break;
    }
}

void Bullets::fire(float speedx, float speedy)
{
    bullet.move(speedx, speedy);
}

void Bullets::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(bullet);
}

void Bullets::setPos(sf::Vector2f newPos)
{
    bullet.setPosition(newPos);
}

int Bullets::getRight()
{
    return bullet.getPosition().x + bullet.getSize().x;
}

int Bullets::getLeft()
{
    return bullet.getPosition().x;
}

int Bullets::getTop()
{
    return bullet.getPosition().y;
}

int Bullets::getBottom()
{
    return bullet.getPosition().y + bullet.getSize().y;
}

void Bullets::update(float elapsed)
{
    //sf::Vector2f dist = dir * (elapsed * 50);
    fire(dir.x, dir.y);
}
