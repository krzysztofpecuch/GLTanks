#ifndef TANK_H
#define TANK_H

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <SFML\System.hpp>
#include <iostream>
#include <fstream>
#include <string>



#define TANK_SIZE 50
#define TILE_SIZE 50

enum Directions
{
    UP,
    RIGHT,
    DOWN,
    LEFT,
    NUMBER_DIR_STATES
};

struct StartPosition
{
    sf::Vector2i position = {};
    Directions direction = UP;

    StartPosition(const sf::Vector2i& pos = {}, const Directions& dir = UP) :
        position(pos), direction(dir)
    {

    }
};


class Tank: public sf::Drawable
{
public:
    Tank(const StartPosition& initPosition = StartPosition());
    ~Tank();

    void shot();
    void moveStraight();
    void moveBackward();
    void turnLeft();
    void turnRight();

    void setPosition(const StartPosition& position);
    sf::Vector2i getPosition() const;
    int getCurrentDirection() const;
	void update(float elapsed);

private:
    int m_currentDirection;

	sf::Sprite  m_sprite;
	sf::Sprite m_actualSprite;
    sf::Texture tankTexture;
    sf::Image textureImage;
	
    static int instancesCount;

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif // TANK_H
