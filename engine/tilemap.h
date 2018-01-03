#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include <string>

const int TILES_COUNT = 289;

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    bool load(const std::string& tileset, sf::Vector2u tileSize, unsigned int width, unsigned int height);
    void loadFile();
	const int* getTileMap();
    int getSizeX() const;
    int getSizeY() const;
    int getTileCount() const;
    const int& getTileNumber(const int &row, const int &col) const;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    const int sizeX = 17;
    const int sizeY = 17;
    int m_tiles[TILES_COUNT];
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

#endif // TILEMAP_H
