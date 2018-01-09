#include "resources.h"

#include <iostream>


const std::string texturesPath = "graphics/";
const std::string fontsPath = "fonts/";
const std::string texturesExtention = ".png";
const std::string fontsExtention = ".ttf";
const std::vector<std::string> textureFileNames = {"tank"};
const std::vector<std::string> fontFileNames = { "arial" };
const std::vector<TextureType> textureTypes = {TextureType::Tank1, TextureType::Tank2, TextureType::Tank3, TextureType::Tank4};
const std::vector<FontType> fontTypes = {FontType::Arial};


Resources Resources::m_resources = Resources();

Resources::Resources()
{
    srand(time(nullptr));
    m_randomNumberGenerator = std::mt19937(std::random_device()());
    m_range = std::uniform_int_distribution<>(0, 255);

    m_originalTankTexture.loadFromFile(texturesPath + "tank" + texturesExtention);

    int texturesCount = static_cast<int>(TextureType::Count);

    for (int i = 0; i < texturesCount; ++i)
    {
        sf::Texture texture = generateTankTexture();
        m_textures.insert(std::pair<TextureType, sf::Texture>(textureTypes[i], texture));
    }

    int fontCount = static_cast<int>(FontType::Count);

    for (int i = 0; i < fontCount; ++i)
    {
        std::string path = fontsPath + fontFileNames[i] + fontsExtention;

        sf::Font font;
        if (!font.loadFromFile(path))
        {
            std::cout << "Can't load font " << path.c_str() << std::endl;
        }

        m_fonts.insert(std::pair<FontType, sf::Font>(fontTypes[i], font));
    }
}

Resources::~Resources()
{
    m_textures.clear();
    m_fonts.clear();
}

sf::Texture& Resources::getTexture(const TextureType &type)
{
    return m_resources.m_textures.at(type);
}

sf::Font &Resources::getFont(const FontType& type)
{
    return m_resources.m_fonts.at(type);
}

sf::Texture Resources::generateTankTexture()
{
    sf::Image textureImage = m_resources.m_originalTankTexture.copyToImage();

    sf::Vector2u size = textureImage.getSize();
    sf::Color randomizedBodyColor(m_range(m_randomNumberGenerator), m_range(m_randomNumberGenerator), m_range(m_randomNumberGenerator), 255);
    sf::Color gunColor(255 - randomizedBodyColor.r, 255 - randomizedBodyColor.g, 255 - randomizedBodyColor.b, 255);
    for (unsigned int width = 0; width < size.x; width++)
    {
        for (unsigned int height = 0; height < size.y; height++)
        {
            sf::Color currentColor = textureImage.getPixel(width, height);

            if (currentColor == sf::Color(237, 28, 36, 255))
            {
                textureImage.setPixel(width, height, randomizedBodyColor);
            }
            else
            {
                textureImage.setPixel(width, height, gunColor);
            }
        }
    }

    sf::Texture texture;
    texture.loadFromImage(textureImage);

    return texture;
}
