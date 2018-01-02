#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include <string>


class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    TileMap();
    ~TileMap();


    bool load(const std::string& tileset, sf::Vector2u tileSize, unsigned int width, unsigned int height);
    void loadFile();
	const int* getTileMap();
	int getSizeX();
	int getSizeY();
	int getTileCount();
    const int& getTileNumber(const int &row, const int &col) const;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	int sizeX;
	int sizeY;
    int m_tiles[289];
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

#endif // TILEMAP_H
