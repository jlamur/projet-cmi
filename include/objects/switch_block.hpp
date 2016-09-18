#pragma once
#include <SFML/Graphics.hpp>
#include "../level_data.hpp"
#include "block.hpp"

class Game;

/**
 * Le bloc changeur est un bloc spécial qui inverse la polarité
 * des joueurs qui passent dessus. Ce bloc ne peut être utilisé
 * qu'une seule fois par partie
 */
class SwitchBlock : public Block {
public:
    /**
     * Identifiant unique du type "bloc changeur"
     */
    static const unsigned int TYPE_ID;

    /**
     * Charge un bloc changeur depuis le fichier donné et retourne
     * un pointeur vers cet objet
     */
    static LevelData::ObjectPtr load(std::ifstream& file);

    SwitchBlock(sf::Vector2f position = sf::Vector2f());
    virtual LevelData::ObjectPtr clone() const override;
    virtual void draw(sf::RenderWindow&) override;
    virtual unsigned int getTypeId() const override;

protected:
    virtual void activate(Game& game, Object& object) override;

private:
    sf::Sprite icon_sprite;
    float opacity;
    bool used;
};
