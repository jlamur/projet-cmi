#include "manager.hpp"
#include "resource_manager.hpp"
#include "states/game.hpp"
#include "objects/player.hpp"
#include "objects/kill_block.hpp"

const unsigned int KillBlock::TYPE_ID = 5;

KillBlock::KillBlock() : Block() {
    icon_sprite.setOrigin(sf::Vector2f(23, 23));
    icon_sprite.setTexture(*ResourceManager::get().getTexture(
        "objects/kill_block.tga"
    ));
}

KillBlock::~KillBlock() {}

Object::Ptr KillBlock::clone() const {
    return Object::Ptr(new KillBlock(*this));
}

void KillBlock::draw(Level& level) {
    // on dessine le bloc normal
    Block::draw(level);

    // on dessine l'icône
    icon_sprite.setPosition(getPosition());
    level.getManager().getWindow().draw(icon_sprite);
}

void KillBlock::activate(Game& game, Object::Ptr object) {
    Block::activate(game, object);

    // si un joueur touche un bloc de mort, on le tue
    if (object->getTypeId() == Player::TYPE_ID) {
        game.kill(object);
        game.setMode(Game::Mode::LOST);
        game.setDeathCause(Game::DeathCause::KILLED);
    }
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
