#pragma once

#include <SFML/Graphics.hpp>
#include <secs.hpp>

namespace bol
{
    class BouncerController : public secs::System
    {
        public:

            void update(float deltaTime);

            sf::Texture idleTexture;
            sf::Texture activeTexture;
    };
}