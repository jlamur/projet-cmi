#include "manager.hpp"
#include "resource_manager.hpp"
#include "utility.hpp"
#include "switch_block.hpp"
#include "game.hpp"

const unsigned int SwitchBlock::TYPE_ID = 6;

SwitchBlock::SwitchBlock() : Block(), opacity(255), used(false) {
    icon_sprite.setOrigin(sf::Vector2f(23, 23));
    icon_sprite.setTexture(*ResourceManager::get().getTexture(
        "objects/switch_block.tga"
    ));
}

SwitchBlock::~SwitchBlock() {}

Object::Ptr SwitchBlock::clone() const {
    return Object::Ptr(new SwitchBlock(*this));
}

void SwitchBlock::draw(Level& level) {
    // on dessine le bloc normal
    Block::draw(level);

    // on anime l'opacité de l'icône
    opacity = Utility::animateValue(opacity, 5, used ? 0 : 255);
    icon_sprite.setColor(sf::Color(255, 255, 255, opacity));

    // on dessine l'icône
    icon_sprite.setPosition(getPosition());
    level.getManager().getWindow().draw(icon_sprite);
}

void SwitchBlock::activate(Game& game, Object::Ptr object) {
    // on ne peut utiliser le bloc qu'une seule fois
    if (used) {
        return;
    }

    Block::activate(game, object);

    // on échange la polarité de l'objet en contact
    object->setCharge(-object->getCharge());
    used = true;
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
