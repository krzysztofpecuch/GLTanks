#include "resources.h"

#include <vector>
#include <random>

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
    m_originalTankTexture.loadFromFile(texturesPath + "tank" + texturesExtention);

    int m_texturesCount = static_cast<int>(TextureType::Count);

    for (int i = 0; i < m_texturesCount; ++i)
    {
        sf::Texture texture = generateTankTexture();
        m_textures.insert(std::pair<TextureType, sf::Texture>(textureTypes[i], texture));
    }

    int fontCount = static_cast<int>(FontType::Count);

    for (int i = 0; i < fontCount; ++i)
    {
        sf::Font font;
        std::string path = fontsPath + fontFileNames[i] + fontsExtention;

        if (!font.loadFromFile(path))
        {
//            qDebug() << "Can't load texture: " << path.c_str();
        }

        m_fonts.insert(std::pair<FontType, sf::Font>(fontTypes[i], font));
    }


}

Resources::~Resources() {
    for (unsigned i = 0; i < loadedImages.size(); i++)
	{
		delete loadedImages[i];
	}
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
	sf::Image *textureImage = new sf::Image();
	loadedImages.push_back(textureImage);
	*textureImage = m_resources.m_originalTankTexture.copyToImage();

    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 255);

    sf::Vector2u size = textureImage->getSize();
    sf::Color randomizedBodyColor(dist(rng), dist(rng), dist(rng), 255);
    sf::Color gunColor(255 - randomizedBodyColor.r, 255 - randomizedBodyColor.g, 255 - randomizedBodyColor.b, 255);
    for (unsigned int w = 0; w < size.x; w++)
    {
        for (unsigned int h = 0; h < size.y; h++)
        {
            sf::Color currentColor = textureImage->getPixel(w, h);
            if (currentColor == sf::Color(237, 28, 36, 255)) {
                textureImage->setPixel(w, h, randomizedBodyColor);
            } else {
                textureImage->setPixel(w, h, gunColor);
            }
        }
    }
    sf::Texture texture;
    texture.loadFromImage(*textureImage);

    return texture;
}
