#include "tank.h"
#include "resources.h"
#include <random>

Tank::Tank(const StartPosition &initPosition)
{	
    m_ghostSprite.setPosition(initPosition.position.x * TILE_SIZE ,initPosition.position.y * TILE_SIZE);
    m_sprite.setPosition(initPosition.position.x * TILE_SIZE, initPosition.position.y * TILE_SIZE);

    switch (initPosition.direction)
    {
    case UP:
        m_ghostSprite.rotate(180);
        m_sprite.rotate(180);
        m_currentDirection = Direction::UP;
        break;
    case DOWN:
        m_currentDirection = Direction::DOWN;
        break;
    case LEFT:
        m_ghostSprite.rotate(90);
        m_sprite.rotate(90);
        m_currentDirection = Direction::LEFT;
        m_ghostSprite.move(TILE_SIZE, 0);
        m_sprite.move(TILE_SIZE, 0);
        break;
    case RIGHT:
        m_ghostSprite.rotate(-90);
        m_sprite.rotate(-90);
        m_currentDirection = Direction::RIGHT;
        m_ghostSprite.move(0, TILE_SIZE);
        m_sprite.move(0, TILE_SIZE);
        break;
    default:
        break;
    }
}

void Tank::moveStraight()
{
    switch (m_currentDirection)
    {
    case UP:
        m_ghostSprite.move(0, -TILE_SIZE);
        break;
    case DOWN:
        m_ghostSprite.move(0, TILE_SIZE);
        break;
    case LEFT:
        m_ghostSprite.move(- TILE_SIZE ,0);
        break;
    case RIGHT:
        m_ghostSprite.move(  TILE_SIZE ,0);
        break;
    default:
        break;
    }
}

void Tank::moveBackward()
{
    switch (m_currentDirection)
    {
    case UP:
        m_ghostSprite.move(0, TILE_SIZE);
        break;
    case DOWN:
        m_ghostSprite.move(0, -TILE_SIZE);
        break;
    case LEFT:
        m_ghostSprite.move( TILE_SIZE ,0);
        break;
    case RIGHT:
        m_ghostSprite.move(  -TILE_SIZE ,0);
        break;
    default:
        break;
    }
}

void Tank::turnLeft()
{
    m_currentDirection -=1;

    if(m_currentDirection < UP)
    {
        m_currentDirection = LEFT;
    }
	
    m_ghostSprite.rotate(-90);

    switch (m_currentDirection) {
    case UP:
        m_ghostSprite.setPosition(m_ghostSprite.getPosition().x + TANK_SIZE, m_ghostSprite.getPosition().y);
        break;
    case DOWN:
        m_ghostSprite.setPosition(m_ghostSprite.getPosition().x - TANK_SIZE, m_ghostSprite.getPosition().y);
        break;
    case LEFT:
        m_ghostSprite.setPosition(m_ghostSprite.getPosition().x, m_ghostSprite.getPosition().y - TANK_SIZE);
        break;
    case RIGHT:
        m_ghostSprite.setPosition(m_ghostSprite.getPosition().x, m_ghostSprite.getPosition().y + TANK_SIZE);
        break;
    default:
        break;
    }
}

void Tank::turnRight()
{
    m_currentDirection +=1;

    if(m_currentDirection == DIRECTIONS_COUNT)
    {
        m_currentDirection = UP;
    }

    m_ghostSprite.rotate(90);

    switch (m_currentDirection)
    {
    case UP:
        m_ghostSprite.setPosition(m_ghostSprite.getPosition().x, m_ghostSprite.getPosition().y + TANK_SIZE);
        break;
    case DOWN:
        m_ghostSprite.setPosition(m_ghostSprite.getPosition().x, m_ghostSprite.getPosition().y - TANK_SIZE );
        break;
    case LEFT:
        m_ghostSprite.setPosition(m_ghostSprite.getPosition().x + TANK_SIZE , m_ghostSprite.getPosition().y);
        break;
    case RIGHT:
        m_ghostSprite.setPosition(m_ghostSprite.getPosition().x - TANK_SIZE , m_ghostSprite.getPosition().y);
        break;
    default:
        break;
    }

}

