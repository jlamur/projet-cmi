#pragma once
#include <SFML/Graphics.hpp>
#include "../level_data.hpp"
#include "block.hpp"

class Game;

/**
 * Le bloc d'arrivée est un bloc spécial qui tue les joueurs
 * qui arrivent dessus et fait gagner le niveau lorsqu'il n'y a
 * plus de joueurs
 */
class FinishBlock : public Block {
public:
    /**
     * Identifiant unique du type "bloc d'arrivée"
     */
    static const unsigned int TYPE_ID;

    /**
     * Charge un bloc d'arrivée depuis le fichier donné et retourne
     * un pointeur vers cet objet
     */
    static LevelData::ObjectPtr load(std::ifstream& file);

    FinishBlock(sf::Vector2f position = sf::Vector2f());
    virtual LevelData::ObjectPtr clone() const override;
    virtual void draw(sf::RenderWindow&) override;
    virtual unsigned int getTypeId() const override;

protected:
    virtual void activate(Game& game, Object& object) override;

private:
    sf::Sprite icon_sprite;
};
