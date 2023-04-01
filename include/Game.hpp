#pragma once

#include "Renderer.hpp"
#include "RigidbodyPhysics.hpp"
#include "PlayerController.hpp"
#include "BouncerController.hpp"

#include <secs.hpp>

namespace bol 
{
    class Game
    {
        public:

            void setup();
            void mainLoop();

        private:

            secs::SECS ecs {};

            Renderer renderer {};
            RigidbodyPhysics rigidbodyPhysics {};
            PlayerController playerController {};
            BouncerController bouncerController {};

            const char *assetsDir = "assets/";
    };
}