#ifndef TANK_H
#define TANK_H

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <QDebug>



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


class Tank: public sf::Drawable
{
public:

    Tank(sf::Vector2i tile = {});


    void shot();
    void moveStraight();
    void moveBackward();
    void turnLeft();
    void turnRight();

    sf::Vector2i getPossition() const;
    int getCurrnetDir() const;

private:

    int m_currnetDir;

    sf::Sprite  m_sprite;


protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif // TANK_H
