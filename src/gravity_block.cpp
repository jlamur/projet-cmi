#include "gravity_block.hpp"
#include "level.hpp"

const unsigned int GravityBlock::TYPE_ID = 3;

GravityBlock::GravityBlock() : Block() {}
GravityBlock::~GravityBlock() {}

ObjectPtr GravityBlock::clone() const {
    return ObjectPtr(new GravityBlock(*this));
}

void GravityBlock::activated(Level& level, Object& object) {
    Block::activated(level, object);

    // lorsque le bloc est activé, il transmet son
    // sens de gravité au niveau
    level.setGravityDirection(gravity_direction);
}

unsigned int GravityBlock::getTypeId() const {
    return TYPE_ID;
}

ObjectPtr GravityBlock::load(std::ifstream& file) {
    ObjectPtr object = ObjectPtr(new GravityBlock);
    std::shared_ptr<GravityBlock> block = std::dynamic_pointer_cast<GravityBlock>(object);

    // lecture de la direction de la gravité
    char gravity_direction;
    file.read(&gravity_direction, 1);
    block->setGravityDirection((GravityDirection) gravity_direction);

    // lecture des propriétés communes des objets
    Object::load(file, object);
    file.seekg(1, file.cur);

    return object;
}

GravityDirection GravityBlock::getGravityDirection() const {
    return gravity_direction;
}

void GravityBlock::setGravityDirection(GravityDirection set_gravity_direction) {
    gravity_direction = set_gravity_direction;
}
