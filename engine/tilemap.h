#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include <string>


class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    TileMap();


    bool load(const std::string& tileset, sf::Vector2u tileSize, unsigned int width, unsigned int height);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

#endif // TILEMAP_H
