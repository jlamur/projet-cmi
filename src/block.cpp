#include "block.hpp"
#include "constants.hpp"

Block::Block(float x, float y) : Object(x, y),
    shape(sf::Vector2f(Constants::GRID, Constants::GRID)) {
    // par défaut, les blocs ne sont pas déplaçables
    // et ont donc une masse infinie
    setMass(0.f);

    shape.setOrigin(sf::Vector2f(Constants::GRID / 2, Constants::GRID / 2));
}

void Block::draw(sf::RenderWindow& window) {
    Object::draw(window);

	// chargement de la texture de test
	if (!texture.loadFromFile("./res/block.png")) {
    	// erreur
	}

	shape.setTexture(&texture);

    if (getCharge() > 0) {
        shape.setFillColor(sf::Color(180, 180, 255));
    } else if (getCharge() < 0) {
        shape.setFillColor(sf::Color(255, 180, 180));
    } else {
        shape.setFillColor(sf::Color::White);
    }

    shape.setPosition(getPosition());
    window.draw(shape);
}

std::unique_ptr<sf::FloatRect> Block::getAABB() {
    return std::unique_ptr<sf::FloatRect>(new sf::FloatRect(
        getPosition().x - Constants::GRID / 2,
        getPosition().y - Constants::GRID / 2,
        Constants::GRID, Constants::GRID
    ));
}
