#include "block.hpp"
#include "player.hpp"
#include "constants.hpp"
#include "resource_manager.hpp"

const unsigned int Block::TYPE_ID = 1;

Block::Block(float x, float y) : Object(x, y) {
    // par défaut, les blocs ne sont pas déplaçables et ont
    // donc une masse infinie, représentée par 0
    setMass(0.f);

    // déplacement de l'origine au centre du bloc
    sprite.setOrigin(sf::Vector2f(Constants::GRID / 2, Constants::GRID / 2));
}

Block::~Block() {}

std::shared_ptr<Object> Block::load(std::ifstream& file) {
    return std::shared_ptr<Object>(new Block(10, 10));
}

void Block::draw(Manager& manager) {
    Object::draw(manager);

    // utilisation de la texture
    sprite.setTexture(
        manager.getResourceManager().getTexture("block.png")
    );

    // coloration du bloc en fonction de sa charge
    if (getCharge() > 0) {
        sprite.setColor(sf::Color(180, 180, 255));
    } else if (getCharge() < 0) {
        sprite.setColor(sf::Color(255, 180, 180));
    } else {
        sprite.setColor(sf::Color::White);
    }

    sprite.setPosition(getPosition());
    manager.getWindow().draw(sprite);
}

std::unique_ptr<sf::FloatRect> Block::getAABB() const {
    return std::unique_ptr<sf::FloatRect>(new sf::FloatRect(
        getPosition().x - Constants::GRID / 2,
        getPosition().y - Constants::GRID / 2,
        Constants::GRID, Constants::GRID
    ));
}

unsigned int Block::getTypeId() const {
    return TYPE_ID;
}
