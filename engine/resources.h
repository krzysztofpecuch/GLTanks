#ifndef RESOURCES_H
#define RESOURCES_H

#include <SFML/Graphics.hpp>

#include <map>

enum class TextureType
{
    Tank1,
    Tank2,
    Tank3,
    Tank4,
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
    static sf::Font& getFont(const FontType &type);

private:
    sf::Texture generateTankTexture();

    static Resources m_resources;
    std::map<TextureType, sf::Texture> m_textures;
    std::map<FontType, sf::Font> m_fonts;

    sf::Texture m_originalTankTexture;
};

#endif // RESOURCES_H
