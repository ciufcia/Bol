#include "RigidbodyPhysics.hpp"

#include "Rigidbody.hpp"
#include "Renderable.hpp"
#include "CircleCollider.hpp"

#include <secs.hpp>

float sgn(float i) {
    if (i > 0.f) return 1.f;
    if (i < 0.f) return -1.f;
    return 0.f;
}

void bol::RigidbodyPhysics::update() {
    for (secs::Entity entity : getEntities()) {
        auto rigidbody = getECS()->getComponent<bol::Rigidbody>(entity);
        auto renderable = getECS()->getComponent<bol::Renderable>(entity);
        auto collider = getECS()->getComponent<bol::CircleCollider>(entity);

        rigidbody->acceleration = rigidbody->force / rigidbody->mass;

        rigidbody->velocity += rigidbody->acceleration;

        rigidbody->velocity.x -= rigidbody->velocity.x * 0.001f;

        if (rigidbody->velocity.x > 10.f)
            rigidbody->velocity.x = 10.f;
        else if (rigidbody->velocity.x < -10.f)
            rigidbody->velocity.x = -10.f;
        if (rigidbody->velocity.y > 25.f)
            rigidbody->velocity.y = 25.f;
        else if (rigidbody->velocity.y < -25.f)
            rigidbody->velocity.y = -25.f;

        if (abs(rigidbody->velocity.x) < 0.01f)
            rigidbody->velocity.x = 0.f;

        rigidbody->position += rigidbody->velocity;

        renderable->sprite.setPosition(rigidbody->position);
        collider->center = rigidbody->position;

        rigidbody->force = {};
    }
}