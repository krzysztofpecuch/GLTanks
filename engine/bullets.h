#ifndef BULLETS_H
#define BULLETS_H

#include <SFML/Graphics.hpp>

#define TILE_SIZE 50

class Bullets : public sf::Drawable
{
public:
    Bullets();
    Bullets(sf::Vector2f size, const int& direction);

    void fire(const float &speedx, const float &speedy);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void setPos(sf::Vector2f newPos);

    int getRight();
    int getLeft();
    int getTop();
    int getBottom();
    void update(float elapsed);
    sf::Vector2f getTile();
	int getDirection();

private:
    sf::RectangleShape m_bullet;
    sf::Vector2f m_dir;
    sf::Vector2f m_dist;
	int m_intDir;
};

#endif // BULLETS_H
