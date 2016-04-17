#include "resource_manager.hpp"
#include "gravity_block.hpp"
#include "game.hpp"

const unsigned int GravityBlock::TYPE_ID = 3;

GravityBlock::GravityBlock() : Block(), used(false) {}
GravityBlock::~GravityBlock() {}

Object::Ptr GravityBlock::clone() const {
    return Object::Ptr(new GravityBlock(*this));
}

void GravityBlock::prepareDraw(ResourceManager& resources) {
    Block::prepareDraw(resources);
    std::string texture_name = "gravity_block_";

    switch (gravity_direction) {
    case GravityDirection::NORTH:
        texture_name += "north";
        break;

    case GravityDirection::EAST:
        texture_name += "east";
        break;

    case GravityDirection::SOUTH:
        texture_name += "south";
        break;

    case GravityDirection::WEST:
        texture_name += "west";
        break;
    }

    sprite.setTexture(resources.getTexture(texture_name + ".tga"));
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
}
