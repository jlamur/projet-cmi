#include "block.hpp"

void Block::draw(sf::RenderWindow& window) {
    if (charge > 0) {
        shape.setFillColor(sf::Color(0, 0, 255));
    } else if (charge < 0) {
        shape.setFillColor(sf::Color(255, 0, 0));
    } else {
        shape.setFillColor(sf::Color(0, 0, 0));
    }

    shape.setPosition(position);
    window.draw(shape);
}

void Block::update(State state) {
    // rien à mettre à jour
}
