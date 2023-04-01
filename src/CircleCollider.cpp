#include "CircleCollider.hpp"

#include <cmath>

#include <SFML/Graphics.hpp>

float magnitudeSquared(sf::Vector2f vec) {
    return vec.x * vec.x + vec.y * vec.y;
}

float magnitude(sf::Vector2f vec) {
    return std::sqrtf(magnitudeSquared(vec));
}

bool bol::CircleCollider::checkIfIsColliding(const bol::CircleCollider &other) const {
    sf::Vector2f distanceVector = center - other.center;

    if (magnitudeSquared(distanceVector) < (float)(std::pow(radius + other.radius, 2)))
        return true;

    return false;
}

sf::Vector2f bol::CircleCollider::getMTV(const bol::CircleCollider &other) const {
    sf::Vector2f distanceVector = center - other.center;

    if (distanceVector == sf::Vector2f(0.f, 0.f)) {
        if (radius > other.radius)
            return sf::Vector2f(1.f, 0.f) * radius;
        else
            return sf::Vector2f(1.f, 0.f) * other.radius;
    }

    float distance = magnitude(distanceVector);

    float sumOfRadii = radius + other.radius;

    if (!(distance < sumOfRadii))
        return {};

    float collisionDepth = sumOfRadii - distance;

    return (distanceVector / distance) * collisionDepth;
}
