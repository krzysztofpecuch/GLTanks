#ifndef RESOURCES_H
#define RESOURCES_H

#include <SFML/Graphics.hpp>

#include <map>

enum class TextureType
{
    Tank,
    Count
};

class Resources
{
public:
    Resources();

    static sf::Texture& getTexture(const TextureType& type);
    static sf::Font& getFont();

private:
    static Resources m_resources;
    std::map<TextureType, sf::Texture> m_textures;
    sf::Font m_font;

    int m_texturesCount;
};

#endif // RESOURCES_H
