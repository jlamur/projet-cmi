#include "block.hpp"

void Block::draw(sf::RenderWindow& window) {
	// chargement de la texture de test
	if (!texture.loadFromFile("./res/block.png")) {
    	// erreur
	}

	shape.setTexture(&texture);

    if (charge > 0) {
        shape.setFillColor(sf::Color(180, 180, 255));
    } else if (charge < 0) {
        shape.setFillColor(sf::Color(255, 180, 180));
    } else {
        shape.setFillColor(sf::Color::White);
    }

    shape.setPosition(position);
    window.draw(shape);
}

void Block::update(State state) {
    // rien à mettre à jour
}
