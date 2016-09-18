#include "manager.hpp"
#include "resource_manager.hpp"
#include "utility.hpp"
#include "states/game.hpp"
#include "objects/gravity_block.hpp"

const unsigned int GravityBlock::TYPE_ID = 3;

// propriété indiquant la direction dans laquelle le bloc change la gravité
const unsigned int PROP_GRAVITY_DIRECTION = 30;
const Utility::Direction DEFAULT_GRAVITY_DIRECTION = Utility::Direction::NORTH;

LevelData::ObjectPtr GravityBlock::load(std::ifstream& file) {
    auto object = std::shared_ptr<GravityBlock>(new GravityBlock);
    object->read(file);
    return object;
}

GravityBlock::GravityBlock(sf::Vector2f position) : Block(position),
    gravity_direction(DEFAULT_GRAVITY_DIRECTION), opacity(255), used(false) {
    icon_sprite.setOrigin(sf::Vector2f(23, 23));
}

LevelData::ObjectPtr GravityBlock::clone() const {
    return LevelData::ObjectPtr(new GravityBlock(*this));
}

void GravityBlock::draw(sf::RenderWindow& window) {
    // on dessine le bloc normal
    Block::draw(window);

    // on anime l'opacité de l'icône
    opacity = Utility::animateValue(opacity, 5, used ? 0 : 255);
    icon_sprite.setColor(sf::Color(255, 255, 255, opacity));

    // on dessine l'icône
    icon_sprite.setPosition(getPosition());
    window.draw(icon_sprite);
}

unsigned int GravityBlock::getTypeId() const {
    return TYPE_ID;
}

void GravityBlock::activate(Game& game, Object& object) {
    // on ne peut utiliser le bloc qu'une seule fois
    if (used) {
        return;
    }

    Block::activate(game, object);

    // lorsque le bloc est activé, il transmet son
    // sens de gravité au niveau
    game.setGravityDirection(gravity_direction);
    used = true;
}

bool GravityBlock::readProperty(unsigned int prop_type, std::ifstream& file) {
    if (prop_type == PROP_GRAVITY_DIRECTION) {
        char gravity_direction;
        file.read(&gravity_direction, 1);
        setGravityDirection((Utility::Direction) gravity_direction);
        return true;
    }

    return false;
}

void GravityBlock::writeProperties(std::ofstream& file) const {
    if (gravity_direction != DEFAULT_GRAVITY_DIRECTION) {
        char prop_type = PROP_GRAVITY_DIRECTION;
        char value = (char) gravity_direction;
        file.write(&prop_type, 1);
        file.write(&value, 1);
    }
}

Utility::Direction GravityBlock::getGravityDirection() const {
    return gravity_direction;
}

void GravityBlock::setGravityDirection(Utility::Direction gravity_direction) {
    this->gravity_direction = gravity_direction;

    // sélectionne le sprite d'icône selon la direction
    std::string texture;

    switch (gravity_direction) {
    case Utility::Direction::NORTH:
        texture = "north";
        break;

    case Utility::Direction::EAST:
        texture = "east";
        break;

    case Utility::Direction::SOUTH:
        texture = "south";
        break;

    case Utility::Direction::WEST:
        texture = "west";
        break;
    }

    icon_sprite.setTexture(*ResourceManager::get().getTexture(
        "objects/gravity_block_" + texture + ".tga"
    ));
}
