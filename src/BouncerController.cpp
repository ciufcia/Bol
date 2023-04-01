#include "BouncerController.hpp"

#include "Bouncer.hpp"
#include "Renderable.hpp"

void bol::BouncerController::update(float deltaTime) {
    for (secs::Entity entity : getEntities()) {

        auto bouncer = getECS()->getComponent<bol::Bouncer>(entity);
        auto renderable = getECS()->getComponent<bol::Renderable>(entity);

        if (bouncer->hit) {
            renderable->sprite.setTexture(activeTexture);
            bouncer->activatedTime = 0.f;
            bouncer->isActivated = true;
            bouncer->hit = false;
        }

        if (bouncer->isActivated) {
            if (bouncer->activatedTime >= 0.1f) {
                bouncer->isActivated = false;
                renderable->sprite.setTexture(idleTexture);
            } else {
                bouncer->activatedTime += deltaTime;
            }
        }
    }
}