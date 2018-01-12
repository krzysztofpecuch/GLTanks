#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>

const int TILES_COUNT = 289;

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    bool loadTileMap(const std::string& filePath, const sf::Vector2u& tileSize, unsigned int width, unsigned int height);
    void loadTextMap();

	const int* getTileMap();
    int getSizeX() const;
    int getSizeY() const;
    int getTileCount() const;
    int getTileNumber(const int& row, const int& col) const;
    void updateMap(const int& row, const int& col);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    int m_tiles[TILES_COUNT];
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    sf::Vertex* m_quad;
};

#endif // TILEMAP_H
