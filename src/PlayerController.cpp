#include "PlayerController.hpp"

#include "Rigidbody.hpp"
#include "Renderable.hpp"

#include "CircleCollider.hpp"

#include "Bouncer.hpp"

#include <SFML/Graphics.hpp>
#include <secs.hpp>

sf::Vector2f normalized(sf::Vector2f vec) {
    float magnitude = std::sqrtf(vec.x * vec.x + vec.y * vec.y);
    return vec / magnitude;
}

const sf::Vector2f DEFAULT_PLAYER_SCALE {4.f, 4.f};

bol::PlayerController::PlayerController() {
    jumpingSoundBuffer.loadFromFile("assets/jump.wav");
    jumpingSound.setBuffer(jumpingSoundBuffer);
}

void bol::PlayerController::update(float deltaTime) {
    for (secs::Entity entity : getEntities()) {
        auto rigidbody = getECS()->getComponent<bol::Rigidbody>(entity);
        auto renderable = getECS()->getComponent<bol::Renderable>(entity);

        if (rigidbody->velocity.y > 0.f)
            renderable->sprite.setScale({DEFAULT_PLAYER_SCALE.x, DEFAULT_PLAYER_SCALE.y + 0.05f * rigidbody->velocity.y});
        else
            renderable->sprite.setScale(DEFAULT_PLAYER_SCALE);
    }
}

void bol::PlayerController::fixedUpdate(float screenWidth) {
    for (secs::Entity entity : getEntities()) {
        auto rigidbody = getECS()->getComponent<bol::Rigidbody>(entity);
        auto renderable = getECS()->getComponent<bol::Renderable>(entity);
        auto collider = getECS()->getComponent<bol::CircleCollider>(entity);

        secs::Archetype archetype {};

        archetype.setComponent(getECS()->getComponentId<bol::CircleCollider>(), true);

        rigidbody->force.y = .25f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            rigidbody->force.x = -.25f;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            rigidbody->force.x = .25f;

        for (secs::Entity e : getECS()->getEntities(archetype)) {
            if (e == entity)
                continue;

            auto otherCollider = getECS()->getComponent<bol::CircleCollider>(e);

            if (collider->checkIfIsColliding(*otherCollider)) {
                if (getECS()->checkIfHasComponent<bol::Bouncer>(e)) {
                    jumpingSound.play();
                    getECS()->getComponent<bol::Bouncer>(e)->hit = true;
                }

                rigidbody->velocity = {};
                sf::Vector2f MTV = collider->getMTV(*otherCollider);
                rigidbody->position += MTV;
                rigidbody->force += normalized(MTV) * 12.f;
            }
        }

        if (rigidbody->position.x - 32.f < 0.f) {
            rigidbody->position.x = 32.f;
            rigidbody->velocity.x = 0.f;
            rigidbody->force.x = 0.f;
        }
        else if (rigidbody->position.x + 32.f > screenWidth) {
            rigidbody->position.x = screenWidth - 32.f;
            rigidbody->velocity.x = 0.f;
            rigidbody->force.x = 0.f;
        }

        renderable->sprite.setPosition(rigidbody->position);
        collider->center = rigidbody->position;
    }
}