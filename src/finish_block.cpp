#include "finish_block.hpp"
#include "level.hpp"

const unsigned int FinishBlock::TYPE_ID = 4;

FinishBlock::FinishBlock() : Block() {}
FinishBlock::~FinishBlock() {}

Object::Ptr FinishBlock::clone() const {
    return Object::Ptr(new FinishBlock(*this));
}

void FinishBlock::prepareDraw(ResourceManager& resources) {
    Block::prepareDraw(resources);
    sprite.setOrigin(sf::Vector2f(23, 41));
    sprite.setTexture(resources.getTexture("finish_block.tga"), true);
}

void FinishBlock::activate(Level& level, Object* object) {
    Block::activate(level, object);
    // TODO: implémenter le comportmeent des blocs d'arrivée
}

unsigned int FinishBlock::getTypeId() const {
    return TYPE_ID;
}

void FinishBlock::init(std::ifstream& file, Object::Ptr object) {
    // lecture des propriétés d'un bloc
    Block::init(file, object);
}

Object::Ptr FinishBlock::load(std::ifstream& file) {
    Object::Ptr object = Object::Ptr(new FinishBlock);
    FinishBlock::init(file, object);
    return object;
}

void FinishBlock::save(std::ofstream& file) const {
    // écriture des propriétés d'un bloc
    Block::save(file);
}
