#include "block.hpp"

void Block::draw(sf::RenderWindow& window) {
    shape.setPosition(position);
    window.draw(shape);
}

void Block::update(State state) {
    // rien à mettre à jour
}
