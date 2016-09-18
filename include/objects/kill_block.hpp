#pragma once
#include <SFML/Graphics.hpp>
#include "../level_data.hpp"
#include "block.hpp"

class Game;

/**
 * Le bloc tueur est un bloc spécial qui tue les joueurs
 * qui arrivent dessus et fait perdre la partie
 */
class KillBlock : public Block {
public:
    /**
     * Identifiant unique du type "bloc tueur"
     */
    static const unsigned int TYPE_ID;

    /**
     * Charge un bloc tueur depuis le fichier donné et retourne
     * un pointeur vers cet objet
     */
    static LevelData::ObjectPtr load(std::ifstream& file);

    KillBlock(sf::Vector2f position = sf::Vector2f());
    virtual LevelData::ObjectPtr clone() const override;
    virtual void draw(sf::RenderWindow&) override;
    virtual unsigned int getTypeId() const override;

protected:
    virtual void activate(Game& game, Object& object) override;

private:
    sf::Sprite icon_sprite;
};
