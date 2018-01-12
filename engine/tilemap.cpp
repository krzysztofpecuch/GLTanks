#include "tilemap.h"
#include <fstream>
#include <iostream>

const int sizeX = 17;
const int sizeY = 17;
const int TILE_SIZE = 50;

void TileMap::loadTextMap()
{
    std::ifstream file("graphics/map.txt");

    if(file.is_open())
    {
        for(int i = 0; i < TILES_COUNT; ++i)
        {
            file >> m_tiles[i];
        }
    }
    file.close();
}

bool TileMap::loadTileMap(const std::string& filePath, const sf::Vector2u &tileSize, unsigned int width, unsigned int height)
{
    if(!m_tileset.loadFromFile(filePath))
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

//            int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
//            int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);
            int tv = 0;
            int tu = 0;
            if(tileNumber > 0)
            {
                tu = 1;
            }
            else
            {
                tu = 0;
            }

            m_quad = &m_vertices[(i + j * width) * 4];

            m_quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            m_quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            m_quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            m_quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

            m_quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            m_quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            m_quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            m_quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }
    }
    return true;
}

int TileMap::getTileNumber(const int& row, const int& col) const
{
    return m_tiles[row * sizeX + col];
}

void TileMap::updateMap(const int &row, const int &col)
{

    if(m_tiles[row * sizeX + col] > 0)
    {
        m_tiles[row * sizeX + col] -= 1;
    }
    std::cout << m_tiles[row * sizeX + col] << std::endl;
    if(m_tiles[row * sizeX + col] == 0)
    {
        m_quad = &m_vertices[(row * sizeX + col) * 4];
        m_quad[0].position = sf::Vector2f(col * TILE_SIZE, row * TILE_SIZE);
        m_quad[1].position = sf::Vector2f((col + 1) * TILE_SIZE, row * TILE_SIZE);
        m_quad[2].position = sf::Vector2f((col + 1) * TILE_SIZE, (row + 1) * TILE_SIZE);
        m_quad[3].position = sf::Vector2f(col * TILE_SIZE, (row + 1) * TILE_SIZE);

        m_quad[0].texCoords = sf::Vector2f(0 * TILE_SIZE, 0 * TILE_SIZE);
        m_quad[1].texCoords = sf::Vector2f((0 + 1) * TILE_SIZE, 0 * TILE_SIZE);
        m_quad[2].texCoords = sf::Vector2f((0 + 1) * TILE_SIZE, (0 + 1) * TILE_SIZE);
        m_quad[3].texCoords = sf::Vector2f(0 * TILE_SIZE, (0 + 1) * TILE_SIZE);
    }

   // std::cout << getTileNumber(16, 16) << std::endl;

}

const int* TileMap::getTileMap()
{
    return m_tiles;
}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &m_tileset;
    target.draw(m_vertices, states);
}

int TileMap::getSizeY() const
{
    return sizeY;
}

int TileMap::getSizeX() const
{
    return sizeX;
}

int TileMap::getTileCount() const
{
    return sizeY * sizeX;
}
