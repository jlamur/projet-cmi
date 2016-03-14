#include "block.hpp"
#include "constants.hpp"

Block::Block(float x, float y) : Object(x, y), shape(sf::Vector2f(Constants::GRID, Constants::GRID)) {
    shape.setOrigin(sf::Vector2f(Constants::GRID / 2, Constants::GRID / 2));
}

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

void Block::update(EngineState& state) {
    // rien à mettre à jour
}

std::unique_ptr<sf::FloatRect> Block::getAABB() {
    return std::unique_ptr<sf::FloatRect>(new sf::FloatRect(
        position.x - Constants::GRID / 2,
        position.y - Constants::GRID / 2,
        Constants::GRID, Constants::GRID
    ));
}
