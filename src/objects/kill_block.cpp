#include "manager.hpp"
#include "resource_manager.hpp"
#include "states/game.hpp"
#include "objects/player.hpp"
#include "objects/kill_block.hpp"

const unsigned int KillBlock::TYPE_ID = 5;

LevelData::ObjectPtr KillBlock::load(std::ifstream& file) {
    auto object = std::shared_ptr<KillBlock>(new KillBlock);
    object->read(file);
    return object;
}

KillBlock::KillBlock(sf::Vector2f position) : Block(position) {
    icon_sprite.setOrigin(sf::Vector2f(23, 23));
    icon_sprite.setTexture(*ResourceManager::get().getTexture(
        "objects/kill_block.tga"
    ));
}

LevelData::ObjectPtr KillBlock::clone() const {
    return LevelData::ObjectPtr(new KillBlock(*this));
}

void KillBlock::draw(sf::RenderWindow& window) {
    // on dessine le bloc normal
    Block::draw(window);

    // on dessine l'icône
    icon_sprite.setPosition(getPosition());
    window.draw(icon_sprite);
}

unsigned int KillBlock::getTypeId() const {
    return TYPE_ID;
}

void KillBlock::activate(Game& game, Object& object) {
    Block::activate(game, object);

    // si un joueur touche un bloc de mort, on le tue
    if (object.getTypeId() == Player::TYPE_ID) {
        game.kill(object);
        game.setMode(Game::Mode::LOST);
        game.setDeathCause(Game::DeathCause::KILLED);
    }
}
