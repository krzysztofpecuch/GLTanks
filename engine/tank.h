#ifndef TANK_H
#define TANK_H

#include <SFML\Graphics.hpp>

#include <fstream>
#include <iostream>

const int TANK_SIZE = 50;
const int TILE_SIZE = 50;

enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT,
    DIRECTIONS_COUNT
};

struct StartPosition
{
    sf::Vector2i position = {};
    Direction direction = UP;

    StartPosition(const sf::Vector2i& position = {}, const Direction& direction = UP) :
        position(position), direction(direction) { }
};

class Tank: public sf::Drawable
{
public:
    Tank(const StartPosition& initPosition = StartPosition());

    void moveStraight();
    void moveBackward();
    void turnLeft();
    void turnRight();

    void setPosition(const StartPosition& position);
    void setTexture(const sf::Texture &texture);
    sf::Vector2i getPosition() const;
    int getCurrentDirection() const;
	void update(float elapsed);

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Sprite  m_ghostSprite;
    sf::Sprite m_sprite;
    int m_currentDirection;
};

#endif // TANK_H
