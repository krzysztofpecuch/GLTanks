#include "tank.h"
#include "resources.h"
#include <random>

Tank::Tank(sf::Vector2i tile)
{
    std::cout<<  tile.x << " " << tile.y << std::endl;
    //m_sprite.setTexture(*tankTexture);

    m_sprite.setPosition(tile.x * TILE_SIZE ,tile.y * TILE_SIZE);

    m_currnetDir = Directions::DOWN;

}

Tank::~Tank() {
	//if (textureImage != nullptr)
	//	delete textureImage;
	//if (tankTexture != nullptr)
	//	delete tankTexture;
}

Tank::Tank(StartPosition initPosition)
{
	//New for separate textures
	textureImage = new sf::Image();
	tankTexture = new sf::Texture();
	
	*textureImage = Resources::getTexture(TextureType::Tank).copyToImage();

	//Random and replace colors of original texture
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, 255);
	
	sf::Vector2u size = textureImage->getSize();
	sf::Color randomizedBodyColor(dist(rng), dist(rng), dist(rng), 255);
	sf::Color gunColor(255 - randomizedBodyColor.r, 255 - randomizedBodyColor.g, 255 - randomizedBodyColor.b, 255);
	for (int w = 0; w < size.x; w++)
	{
		for (int h = 0; h < size.y; h++)
		{
			sf::Color currentColor = textureImage->getPixel(w, h);
			if (currentColor == sf::Color(237, 28, 36, 255)) {
				textureImage->setPixel(w, h, randomizedBodyColor);
			} else {
				textureImage->setPixel(w, h, gunColor);
			}
		}
	}
	
	//Initialize texture from new randomized Image
	tankTexture->loadFromImage(*textureImage);
	m_sprite.setTexture(*tankTexture);
	
	//Same old
    m_sprite.setPosition(initPosition.position.x * TILE_SIZE ,initPosition.position.y * TILE_SIZE);

    switch (initPosition.direction) {
    case UP:
        m_sprite.rotate(180);
        m_currnetDir = Directions::UP;
        break;
    case DOWN:
        m_currnetDir = Directions::DOWN;
        break;
    case LEFT:
        m_sprite.rotate(90);
        m_currnetDir = Directions::LEFT;
        m_sprite.move( TILE_SIZE ,0);
        break;
    case RIGHT:
        m_sprite.rotate(-90);
        m_currnetDir = Directions::RIGHT;
        m_sprite.move( 0 , TILE_SIZE);
        break;
    default:
        break;
    }
}

void Tank::shot()
{

}

void Tank::moveStraight()
{
    switch (m_currnetDir) {
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
    switch (m_currnetDir) {
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
    m_currnetDir -=1;
    if(m_currnetDir < UP)
    {
        m_currnetDir = LEFT;
    }

    m_sprite.rotate(-90);

    switch (m_currnetDir) {
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
    m_currnetDir +=1;
    if(m_currnetDir == NUMBER_DIR_STATES)
    {
        m_currnetDir = UP;
    }

    m_sprite.rotate(90);

    switch (m_currnetDir) {
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

sf::Vector2i Tank::getPosition() const
{
    switch (m_currnetDir) {
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

int Tank::getCurrentDir() const
{
    return m_currnetDir;
}

void Tank::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_sprite);
}
