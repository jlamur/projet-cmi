#include "manager.hpp"
#include "resource_manager.hpp"
#include "states/game.hpp"
#include "objects/finish_block.hpp"

const unsigned int FinishBlock::TYPE_ID = 4;

LevelData::ObjectPtr FinishBlock::load(std::ifstream& file) {
    auto object = std::shared_ptr<FinishBlock>(new FinishBlock);
    object->read(file);
    return object;
}

FinishBlock::FinishBlock(sf::Vector2f position) : Block(position) {
    icon_sprite.setOrigin(sf::Vector2f(23, 41));
    icon_sprite.setTexture(*ResourceManager::get().getTexture(
        "objects/finish_block.tga"
    ));
}

LevelData::ObjectPtr FinishBlock::clone() const {
    return LevelData::ObjectPtr(new FinishBlock(*this));
}

void FinishBlock::draw(sf::RenderWindow& window) {
    // on dessine le bloc normal
    Block::draw(window);

    // on dessine l'icône
    icon_sprite.setPosition(getPosition());
    window.draw(icon_sprite);
}

unsigned int FinishBlock::getTypeId() const {
    return FinishBlock::TYPE_ID;
}

void FinishBlock::activate(Game& game, Object& object) {
    Block::activate(game, object);

    // si un joueur touche le bloc d'arrivée, on le tue
    // s'il ne reste plus de joueur, on a gagné
    if (object.getTypeId() == Player::TYPE_ID) {
        if (game.getLevel().getPlayers().size() == 1) {
            game.setMode(Game::Mode::WON);
        }

        game.kill(object);
    }
}
