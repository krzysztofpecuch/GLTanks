#ifndef RESOURCES_H
#define RESOURCES_H

#include <SFML/Graphics.hpp>

#include <map>

enum class TextureType
{
    Tank,
    Count
};

enum class FontType
{
    Arial,
    Count
};

class Resources
{
public:
    Resources();

    static sf::Texture& getTexture(const TextureType& type);
    static sf::Font& getFont();

private:
    sf::Texture generateTankTexture();

    static Resources m_resources;
    std::map<TextureType, sf::Texture> m_textures;
    sf::Font m_font;

    sf::Texture m_originalTankTexture;
};

#endif // RESOURCES_H
