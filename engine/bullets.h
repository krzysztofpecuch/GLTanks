#ifndef BULLETS_H
#define BULLETS_H

#include <SFML/Graphics.hpp>

class Bullets : public sf::Drawable
{
public:
    Bullets();
    Bullets(sf::Vector2f size, int direction);

    void fire(float speedx, float speedy);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    void setPos(sf::Vector2f newPos);

    int getRight();
    int getLeft();
    int getTop();
    int getBottom();
    void update(float elapsed);

private:
    sf::RectangleShape bullet;
    sf::Vector2f dir;
};

#endif // BULLETS_H
