#ifndef BULLETGFX_H
#define BULLETGFX_H

#include <SFML/Graphics.hpp>

class BulletGFX : public sf::Drawable
{
public:
    BulletGFX(const sf::Vector2f& size = {}, const int& direction = 0);

    void fire(const float &speedx, const float &speedy);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void setPos(const sf::Vector2f& newPos);

    int getRight() const;
    int getLeft() const;
    int getTop() const;
    int getBottom() const;
    void update(float elapsed);
    sf::Vector2f getTile() const;
    int getDirection() const;

private:
    sf::RectangleShape m_bullets;
    sf::Vector2f m_direction;
    sf::Vector2f m_distance;
    int m_intDirection;
};

#endif // BULLETGFX_H
