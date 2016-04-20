#include "manager.hpp"
#include "resource_manager.hpp"
#include "states/game.hpp"
#include "objects/finish_block.hpp"

const unsigned int FinishBlock::TYPE_ID = 4;

FinishBlock::FinishBlock() : Block() {
    icon_sprite.setOrigin(sf::Vector2f(23, 41));
    icon_sprite.setTexture(*ResourceManager::get().getTexture(
        "objects/finish_block.tga"
    ));
}

FinishBlock::~FinishBlock() {}

Object::Ptr FinishBlock::clone() const {
    return Object::Ptr(new FinishBlock(*this));
}

void FinishBlock::draw(Level& level) {
    // on dessine le bloc normal
    Block::draw(level);

    // on dessine l'icône
    icon_sprite.setPosition(getPosition());
    level.getManager().getWindow().draw(icon_sprite);
}

void FinishBlock::activate(Game& game, Object::Ptr object) {
    Block::activate(game, object);

    // si un joueur touche le bloc d'arrivée,
    // on le tue et s'il ne reste plus de joueur, on a gagné
    if (object->getTypeId() == Player::TYPE_ID) {
        if (game.getPlayers().size() == 1) {
            game.setMode(Game::Mode::WON);
        }

        game.kill(object);
    }
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
