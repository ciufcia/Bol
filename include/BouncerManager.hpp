#pragma once

#include <vector>
#include <random>

#include <SFML/Graphics.hpp>
#include <secs.hpp>

namespace bol
{
    const float VERTICAL_DISTANCE_BETWEEN_BOUNCERS = 200.f;

    class BouncerManager
    {
        public:

            BouncerManager(secs::SECS *ecs);
            ~BouncerManager();

            void setup();
            void update(const sf::View &view);

        private:

            secs::SECS *ecs = nullptr;

            secs::Entity createBouncer(const sf::Vector2f &position);            
    
            std::vector<float> bouncerXCoordinates {50.f, 100.f, 150.f, 200.f, 250.f, 300.f};
            std::vector<float> availableBouncerXCoordinates {};

            std::vector<secs::Entity> bouncers {};
            float lastBouncerXPos = -1.f;
            float lastBouncerYPos = 550.f;

            sf::Texture bouncerIdleTexture;

            std::mt19937 rng;
    };
}