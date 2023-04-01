#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <SFML/Graphics.hpp>  

namespace bol
{
    class ScoreCounter
    {
        public:

            ScoreCounter();

            sf::Vector2f getCenter() const;
            sf::Vector2f getRealSize() const;

            void setScore(float newScore);
            void incrementScore(float increment);
            void setCenter(const sf::Vector2f &newCenter);
            void move(const sf::Vector2f &offset);
    
            sf::Vector2f getDigitTextureSize();
            void setScale(const sf::Vector2f &newScale);
            int getAmountOfDigits();

            void setOpacity(unsigned char newOpacity);

            void draw(sf::RenderWindow &window);

        private:

            std::unordered_map<std::string, sf::Texture> digitTextures {};

            float score = 0.f;
            std::string scoreText = "0";

            sf::Vector2f center {4.f, 4.f};

            const sf::Vector2f textureSize {8.f, 8.f};
            sf::Vector2f scale {1.f, 1.f};
            sf::Vector2f size {8.f, 8.f};

            std::vector<sf::Sprite> digitSprites {};

            unsigned char opacity = 255u;
    };
}