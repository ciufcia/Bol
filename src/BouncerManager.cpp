#include "BouncerManager.hpp"
#include "CircleCollider.hpp"
#include "Bouncer.hpp"
#include "Renderable.hpp"

#include <random>

#include <SFML/Graphics.hpp>
#include <secs.hpp>

bol::BouncerManager::BouncerManager(secs::SECS *ecs) {
    this->ecs = ecs;

    bouncerIdleTexture.loadFromFile("assets/bouncer-idle.png");

    availableBouncerXCoordinates = bouncerXCoordinates;
    auto seed = std::random_device();
    rng = std::mt19937(seed());
}

bol::BouncerManager::~BouncerManager() {
    for (secs::Entity bouncer : bouncers) {
        ecs->deleteEntity(bouncer);
    }
}

secs::Entity bol::BouncerManager::createBouncer(const sf::Vector2f &position) {
    secs::Entity bouncer = ecs->createEntity();

    ecs->addComponent<bol::Bouncer>(bouncer);

    bol::CircleCollider bouncerCollider {};
    bouncerCollider.center = position;
    bouncerCollider.radius = 22.f;

    ecs->addComponent<bol::CircleCollider>(bouncer, bouncerCollider);

    ecs->addComponent<bol::Renderable>(bouncer);

    auto bouncerRenderable = ecs->getComponent<bol::Renderable>(bouncer);

    bouncerRenderable->sprite.setTexture(bouncerIdleTexture);
    bouncerRenderable->sprite.setOrigin({8.f, 8.f});
    bouncerRenderable->sprite.setScale({4.f, 4.f});
    bouncerRenderable->sprite.setPosition(position);

    return bouncer;
}

void bol::BouncerManager::setup() {

    bouncers.push_back(createBouncer({175.f, lastBouncerYPos}));
    lastBouncerYPos -= bol::VERTICAL_DISTANCE_BETWEEN_BOUNCERS;
    availableBouncerXCoordinates.erase(availableBouncerXCoordinates.begin() + 2);
    availableBouncerXCoordinates.erase(availableBouncerXCoordinates.begin() + 2);

    for (float yCoordinate = lastBouncerYPos; yCoordinate > -50.f; yCoordinate -= bol::VERTICAL_DISTANCE_BETWEEN_BOUNCERS) {
        std::uniform_int_distribution<> distribution {0, (int)(availableBouncerXCoordinates.size()) - 1};
        int chosenIndex = distribution(rng);
        float xCoordinate = availableBouncerXCoordinates[chosenIndex];

        availableBouncerXCoordinates.erase(availableBouncerXCoordinates.begin() + chosenIndex);

        if (lastBouncerXPos != -1.f)
            availableBouncerXCoordinates.push_back(lastBouncerXPos);
        else {
            availableBouncerXCoordinates.push_back(150.f);
            availableBouncerXCoordinates.push_back(200.f);
        }

        lastBouncerXPos = xCoordinate;
        lastBouncerYPos = yCoordinate;
        
        bouncers.push_back(createBouncer({xCoordinate, yCoordinate}));
    }
}

void bol::BouncerManager::update(const sf::View &view) {
    float viewHalfHeight = view.getSize().y / 2.f;
    float viewTopBound = view.getCenter().y - viewHalfHeight;
    float viewBottomBound =  view.getCenter().y + viewHalfHeight;
    float distanceToTopOfView = lastBouncerYPos - viewTopBound;

    for (float f = distanceToTopOfView; f > bol::VERTICAL_DISTANCE_BETWEEN_BOUNCERS - 50.f; f -= bol::VERTICAL_DISTANCE_BETWEEN_BOUNCERS) {
        std::uniform_int_distribution<> distribution {0, (int)(availableBouncerXCoordinates.size()) - 1};
        int chosenIndex = distribution(rng);
        float xCoordinate = availableBouncerXCoordinates[chosenIndex];

        availableBouncerXCoordinates.erase(availableBouncerXCoordinates.begin() + chosenIndex);

        if (lastBouncerXPos != -1.f)
            availableBouncerXCoordinates.push_back(lastBouncerXPos);

        float yCoordinate = lastBouncerYPos - VERTICAL_DISTANCE_BETWEEN_BOUNCERS;

        lastBouncerXPos = xCoordinate;
        lastBouncerYPos = yCoordinate;

        bouncers.push_back(createBouncer({xCoordinate, yCoordinate}));
    }

    for (int i = 0; i < bouncers.size(); i++) {
        auto renderable = ecs->getComponent<bol::Renderable>(bouncers[i]);

        if (renderable->sprite.getGlobalBounds().top > viewBottomBound + 50.f) {
            ecs->deleteEntity(bouncers[i]);
            bouncers.erase(bouncers.begin());
        } else {
            break;
        }
    }
}