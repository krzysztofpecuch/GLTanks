#include "tank.h"
#include "resources.h"
#include <random>

Tank::Tank(const StartPosition &initPosition)
{	
	//Same old
    m_sprite.setPosition(initPosition.position.x * TILE_SIZE ,initPosition.position.y * TILE_SIZE);
	m_actualSprite.setPosition(initPosition.position.x * TILE_SIZE, initPosition.position.y * TILE_SIZE);

    switch (initPosition.direction) {
    case UP:
		m_sprite.rotate(180);
		m_actualSprite.rotate(180);
        m_currentDirection = Directions::UP;
        break;
    case DOWN:
        m_currentDirection = Directions::DOWN;
        break;
    case LEFT:
		m_sprite.rotate(90);
		m_actualSprite.rotate(90);
        m_currentDirection = Directions::LEFT;
		m_sprite.move(TILE_SIZE, 0);
		m_actualSprite.move(TILE_SIZE, 0);
        break;
    case RIGHT:
		m_sprite.rotate(-90);
		m_actualSprite.rotate(-90);
        m_currentDirection = Directions::RIGHT;
		m_sprite.move(0, TILE_SIZE);
		m_actualSprite.move(0, TILE_SIZE);
        break;
    default:
        break;
    }
}

void Tank::moveStraight()
{
    switch (m_currentDirection) {
    case UP:
        m_sprite.move(0, -TILE_SIZE);
        break;
    case DOWN:
        m_sprite.move(0, TILE_SIZE);
        break;
    case LEFT:
        m_sprite.move(- TILE_SIZE ,0);
        break;
    case RIGHT:
        m_sprite.move(  TILE_SIZE ,0);
        break;
    default:
        break;
    }
}

void Tank::moveBackward()
{
    switch (m_currentDirection) {
    case UP:
        m_sprite.move(0, TILE_SIZE);
        break;
    case DOWN:
        m_sprite.move(0, -TILE_SIZE);
        break;
    case LEFT:
        m_sprite.move( TILE_SIZE ,0);
        break;
    case RIGHT:
        m_sprite.move(  -TILE_SIZE ,0);
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
	
    m_sprite.rotate(-90);

    switch (m_currentDirection) {
    case UP:
        m_sprite.setPosition(m_sprite.getPosition().x + TANK_SIZE, m_sprite.getPosition().y);
        break;
    case DOWN:
        m_sprite.setPosition(m_sprite.getPosition().x - TANK_SIZE, m_sprite.getPosition().y);
        break;
    case LEFT:
        m_sprite.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y - TANK_SIZE);
        break;
    case RIGHT:
        m_sprite.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y + TANK_SIZE);
        break;
    default:
        break;
    }
}

void Tank::turnRight()
{
    m_currentDirection +=1;
    if(m_currentDirection == NUMBER_DIR_STATES)
    {
        m_currentDirection = UP;
    }

    m_sprite.rotate(90);

    switch (m_currentDirection) {
    case UP:
        m_sprite.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y + TANK_SIZE);
        break;
    case DOWN:
        m_sprite.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y - TANK_SIZE );
        break;
    case LEFT:
        m_sprite.setPosition(m_sprite.getPosition().x + TANK_SIZE , m_sprite.getPosition().y);
        break;
    case RIGHT:
        m_sprite.setPosition(m_sprite.getPosition().x - TANK_SIZE , m_sprite.getPosition().y);
        break;
    default:
        break;
    }

}

void Tank::setPosition(const StartPosition &position)
{
    m_sprite.setPosition(position.position.x * TILE_SIZE, position.position.y * TILE_SIZE);
    m_actualSprite.setPosition(position.position.x * TILE_SIZE, position.position.y * TILE_SIZE);

    switch (position.direction) {
    case UP:
        m_sprite.rotate(180);
        m_actualSprite.rotate(180);
        m_currentDirection = Directions::UP;
        break;
    case DOWN:
        m_currentDirection = Directions::DOWN;
        break;
    case LEFT:
        m_sprite.rotate(90);
        m_actualSprite.rotate(90);
        m_currentDirection = Directions::LEFT;
        m_sprite.move(TILE_SIZE, 0);
        m_actualSprite.move(TILE_SIZE, 0);
        break;
    case RIGHT:
        m_sprite.rotate(-90);
        m_actualSprite.rotate(-90);
        m_currentDirection = Directions::RIGHT;
        m_sprite.move(0, TILE_SIZE);
        m_actualSprite.move(0, TILE_SIZE);
        break;
    default:
        break;
    }

}

void Tank::setTexture(const sf::Texture &texture)
{
    m_actualSprite.setTexture(texture);
}

sf::Vector2i Tank::getPosition() const
{
    switch (m_currentDirection) {
    case UP:
        return(sf::Vector2i(m_sprite.getPosition().x / TANK_SIZE - 1 , m_sprite.getPosition().y / TANK_SIZE - 1));
        break;
    case DOWN:
        return(sf::Vector2i(m_sprite.getPosition().x / TANK_SIZE , m_sprite.getPosition().y / TANK_SIZE));
        break;
    case LEFT:
        return(sf::Vector2i(m_sprite.getPosition().x / TANK_SIZE - 1 , m_sprite.getPosition().y / TANK_SIZE));
        break;
    case RIGHT:
        return(sf::Vector2i(m_sprite.getPosition().x / TANK_SIZE , m_sprite.getPosition().y / TANK_SIZE - 1));
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
    target.draw(m_actualSprite, states);
}

void Tank::update(float elapsed) {
	sf::Vector2f startingPosition(m_actualSprite.getPosition());
	sf::Vector2f destinationPosition(m_sprite.getPosition());

	sf::Vector2f distanceVector = destinationPosition - startingPosition;

	float distanceLen = sqrt((destinationPosition.x - startingPosition.x) * (destinationPosition.x - startingPosition.x) + (destinationPosition.y - startingPosition.y) * (destinationPosition.y - startingPosition.y));
	
	if (distanceLen <= 0.05) {
		return;
	}

	distanceVector /= distanceLen;

	float deltaDist = elapsed * 50 * 2;

	m_actualSprite.setPosition(startingPosition + distanceVector * deltaDist);

	float startingRotation = m_actualSprite.getRotation();
	float targetRotation = m_sprite.getRotation();
	float diffRotation = targetRotation - startingRotation;
	if (diffRotation != 0)
		std::cout << diffRotation << std::endl;
	
	if (diffRotation >= -1 && diffRotation <= 1) {
		return;
	}

	if (diffRotation == -270 || diffRotation == 90)
	{
		m_actualSprite.setRotation(m_sprite.getRotation());
		m_actualSprite.move(distanceVector.x * TANK_SIZE, distanceVector.y * TANK_SIZE);
	}
	
	if (diffRotation == 270 || diffRotation == -90) {
		m_actualSprite.setRotation(m_sprite.getRotation());
		m_actualSprite.move(distanceVector.x * TANK_SIZE, distanceVector.y * TANK_SIZE);
	}
}
