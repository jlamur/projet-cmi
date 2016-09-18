#pragma once
#include <SFML/Graphics.hpp>
#include "../level_data.hpp"
#include "object.hpp"

class Game;

/**
 * Un bloc est un objet du jeu à boîte englobante carrée.
 * Il permet de réaliser des structures simples encadrant
 * les joueurs. Il sert de base aux blocs réalisant des
 * actions lorsqu'ils sont activés
 */
class Block : public Object {
public:
    /**
     * Identifiant unique du type "bloc"
     */
    static const unsigned int TYPE_ID;

    /**
     * Charge un bloc depuis le fichier donné et retourne
     * un pointeur vers cet objet
     */
    static LevelData::ObjectPtr load(std::ifstream& file);

    Block(sf::Vector2f position = sf::Vector2f());
    virtual LevelData::ObjectPtr clone() const override;
    virtual void draw(sf::RenderWindow&) override;
    virtual unsigned int getTypeId() const override;
    virtual Geometry getGeometry() const override;

protected:
    virtual void activate(Game& game, Object& object) override;

private:
    sf::Sprite sprite;
};
