#include "manager.hpp"
#include "switch_block.hpp"
#include "game.hpp"

const unsigned int SwitchBlock::TYPE_ID = 6;

SwitchBlock::SwitchBlock() : Block() {}
SwitchBlock::~SwitchBlock() {}

Object::Ptr SwitchBlock::clone() const {
    return Object::Ptr(new SwitchBlock(*this));
}

void SwitchBlock::prepareDraw(ResourceManager& resources) {
    Block::prepareDraw(resources);
    sprite.setTexture(resources.getTexture("switch_block.tga"));
}

void SwitchBlock::activate(Game& game, Object::Ptr object) {
    Block::activate(game, object);

    // on échange la polarité de l'objet en contact, si le dernier
    // objet touché par la balle n'est pas ce bloc et si un temps
    // d'une seconde est passé
    sf::Time current_time = game.getManager().getCurrentTime();

    if (current_time - last_activation >= sf::seconds(1) &&
            object->getLastActivator().lock() != shared_from_this()) {
        last_activation = current_time;
        object->setCharge(-object->getCharge());
    }
}

unsigned int SwitchBlock::getTypeId() const {
    return TYPE_ID;
}

void SwitchBlock::init(std::ifstream& file, Object::Ptr object) {
    // lecture des propriétés d'un bloc
    Block::init(file, object);
}

Object::Ptr SwitchBlock::load(std::ifstream& file) {
    Object::Ptr object = Object::Ptr(new SwitchBlock);
    SwitchBlock::init(file, object);
    return object;
}

void SwitchBlock::save(std::ofstream& file) const {
    // écriture des propriétés d'un bloc
    Block::save(file);
}
