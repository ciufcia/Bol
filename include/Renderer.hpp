#pragma once

#include <SFML/Graphics.hpp>
#include <secs.hpp>

namespace bol
{
    class Renderer : public secs::System
    {
        public:

            void render(sf::RenderWindow &window);
    };
}