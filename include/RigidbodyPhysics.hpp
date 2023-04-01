#pragma once

#include <SFML/Graphics.hpp>
#include <secs.hpp>

namespace bol
{
    const float DRAG_COEFFICIENT = 0.9f;

    class RigidbodyPhysics : public secs::System
    {
        public:

            void update();
    };
}