#include "Renderable.hpp"

#include <SFML/Graphics.hpp>

void bol::Renderable::load(const char *filepath) {
    if (!texture.loadFromFile(filepath))
        throw std::exception("Couldn\'t load texture");

    sprite.setTexture(texture);
}