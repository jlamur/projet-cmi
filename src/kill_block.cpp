#include "kill_block.hpp"
#include "level.hpp"

const unsigned int KillBlock::TYPE_ID = 5;

KillBlock::KillBlock() : Block() {}
KillBlock::~KillBlock() {}

Object::Ptr KillBlock::clone() const {
    return Object::Ptr(new KillBlock(*this));
}

void KillBlock::prepareDraw(ResourceManager& resources) {
    Block::prepareDraw(resources);
    sprite.setTexture(resources.getTexture("kill_block.tga"));
}

void KillBlock::activate(Level& level, Object* object) {
    Block::activate(level, object);
    // TODO: implémenter le comportmeent des blocs tueurs
}

unsigned int KillBlock::getTypeId() const {
    return TYPE_ID;
}

void KillBlock::init(std::ifstream& file, Object::Ptr object) {
    // lecture des propriétés d'un bloc
    Block::init(file, object);
}

Object::Ptr KillBlock::load(std::ifstream& file) {
    Object::Ptr object = Object::Ptr(new KillBlock);
    KillBlock::init(file, object);
    return object;
}

void KillBlock::save(std::ofstream& file) const {
    // écriture des propriétés d'un bloc
    Block::save(file);
}
