#include "resources.h"

#include <vector>
#include <random>

const std::string texturesPath = "graphics/";
const std::string fontsPath = "fonts/";
const std::string texturesExtention = ".png";
const std::string fontsExtention = ".ttf";
const std::vector<std::string> textureFileNames = {"tank"};
const std::vector<std::string> fontFileNames = { "arial" };
const std::vector<TextureType> textureTypes = {TextureType::Tank};
const std::vector<FontType> fontTypes = {FontType::Arial};


Resources Resources::m_resources = Resources();

Resources::Resources()
{
    int m_texturesCount = static_cast<int>(TextureType::Count);

    sf::Texture texture;
    std::string path;

    for (int i = 0; i < m_texturesCount; ++i)
    {
        path = texturesPath + textureFileNames[i] + texturesExtention;

        if (!texture.loadFromFile(path))
        {
//            qDebug() << "Can't load texture: " << path.c_str();
        }

        m_textures.insert(std::pair<TextureType, sf::Texture>(textureTypes[i], texture));
    }

//    if (!m_font.loadFromFile("fonts/LuckiestGuy.ttf"))
//    {
//        qDebug() << "Can't load font!";
//    }

    m_originalTankTexture = m_textures.at(TextureType::Tank);


}

sf::Texture& Resources::getTexture(const TextureType &type)
{
    if (type == TextureType::Tank)
        m_resources.m_textures[type] = m_resources.generateTankTexture();

    return m_resources.m_textures.at(type);
}

sf::Font &Resources::getFont()
{
    return m_resources.m_font;
}

sf::Texture Resources::generateTankTexture()
{
    sf::Image textureImage = m_resources.m_originalTankTexture.copyToImage();

    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 255);

    sf::Vector2u size = textureImage.getSize();
    sf::Color randomizedBodyColor(dist(rng), dist(rng), dist(rng), 255);
    sf::Color gunColor(255 - randomizedBodyColor.r, 255 - randomizedBodyColor.g, 255 - randomizedBodyColor.b, 255);
    for (unsigned int w = 0; w < size.x; w++)
    {
        for (unsigned int h = 0; h < size.y; h++)
        {
            sf::Color currentColor = textureImage.getPixel(w, h);
            if (currentColor == sf::Color(237, 28, 36, 255)) {
                textureImage.setPixel(w, h, randomizedBodyColor);
            } else {
                textureImage.setPixel(w, h, gunColor);
            }
        }
    }
    sf::Texture texture;
    texture.loadFromImage(textureImage);

    return texture;
}
