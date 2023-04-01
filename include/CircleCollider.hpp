#pragma once

#include <SFML/Graphics.hpp>

namespace bol
{
    struct CircleCollider
    {
        sf::Vector2f center {};
        float radius = 0.f;

        bool checkIfIsColliding(const CircleCollider &other) const;
        sf::Vector2f getMTV(const CircleCollider &other) const;
    };
}