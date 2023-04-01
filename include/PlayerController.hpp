#pragma once

#include <SFML/Audio.hpp>
#include <secs.hpp>

namespace bol
{
    class PlayerController : public secs::System
    {
        public:

            PlayerController();
            void update(float deltaTime);
            void fixedUpdate(float screenWidth);

        private:

            sf::SoundBuffer jumpingSoundBuffer {};
            sf::Sound jumpingSound {};
    };
}