#include "manager.hpp"
#include "resource_manager.hpp"
#include "states/game.hpp"
#include "objects/geometry.hpp"
#include "objects/player.hpp"
#include "objects/block.hpp"

const unsigned int Block::TYPE_ID = 2;

LevelData::ObjectPtr Block::load(std::ifstream& file) {
    auto object = std::shared_ptr<Block>(new Block);
    object->read(file);
    return object;
}

Block::Block(sf::Vector2f position) : Object(position) {
    sprite.setOrigin(sf::Vector2f(23, 23));
}

LevelData::ObjectPtr Block::clone() const {
    return LevelData::ObjectPtr(new Block(*this));
}

void Block::draw(sf::RenderWindow& window) {
    // récupération de la texture correspondant au type de bloc
    std::string texture_name = "block.tga";

    if (getMass() > 0) {
        texture_name = "movable_" + texture_name;
    }

    if (isSelected()) {
        texture_name = "selected_" + texture_name;
    }

    sprite.setTexture(*ResourceManager::get().getTexture(
        "objects/" + texture_name
    ));

    // coloration du bloc selon sa charge
    if (getCharge() > 0) {
        sprite.setColor(sf::Color(180, 180, 255));
    } else if (getCharge() < 0) {
        sprite.setColor(sf::Color(255, 180, 180));
    } else {
        sprite.setColor(sf::Color::White);
    }

    sprite.setPosition(getPosition());
    window.draw(sprite);
}

unsigned int Block::getTypeId() const {
    return Block::TYPE_ID;
}

Geometry Block::getGeometry() const {
    return Geometry(getPosition(), sf::Vector2f(LevelData::GRID, LevelData::GRID));
}

void Block::activate(Game& game, Object& object) {
    // aucune action en cas d'activation du bloc
    // les classes filles peuvent définir leur propre action
}
