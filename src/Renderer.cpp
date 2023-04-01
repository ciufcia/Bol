#include "Renderer.hpp"

#include "Renderable.hpp"

#include <SFML/Graphics.hpp>
#include <secs.hpp>

void bol::Renderer::render(sf::RenderWindow &window) {
    for (secs::Entity entity : getEntities()) {
        auto renderable = getECS()->getComponent<bol::Renderable>(entity);

        window.draw(renderable->sprite);
    }
}