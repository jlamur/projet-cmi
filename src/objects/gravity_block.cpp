#include "manager.hpp"
#include "resource_manager.hpp"
#include "utility.hpp"
#include "states/game.hpp"
#include "objects/gravity_block.hpp"

const unsigned int GravityBlock::TYPE_ID = 3;

GravityBlock::GravityBlock() : Block(), opacity(255), used(false) {
    icon_sprite.setOrigin(sf::Vector2f(23, 23));
}

GravityBlock::~GravityBlock() {}

Object::Ptr GravityBlock::clone() const {
    return Object::Ptr(new GravityBlock(*this));
}

void GravityBlock::draw(Level& level) {
    // on dessine le bloc normal
    Block::draw(level);

    // on anime l'opacité de l'icône
    opacity = Utility::animateValue(opacity, 5, used ? 0 : 255);
    icon_sprite.setColor(sf::Color(255, 255, 255, opacity));

    // on dessine l'icône
    icon_sprite.setPosition(getPosition());
    level.getManager().getWindow().draw(icon_sprite);
}

void GravityBlock::activate(Game& game, Object::Ptr object) {
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

unsigned int GravityBlock::getTypeId() const {
    return TYPE_ID;
}

void GravityBlock::init(std::ifstream& file, Object::Ptr object) {
    GravityBlock::Ptr gravity_block = std::dynamic_pointer_cast<GravityBlock>(object);

    // lecture de la direction de la gravité
    char gravity_direction;
    file.read(&gravity_direction, 1);
    gravity_block->setGravityDirection((GravityDirection) gravity_direction);

    // lecture des propriétés d'un bloc
    Block::init(file, object);
}

Object::Ptr GravityBlock::load(std::ifstream& file) {
    Object::Ptr object = Object::Ptr(new GravityBlock);
    GravityBlock::init(file, object);
    return object;
}

void GravityBlock::save(std::ofstream& file) const {
    // écriture de la direction de la gravité
    char write_gravity_direction = (char) gravity_direction;
    file.write(&write_gravity_direction, 1);

    // écriture des propriétés d'un bloc
    Block::save(file);
}

GravityDirection GravityBlock::getGravityDirection() const {
    return gravity_direction;
}

void GravityBlock::setGravityDirection(GravityDirection set_gravity_direction) {
    gravity_direction = set_gravity_direction;

    // sélectionne le sprite d'icône selon la direction
    std::string texture;

    switch (gravity_direction) {
    case GravityDirection::NORTH:
        texture = "north";
        break;

    case GravityDirection::EAST:
        texture = "east";
        break;

    case GravityDirection::SOUTH:
        texture = "south";
        break;

    case GravityDirection::WEST:
        texture = "west";
        break;
    }

    icon_sprite.setTexture(*ResourceManager::get().getTexture(
        "objects/gravity_block_" + texture + ".tga"
    ));
}