void Tank::setPosition(const StartPosition &position)
{
    m_ghostSprite.setPosition(position.position.x * TILE_SIZE, position.position.y * TILE_SIZE);
    m_sprite.setPosition(position.position.x * TILE_SIZE, position.position.y * TILE_SIZE);

    switch (position.direction)
    {
    case UP:
        m_ghostSprite.rotate(180);
        m_sprite.rotate(180);
        m_currentDirection = Direction::UP;
        break;
    case DOWN:
        m_currentDirection = Direction::DOWN;
        break;
    case LEFT:
        m_ghostSprite.rotate(90);
        m_sprite.rotate(90);
        m_currentDirection = Direction::LEFT;
        m_ghostSprite.move(TILE_SIZE, 0);
        m_sprite.move(TILE_SIZE, 0);
        break;
    case RIGHT:
        m_ghostSprite.rotate(-90);
        m_sprite.rotate(-90);
        m_currentDirection = Direction::RIGHT;
        m_ghostSprite.move(0, TILE_SIZE);
        m_sprite.move(0, TILE_SIZE);
        break;
    default:
        break;
    }

}

void Tank::setTexture(const sf::Texture &texture)
{
    m_sprite.setTexture(texture);
}

sf::Vector2i Tank::getPosition() const
{
    switch (m_currentDirection)
    {
    case UP:
        return(sf::Vector2i(m_ghostSprite.getPosition().x / TANK_SIZE - 1 , m_ghostSprite.getPosition().y / TANK_SIZE - 1));
        break;
    case DOWN:
        return(sf::Vector2i(m_ghostSprite.getPosition().x / TANK_SIZE , m_ghostSprite.getPosition().y / TANK_SIZE));
        break;
    case LEFT:
        return(sf::Vector2i(m_ghostSprite.getPosition().x / TANK_SIZE - 1 , m_ghostSprite.getPosition().y / TANK_SIZE));
        break;
    case RIGHT:
        return(sf::Vector2i(m_ghostSprite.getPosition().x / TANK_SIZE , m_ghostSprite.getPosition().y / TANK_SIZE - 1));
        break;
    default:
        return(sf::Vector2i(-1,-1));
        break;
    }
}

int Tank::getCurrentDirection() const
{
    return m_currentDirection;
}

void Tank::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}

void Tank::update(float elapsed)
{
    float startingRotation = m_sprite.getRotation();
    float targetRotation = m_ghostSprite.getRotation();
	float diffRotation = targetRotation - startingRotation;

	if (diffRotation == -270 || diffRotation == 90 || diffRotation == 270 || diffRotation == -90)
	{
        m_sprite.setRotation(m_ghostSprite.getRotation());
        m_sprite.setPosition(m_ghostSprite.getPosition().x, m_ghostSprite.getPosition().y);
	}

    sf::Vector2f startingPosition(m_sprite.getPosition());
    sf::Vector2f destinationPosition(m_ghostSprite.getPosition());

	sf::Vector2f distanceVector = destinationPosition - startingPosition;

	float distanceLen = sqrt((destinationPosition.x - startingPosition.x) * (destinationPosition.x - startingPosition.x) + (destinationPosition.y - startingPosition.y) * (destinationPosition.y - startingPosition.y));
	distanceVector /= distanceLen;

	if (distanceLen <= 1) 
	{
		return;
	}

	float deltaDist;
	if (distanceLen > 10) 
	{
		deltaDist = elapsed * distanceLen * 4;
	}
	else 
	{
		deltaDist = elapsed * 5 * 3;
	}

    m_sprite.setPosition(startingPosition + distanceVector * deltaDist);

}
