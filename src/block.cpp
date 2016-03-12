#include "block.hpp"

void Block::draw(sf::RenderWindow& window) {
	// chargement de la texture de test
	if (!texture.loadFromFile("./Images/texture_test.jpeg")) {
    	// erreur
	}

	shape.setTexture(&texture);

    if (charge > 0) {
        shape.setFillColor(sf::Color(0, 0, 255));
    } else if (charge < 0) {
        shape.setFillColor(sf::Color(255, 0, 0));
    } else {
        shape.setFillColor(sf::Color(75, 75, 75));
    }

    shape.setPosition(position);
    window.draw(shape);
}

void Block::update(State state) {
    // rien à mettre à jour
}
