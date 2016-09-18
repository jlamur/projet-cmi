#pragma once
#include <SFML/Graphics.hpp>
#include "../level_data.hpp"
#include "object.hpp"

/**
 * Le joueur est un objet du jeu qui peut être déplacé au clavier
 * et interagit avec les autres objets. Il possède un statut particulier
 * au sein du niveau car il détermine si l'on gagne ou on perd la partie
 */
class Player : public Object {
public:
    typedef std::shared_ptr<Player> Ptr;

public:
    /**
     * Identifiant unique du type "joueur"
     */
    static const unsigned int TYPE_ID;

    /**
     * Charge un joueur depuis le fichier donné et retourne
     * un pointeur vers cet objet
     */
    static LevelData::ObjectPtr load(std::ifstream& file);

    Player(sf::Vector2f position = sf::Vector2f());
    virtual LevelData::ObjectPtr clone() const override;
    virtual void draw(sf::RenderWindow&) override;
    virtual unsigned int getTypeId() const override;
    virtual Geometry getGeometry() const override;


    /**
     * Le numéro du joueur qui détermine quel joueur
     * physique contrôle cette balle.
     *
     * (Lecture et écriture.)
     */
    unsigned int getPlayerNumber() const;
    void setPlayerNumber(unsigned int);

protected:
    virtual void activate(Game& game, Object& object) override;
    virtual sf::Vector2f getForces(const Game& game) const override;

private:
    sf::CircleShape sprite;
    sf::CircleShape shadow_sprite;
    unsigned int player_number;
};
