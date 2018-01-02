#include "tilemap.h"
#include <fstream>

TileMap::TileMap()
{
	sizeY = 17;
	sizeX = 17;
}

TileMap::~TileMap()
{

}

void TileMap::loadFile()
{
    std::ifstream file("graphics/map.txt");

    const int tilesCount = 289;

    if(file.is_open())
    {
        for(int i = 0; i < tilesCount; ++i)
        {
                file >> m_tiles[i];
        }
    }
    file.close();
}

bool TileMap::load(const std::string& tileset, sf::Vector2u tileSize, unsigned int width, unsigned int height)
{

    if(!m_tileset.loadFromFile(tileset))
    {
        return false;
    }

    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width * height * 4);

    for(unsigned int i = 0; i < width; ++i)
    {
        for(unsigned int j = 0; j < height; ++j)
        {
            int tileNumber = m_tiles[i + j * width];

            int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
            int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

            sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

            quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);


        }
    }
    return true;
}

const int &TileMap::getTileNumber(const int& row, const int& col) const
{
    return m_tiles[row * 17 + col];
}

const int* TileMap::getTileMap() {
	return m_tiles;
}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &m_tileset;
    target.draw(m_vertices, states);
}

int TileMap::getSizeY() {
	return sizeY;
}

int TileMap::getSizeX() {
	return sizeX;
}

int TileMap::getTileCount() {
	return sizeY * sizeX;
}