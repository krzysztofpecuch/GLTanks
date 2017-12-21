#include "resources.h"

#include <vector>

#include <QDebug>

const std::string texturesPath = "";
const std::string texturesExtention = ".png";
const std::vector<std::string> fileNames = {"tank"};
const std::vector<TextureType> types = {TextureType::Tank};


Resources Resources::m_resources = Resources();

Resources::Resources()
{
    m_texturesCount = static_cast<int>(TextureType::Count);

    sf::Texture texture;
    std::string path;

    for (int i = 0; i < m_texturesCount; ++i)
    {
        path = texturesPath + fileNames[i] + texturesExtention;

        if (!texture.loadFromFile(path))
        {
            qDebug() << "Can't load texture: " << path.c_str();
        }

        m_textures.insert(std::pair<TextureType, sf::Texture>(types[i], texture));
    }

//    if (!m_font.loadFromFile("fonts/LuckiestGuy.ttf"))
//    {
//        qDebug() << "Can't load font!";
//    }
}

sf::Texture& Resources::getTexture(const TextureType &type)
{
    return m_resources.m_textures.at(type);
}

sf::Font &Resources::getFont()
{
    return m_resources.m_font;
}
