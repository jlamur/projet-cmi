#include "block.hpp"

void Block::draw(sf::RenderWindow& window) {
    shape.setPosition(position * 32.f);
    window.draw(shape);
}

void Block::update(State state) {
    // rien à mettre à jour
}
