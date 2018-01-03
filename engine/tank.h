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
    sf::Vector2i position;
    Directions direction;
};


class Tank: public sf::Drawable
{
public:

    Tank(sf::Vector2i tile = {});
    Tank(StartPosition initPosition);
	~Tank();


    void shot();
    void moveStraight();
    void moveBackward();
    void turnLeft();
    void turnRight();

    sf::Vector2i getPosition() const;
    int getCurrentDirection() const;
	void update(float elapsed);
	

private:

    int m_currentDirection;

	sf::Sprite  m_sprite;
	sf::Sprite m_actualSprite;
    sf::Texture tankTexture;
    sf::Image textureImage;
	


protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	
};

#endif // TANK_H
