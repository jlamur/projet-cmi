#include "manager.hpp"
#include "resource_manager.hpp"
#include "utility.hpp"
#include "states/game.hpp"
#include "objects/switch_block.hpp"

const unsigned int SwitchBlock::TYPE_ID = 6;

LevelData::ObjectPtr SwitchBlock::load(std::ifstream& file) {
    auto object = std::shared_ptr<SwitchBlock>(new SwitchBlock);
    object->read(file);
    return object;
}

SwitchBlock::SwitchBlock(sf::Vector2f position) : Block(position),
    opacity(255), used(false) {
    icon_sprite.setOrigin(sf::Vector2f(23, 23));
    icon_sprite.setTexture(*ResourceManager::get().getTexture(
        "objects/switch_block.tga"
    ));
}

LevelData::ObjectPtr SwitchBlock::clone() const {
    return LevelData::ObjectPtr(new SwitchBlock(*this));
}

void SwitchBlock::draw(sf::RenderWindow& window) {
    // on dessine le bloc normal
    Block::draw(window);

    // on anime l'opacité de l'icône
    opacity = Utility::animateValue(opacity, 5, used ? 0 : 255);
    icon_sprite.setColor(sf::Color(255, 255, 255, opacity));

    // on dessine l'icône
    icon_sprite.setPosition(getPosition());
    window.draw(icon_sprite);
}

unsigned int SwitchBlock::getTypeId() const {
    return SwitchBlock::TYPE_ID;
}

void SwitchBlock::activate(Game& game, Object& object) {
    // on ne peut utiliser le bloc qu'une seule fois
    if (used) {
        return;
    }

    Block::activate(game, object);

    // on échange la polarité de l'objet en contact
    object.setCharge(-object.getCharge());
    used = true;
}
