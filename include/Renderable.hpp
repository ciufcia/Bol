#pragma once

#include <SFML/Graphics.hpp>

namespace bol
{
    struct Renderable
    {
        sf::Sprite sprite {};
        sf::Texture texture;

        void load(const char *filepath);
    };
}