#pragma once

#include <SFML/Graphics.hpp>

namespace bol
{
    struct Rigidbody
    {
        sf::Vector2f position {};
        sf::Vector2f velocity {};
        sf::Vector2f acceleration {};
        sf::Vector2f force {};

        float mass = 1.f;
    };
}